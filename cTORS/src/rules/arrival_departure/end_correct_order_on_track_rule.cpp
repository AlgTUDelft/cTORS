#include "BusinessRules.h"


/**
 * Validates an ExitAction for the given state. 
 * The ExitAction is invalid if
 * 1. The end time of the scenario is reached, and
 * 2. The outgoing train is blocked to leave from its side track
 * @return A pair describing 1) whether the action is valid, and 2) if not, why
 */
pair<bool, string> end_correct_order_on_track_rule::IsValid(const State* state, const Action* action) const {
	if(state->GetTime() < state->GetEndTime())
		return make_pair(true, "");
	if (auto ea = dynamic_cast<const ExitAction*>(action)) {
		auto su = ea->GetShuntingUnit();
		auto parking = ea->GetOutgoing()->GetParkingTrack();
		auto side = ea->GetOutgoing()->GetSideTrack();
		if (!state->CanMoveToSide(su, side))
			return make_pair(false, "Outstanding shunting unit " + su->toString() + " is not standing on the right position on its track.");
	}
	return make_pair(true, "");
}