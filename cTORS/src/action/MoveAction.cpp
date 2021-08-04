#include "Action.h"
#include "State.h"

vector<string> TransformTrackVector(const vector<const Track*>& tracks) {
	vector<string> trackIDs(tracks.size());
	transform(tracks.begin(), tracks.end(), trackIDs.begin(), [](const Track* t) -> const string& {return t->GetID(); });
	return trackIDs;
}

vector<string> TransformTrackVector(const list<const Track*>& tracks) {
	vector<string> trackIDs(tracks.size());
	transform(tracks.begin(), tracks.end(), trackIDs.begin(), [](const Track* t) -> const string& {return t->GetID(); });
	return trackIDs;
}

MultiMove::MultiMove(const vector<int>& trainIDs, const vector<const Track*>& tracks)
	: MultiMove(trainIDs, TransformTrackVector(tracks)) {}

MultiMove::MultiMove(const ShuntingUnit* su, const vector<const Track*>& tracks)
	: MultiMove(su->GetTrainIDs(), TransformTrackVector(tracks)) {}

MultiMove::MultiMove(const vector<int>& trainIDs, const list<const Track*>& tracks)
	: MultiMove(trainIDs, TransformTrackVector(tracks)) {}

MultiMove::MultiMove(const ShuntingUnit* su, const list<const Track*>& tracks)
	: MultiMove(su->GetTrainIDs(), TransformTrackVector(tracks)) {}

void MoveAction::Start(State* state) const {
	state->AddActiveAction(su, this);
	auto destination = GetDestinationTrack();
	auto& suState = state->GetShuntingUnitState(su);
	if(suState.inNeutral && suState.position->IsSameSide(suState.previous, destination))
		state->SwitchFrontTrain(su);
	auto previous = GetPreviousTrack();
	state->MoveShuntingUnit(su, destination, previous);
	state->ReserveTracks(GetReservedTracks());
	state->SetInNeutral(su, false);
	state->SetBeginMoving(su, false);
}

void MoveAction::Finish(State* state) const {
	state->RemoveActiveAction(su, this);
	state->FreeTracks(GetReservedTracks());
}

const string MoveAction::toString() const {
	return "Move " + su->toString() + " to " + GetDestinationTrack()->toString() + " (route: " + Join(GetTracks(), " - ") + ")";
}

MoveActionGenerator::MoveActionGenerator(const json& params, const Location* location) : ActionGenerator(params, location) {
	params.at("no_routing_duration").get_to(noRoutingDuration);
	params.at("constant_time").get_to(constantTime);
	params.at("default_time").get_to(defaultTime);
	params.at("norm_time").get_to(normTime);
	params.at("walk_time").get_to(walkTime);
}

const Path& MoveActionGenerator::GeneratePath(const State* state, const Move& move) const {
	auto su = InitialCheck(state, move);
	auto& suState  = state->GetShuntingUnitState(su);
	if(!suState.moving) throw InvalidActionException("The shunting unit is not yet moving.");
	auto previous = suState.inNeutral ? nullptr : suState.previous;
	auto destination = location->GetTrackByID(move.GetDestinationID());
	return location->GetNeighborPath({previous, suState.position}, destination); 
}

const Action* MoveActionGenerator::Generate(const State* state, const SimpleAction& action) const {
	auto su = InitialCheck(state, action);
	if(instanceof<Move>(&action)) {
		auto move = static_cast<const Move*>(&action);	
		auto& path = GeneratePath(state, *move);
		return new MoveAction(su, vector<const Track*>(path.route.begin(), path.route.end()), path.length, true);
	} else {
		auto move = static_cast<const MultiMove*>(&action);
		auto& trackIDs = move->GetTrackIDs();
		vector<const Track*> tracks(trackIDs.size());
		transform(trackIDs.begin(), trackIDs.end(), tracks.begin(), [this](const string& id) 
			-> const Track* { return  location->GetTrackByID(id); });
		auto length = location->GetDistance(tracks);
		return new MoveAction(su, tracks, length, false);
	}
	throw invalid_argument("The MoveActionGenerator can only deal with Move and MultiMove actions and not with " + action.toString());
}

void MoveActionGenerator::Generate(const State* state, list<const Action*>& out) const {
	if(state->GetTime()==state->GetEndTime()) return;
	auto& sus = state->GetShuntingUnits();
	for (const auto& [su, suState] : state->GetShuntingUnitStates()) {
		if (suState.HasActiveAction()) continue;
		auto track = suState.position;
		auto previous = suState.inNeutral ? nullptr : suState.previous;
		auto& previous_list = suState.inNeutral ? track->GetNeighbors() : vector<const Track*>({previous});
		for(auto& previous: previous_list) {
			auto& paths = location->GetPossiblePaths({previous, track});
			for(auto& path: paths) {
				out.push_back(Generate(state, MultiMove(su, path.route)));
			}
		}
	}
}