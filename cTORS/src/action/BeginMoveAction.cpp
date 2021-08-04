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