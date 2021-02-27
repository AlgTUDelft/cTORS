#include "Action.h"
#include "State.h"

void BeginMoveAction::Start(State* state) const {
	state->SetMoving(GetShuntingUnit(), true);
	state->AddActiveAction(su, this);
	state->SetWaiting(su, false);
	state->SetBeginMoving(su, true);
}

void BeginMoveAction::Finish(State* state) const {
	state->RemoveActiveAction(su, this);
}

const string BeginMoveAction::toString() const {
	return "BeginMove " + su->toString();
}

const Action* BeginMoveActionGenerator::Generate(const State* state, const SimpleAction& action) const {
	auto su = action.GetShuntingUnit();
	return new BeginMoveAction(su, su->GetStartUpTime(state->GetFrontTrain(su)));
}

void BeginMoveActionGenerator::Generate(const State* state, list<const Action*>& out) const {
	//TODO check employee availability, add duration to the action for walking distance
	if(state->GetTime()==state->GetEndTime()) return;
	for (const auto [su, suState] : state->GetShuntingUnitStates()) {
		if (!state->IsWaiting(su) && !state->IsMoving(su) && !state->HasActiveAction(su)) {
			out.push_back(Generate(state, BeginMove(su)));
		}
	}
}