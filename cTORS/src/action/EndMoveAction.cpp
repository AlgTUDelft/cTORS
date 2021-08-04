#include "Action.h"
#include "State.h"

void EndMoveAction::Start(State* state) const {
	const ShuntingUnit* su = GetShuntingUnit();
	state->SetMoving(su, false);
	state->AddActiveAction(su, this);
}

void EndMoveAction::Finish(State* state) const {
	state->RemoveActiveAction(su, this);
}

const string EndMoveAction::toString() const {
	return "EndMove " + su->toString();
}