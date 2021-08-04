#include "BusinessRules.h"

/**
 * Validates an EndMoveAction, WaitAction or MoveAction for the given state. 
 * The EndMoveAction or WaitAction is invalid iff
 * 1. The current Track of the ShuntingUnit does not allow for parking.
 * 
 * The MoveAction is invalid iff
 * 1. The MoveAction is not a step-by-step move and the destination Track does not allow for parking.
 * @return A pair describing 1) whether the action is valid, and 2) if not, why
 */
pair<bool, string> legal_on_parking_track_rule::IsValid(const State* state, const Action* action) const {
	auto su = action->GetShuntingUnit();
	const Track* position;
	if(instanceof<EndMoveAction>(action) || instanceof<WaitAction>(action))
		position = state->GetPosition(su);
	else if (instanceof<MoveAction>(action)) {
		auto ma = dynamic_cast<const MoveAction*>(action);
		if (!ma->IsStepMove()) position = ma->GetDestinationTrack();
		else return make_pair(true, "");
	} else return make_pair(true, "");
	if (!position->parkingAllowed)
		return make_pair(false, "Parking is not allowed on track " + position->toString() + ".");
	return make_pair(true, "");
}

