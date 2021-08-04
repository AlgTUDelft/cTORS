#include "Action.h"
#include "State.h"

const Action* MoveHelperGenerator::Generate(const State* state, const SimpleAction& action) const {
	auto su = InitialCheck(state, action);
	auto& suState  = state->GetShuntingUnitState(su);
	if(instanceof<BeginMove>(&action)) {
		return new BeginMoveAction(su, su->GetStartUpTime(state->GetFrontTrain(su)));
	} else if(instanceof<EndMove>(&action)) {
		return new EndMoveAction(su, su->GetStartUpTime(state->GetFrontTrain(su)));
	} else if(instanceof<Move>(&action)) {
		auto move = static_cast<const Move*>(&action);
		auto previous = suState.inNeutral ? nullptr : suState.previous;
		auto destination = location->GetTrackByID(move->GetDestinationID());
		auto path = location->GetNeighborPath({previous, suState.position}, destination); 
		return new MoveAction(su, vector<const Track*>(path.route.begin(), path.route.end()), path.length, true);
	}
	throw invalid_argument("MoveHelperGenerator only generates Actions for BeginMove, EndMove and Move, not for " + action.toString());
}

void MoveHelperGenerator::Generate(const State* state, list<const Action*>& out) const {
	if(state->GetTime()==state->GetEndTime()) return;
	list<const Action*> replacements;
	unordered_map<const ShuntingUnit*, vector<const Track*>> visitedNeighbors;
	unordered_map<const ShuntingUnit*, bool> beginMove;
	for(auto su: state->GetShuntingUnits()) beginMove[su] = false;
	out.remove_if([this, state, &replacements, &visitedNeighbors, &beginMove] (const Action*& a) -> bool {
		if(!instanceof<MoveAction>(a)) return false;
		auto su = a->GetShuntingUnit();
		auto& suState = state->GetShuntingUnitState(su);
		if(!suState.moving && !beginMove.at(su)) {
			replacements.push_back(Generate(state, BeginMove(su)));
			beginMove[su] = true;
		} else if (suState.moving) {
			const MoveAction* m = static_cast<const MoveAction*>(a);
			const Track* rail = nullptr;
			for(auto t: m->GetTracks()) {
				if(t == m->GetTracks().at(0)) continue;
				if(t->GetType() == TrackPartType::Railroad) {
					rail = t;
					break;
				}
			}
			assert(rail != nullptr);
			auto it = find(visitedNeighbors[su].begin(), visitedNeighbors[su].end(), rail);
			if(it == visitedNeighbors[su].end()) {
				visitedNeighbors[su].push_back(rail);
				replacements.push_back(Generate(state, Move(su, rail)));	
			}
		}
		delete a;
		return true;
	});
	out.insert(out.end(), replacements.begin(), replacements.end());

	//Generate EndMoveAction%s
	auto& sus = state->GetShuntingUnits();
	for (auto& [su, suState]: state->GetShuntingUnitStates()) {
		auto& track = suState.position;
		if (suState.moving && !suState.beginMoving && !suState.waiting && track->parkingAllowed && !suState.HasActiveAction()) {
			out.push_back(Generate(state, EndMove(su)));
		}
	}

}