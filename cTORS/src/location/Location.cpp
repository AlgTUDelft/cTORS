#include "Location.h"
#if DEBUG
#include <chrono>
#endif

namespace fs = std::filesystem;
using namespace std;

const string Location::locationFileString = "location.json";

Location::Location(const string &folderName) {
	path = folderName;
	try {
		PBLocation pb_location;
		parse_json_to_pb(fs::path(folderName) / fs::path(locationFileString), &pb_location);
		ImportTracks(pb_location);
		ImportFacilities(pb_location);
		ImportDistanceMatrix(pb_location);
		movementConstant = pb_location.movementconstant();
		int movementTrackCoefficient = pb_location.movementtrackcoefficient();
		int movementSwitchCoefficient = pb_location.movementswitchcoefficient();
		moveDuration[TrackPartType::Railroad] = movementTrackCoefficient;
		moveDuration[TrackPartType::Switch] = movementSwitchCoefficient;
		moveDuration[TrackPartType::EnglishSwitch] = 2 * movementSwitchCoefficient;
		moveDuration[TrackPartType::HalfEnglishSwitch] = 2 * movementSwitchCoefficient;
		moveDuration[TrackPartType::InterSection] = 0;
		moveDuration[TrackPartType::Bumper] = 0;
		CalcNeighboringPaths(true); // TODO read parameter from config file
	}
	catch (exception& e) {
		cout << "Error in loading location: " << e.what() << "\n";
		throw e;
	}
}

Location::~Location()
{
	DELETE_VECTOR(tracks)
	DELETE_VECTOR(facilities)
	trackIndex.clear();
}

const Facility* Location::GetFacilityByID(int id) const {
	auto it = find_if(facilities.begin(), 
             facilities.end(), 
             [id] (const Facility* f) -> bool { return f->GetID() == id; });
	return it == facilities.end() ? nullptr : *it;
}

list<Position> GetAllPositions(const vector<Track*>& tracks) {
	list<Position> positions;
	for(auto track: tracks) {
		if(track->type!=TrackPartType::Railroad) continue;
		for(auto neighbor: track->GetNeighbors()) {
			positions.push_back({neighbor, track});
		}
	}
	return positions;
}

void Location::CalcNeighboringPaths(bool byType) {
	if(neighborPaths.size() > 0) return;
	auto positions = GetAllPositions(tracks);
	for(auto& pos: positions) {
		list<Position> open({pos});
		unordered_map<const Track*, const Track*> previous = {{pos.second, pos.first}};
		neighborPaths[pos];
		while(open.size() > 0) {
			auto current = open.front();
			open.pop_front();
			for(auto next: current.second->GetNextTrackParts(current.first)) {
				if(next->type == TrackPartType::Railroad) {
					Position dest {current.second, next};
					list<const Track*> route {dest.first, dest.second};
					int length = byType ? GetDurationByType(dest.second) : GetDistance(dest.first, dest.second);
					while(route.front()->type!=TrackPartType::Railroad) {
						auto prev = previous.at(route.front());
						length += byType ? GetDurationByType(route.front()) : GetDistance(prev, route.front());
						route.push_front(prev);
					}
					neighborPaths[pos][dest] = Path(route, length);
					debug_out("Found a path from " << pos.first->toString()  << ">" << pos.second->toString() << " to " << dest.first->toString()  << ">" << dest.second->toString());
				} else {
					open.push_back({current.second, next});
					previous[next] = current.second;
				} 
			}
		}
	}
}

/**
 * Calculate the shortest paths.
 * Param byType: whether the basic distance is based on track type, or on the distance matrix
 * Param type: The train type for which the shortest paths are calculated. 
 */
void Location::CalcShortestPaths(bool byType, const TrainUnitType* type) {
	auto setbackTime = type->setbackTime;
	auto& shortestPath = this->shortestPath[setbackTime];
	if(shortestPath.size() > 0) return;
	#if DEBUG
	auto begin = chrono::steady_clock::now();
	#endif
	auto positions = GetAllPositions(tracks);
	//Initialize the distance matrix
	for(auto& pos1: positions) {
		for(auto& pos2: positions) {
			shortestPath[{pos1,pos2}];
		}
	}
	//Initialize all railroad to railroad distances
	CalcNeighboringPaths(byType);
	for(auto& [pos, paths]: neighborPaths)
		for(auto& [dest, path]: paths)
			shortestPath[{pos, dest}] = path;
	//Initialize all set-back distances
	for(auto& pos: positions) {
		if(!pos.second->sawMovementAllowed) continue;
		for(auto prev: pos.second->GetNextTrackParts(pos.first))
			shortestPath[{pos, {prev, pos.second}}] = Path({pos.second, pos.second}, setbackTime);
	}
	debug_out("Starting Floyd-Warshall");
	//Calculate all-pairs shortest path (Floyd-Warshal)
	for(auto& k: positions) {
		for(auto& i: positions) {
			for(auto& j: positions) {
				Path& p1 = shortestPath.at({i,k});
				Path& p2 = shortestPath.at({k,j});
				Path& p3 = shortestPath.at({i,j});
				if((p1.length < MAX_PATH_LENGTH && p2.length < MAX_PATH_LENGTH) && p1.length + p2.length < p3.length) {
					p3.route = p1.route;
					p3.route.insert(p3.route.end(), next(p2.route.begin()), p2.route.end());
					p3.length = p1.length + p2.length;
				}
			}
		}
	}
	#if DEBUG
	debug_out("Found paths: ");
	for(auto& [from_to, path]: shortestPath) {
		auto from = from_to.first;
		auto to = from_to.second;
		if(path.length < MAX_PATH_LENGTH) {
			debug_out(from.second->name << " -> " << to.second->name << ": " << path.toString());
		}
	}
	auto end = chrono::steady_clock::now();
	debug_out("Calculating shortest paths finished in "
		<< chrono::duration_cast<chrono::microseconds>(end - begin).count() << "µs" << " // "
		<< chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "ms" << " // "
		<< chrono::duration_cast<chrono::seconds>(end - begin).count() << "s");
	#endif
}

const Path& Location::GetNeighborPath(const Position& from, const Track* destination) const {
	auto& prev_list = from.first == nullptr ? from.second->GetNeighbors() : vector<const Track*>({from.first});
	for(auto& prev: prev_list) {
		for(auto& [dest, path]: neighborPaths.at({prev, from.second})) {
			if(dest.second == destination) return path;
		}
	}
	throw NonExistingPathException("Could not find path from " +from.second->toString() + " to " + destination->toString());
}

void Location::ImportTracks(const PBLocation& pb_location) {
	unordered_map<Track*, vector<UInt>> aSides;
	unordered_map<Track*, vector<UInt>> bSides;
	for (auto& track : pb_location.trackparts()) {
		Track* t = new Track(track);
		aSides[t] = vector<UInt>(track.aside().begin(), track.aside().end());
		bSides[t] = vector<UInt>(track.bside().begin(), track.bside().end());
		tracks.push_back(t);
		trackIndex[t->id] = t;
		debug_out("Imported track " << t->toString());
	}
	for (Track* t : tracks) {
		vector<UInt> saside = aSides[t];
		vector<UInt> sbside = bSides[t];
		auto aside = vector<const Track*>(saside.size());
		auto bside = vector<const Track*>(sbside.size());
		for (int j = 0; j != saside.size(); j++)
			aside[j] = trackIndex[to_string(saside[j])];
		for (int j = 0; j != sbside.size(); j++)
			bside[j] = trackIndex[to_string(sbside[j])];
		t->AssignNeighbors(aside, bside);
	}
	debug_out("finished loading tracks from JSON");
}

void Location::ImportFacilities(const PBLocation& pb_location) {
	for (auto& pb_f : pb_location.facilities()) {
		Facility* f = new Facility(pb_f);
		auto sTracks = vector<UInt>(pb_f.relatedtrackparts().begin(), pb_f.relatedtrackparts().end());
		vector<Track*> tracks;
		for (auto s : sTracks) {
			Track* t = trackIndex[to_string(s)];
			tracks.push_back(t);
			t->AddFacility(f);
		}
		f->AssignTracks(tracks);
		facilities.push_back(f);
		debug_out("Imported facility " << f->toString());
	}
	debug_out("finished loading facilities from JSON");
}

void Location::ImportDistanceMatrix(const PBLocation& pb_location) {
	for (auto& wde : pb_location.distanceentries()) {
		string from = to_string(wde.fromtrackpartid());
		string to = to_string(wde.totrackpartid());
		int distance = wde.distanceinseconds();
		pair<Track*, Track*> key(trackIndex[from], trackIndex[to]);
		distanceMatrix[key] = distance;
	}
}

string Path::toString() const {
	ostringstream path;
	for(auto t: route) path << t->name << ", ";
	path << "length: " << length;
	return path.str();
}