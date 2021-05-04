#include "Action.h"
#include "State.h"

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
	return "Move " + su->toString() + " to " + GetDestinationTrack()->toString();
}

MoveActionGenerator::MoveActionGenerator(const json& params, const Location* location) : ActionGenerator(params, location) {
	params.at("no_routing_duration").get_to(noRoutingDuration);
	params.at("constant_time").get_to(constantTime);
	params.at("default_time").get_to(defaultTime);
	params.at("norm_time").get_to(normTime);
	params.at("walk_time").get_to(walkTime);
}

const Path& MoveActionGenerator::GeneratePath(const State* state, const Move& move) const {
	auto su = state->GetShuntingUnitByTrainIDs(move.GetTrainIDs());
	if(!state->HasShuntingUnit(su)) throw InvalidActionException("The shunting unit does not exist.");
	auto& suState  = state->GetShuntingUnitState(su);
	if(!suState.moving || suState.HasActiveAction()) throw InvalidActionException("The shunting unit is already active.");
	auto previous = suState.inNeutral ? nullptr : suState.previous;
	auto destination = location->GetTrackByID(move.GetDestinationID());
	return location->GetNeighborPath({previous, suState.position}, destination); 
}

const Action* MoveActionGenerator::Generate(const State* state, const SimpleAction& action) const {
	auto move = static_cast<const Move*>(&action);
	auto su = state->GetShuntingUnitByTrainIDs(action.GetTrainIDs());
	auto& path = GeneratePath(state, *move);
	return new MoveAction(su, vector<const Track*>(path.route.begin(), path.route.end()), path.length);
}

void MoveActionGenerator::Generate(const State* state, list<const Action*>& out) const {
	if(state->GetTime()==state->GetEndTime()) return;
	auto& sus = state->GetShuntingUnits();
	for (const auto& [su, suState] : state->GetShuntingUnitStates()) {
		if (!suState.moving || suState.HasActiveAction()) continue;
		auto track = suState.position;
		auto previous = suState.inNeutral ? nullptr : suState.previous;
		auto& previous_list = suState.inNeutral ? track->GetNeighbors() : vector<const Track*>({previous});
		for(auto& previous: previous_list) {
			auto& paths = location->GetNeighboringPaths({previous, track});
			for(auto& [dest, path]: paths) {
				out.push_back(Generate(state, Move(su, dest.second)));
			}
		}
	}
}