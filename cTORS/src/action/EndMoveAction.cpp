#include "Action.h"
#include "State.h"

void EndMoveAction::Start(State* state) const {
	const ShuntingUnit* su = GetShuntingUnit();
	state->SetMoving(su, false);
	state->SetInNeutral(su, true);
	state->AddActiveAction(su, this);
}

void EndMoveAction::Finish(State* state) const {
	state->RemoveActiveAction(su, this);
}

const string EndMoveAction::toString() const {
	return "EndMove " + su->toString();
}

const Action* EndMoveActionGenerator::Generate(const State* state, const SimpleAction& action) const {
	auto su = action.GetShuntingUnit();
	auto suState = state->GetShuntingUnitState(su);
	return new EndMoveAction(su, su->GetStartUpTime(suState.frontTrain));
}

void EndMoveActionGenerator::Generate(const State* state, list<const Action*>& out) const {
	auto& sus = state->GetShuntingUnits();
	for (auto& [su, suState]: state->GetShuntingUnitStates()) {
		auto& track = suState.position;
		if (suState.moving && !suState.beginMoving && !suState.waiting && track->standingAllowed && !suState.HasActiveAction()) {
			out.push_back(Generate(state, EndMove(su)));
		}
	}
}