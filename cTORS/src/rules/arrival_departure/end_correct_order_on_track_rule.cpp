#include "BusinessRules.h"

/*

Rule that verifies that shunting units which stay in the shunting yard after 
the scheduling period will be located in the right order on their track.

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