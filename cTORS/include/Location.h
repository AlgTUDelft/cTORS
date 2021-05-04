#pragma once
#ifndef LOCATION_H
#define LOCATION_H
#include <vector>
#include <fstream>
#include <exception>
#include <unordered_map>
#include <filesystem>
#include "Utils.h"
#include "Track.h"
#include "Facility.h"
#include "Exceptions.h"
#include "Utils.h"
using namespace std;
#define MAX_PATH_LENGTH INT32_MAX

struct Path {
	list<const Track*> route;
	int length;
	Path() : length(MAX_PATH_LENGTH) {}
	Path(list<const Track*> route, int length) : route(route), length(length) {}
	string toString() const;
	inline const Track* GetDestination() const { return route.back(); }
};

typedef pair<const Track*, const Track*> Position;

class Location
{
private:
	static const string locationFileString;
	
	string path;
	vector<Track*> tracks;
	vector<Facility*> facilities;
	unordered_map<Position, double> distanceMatrix;
	//A shortest path map for each setbackTime. The map is from (start_position, end_position) -> path
	map<int, unordered_map<pair<Position, Position>, Path>> shortestPath;
	// Path from a Railroad position to all neighboring RailRoad positions
	unordered_map<Position, unordered_map<Position, Path>> neighborPaths;
	map<string, Track*> trackIndex;
	int movementConstant;
	map<const TrackPartType, int> moveDuration;
	
	void ImportTracks(const PBLocation& pb_location);
	void ImportFacilities(const PBLocation& pb_location);
	void ImportDistanceMatrix(const PBLocation& pb_location);
public:
	Location() = delete;
	Location(const string &path);
	Location(const Location& location) = default;
	~Location();
	
	inline const string& GetLocationFilePath() const { return path; }

	inline Track* GetTrackByID(const string& id) const { return trackIndex.at(id); }

	inline const vector<Track*>& GetTracks() const { return tracks; }
	inline const vector<Facility*>& GetFacilities() const { return facilities; }
	const Facility* GetFacilityByID(int id) const;

	inline double GetDistance(const Track* from, const Track* to) const { return distanceMatrix.at({from, to}); }
	inline int GetDurationByType(const Track* track) const { 
		return (track->GetType() == TrackPartType::Railroad && track->GetLength() == 0) ? 0  : moveDuration.at(track->GetType()); }
	
	void CalcNeighboringPaths(bool byType);
	void CalcShortestPaths(bool byType, const TrainUnitType* type);
	inline Path GetShortestPath(const TrainUnitType* type, const Position& from, const Position& to) const {
		return shortestPath.at(type->setbackTime).at({from, to}); }
	inline const unordered_map<Position,Path>& GetNeighboringPaths(const Position& from) const { return neighborPaths.at(from); }
	const Path& GetNeighborPath(const Position& from, const Track* destination) const;
};

#endif