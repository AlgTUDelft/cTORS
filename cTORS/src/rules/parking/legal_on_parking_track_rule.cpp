#include "BusinessRules.h"

/*

Rule that verifies that parked shunting units are on a track where parking is allowed.

*/

pair<bool, string> legal_on_parking_track_rule::IsValid(const State* state, const Action* action) const {
	auto su = action->GetShuntingUnit();
	bool stopping = false;
	if (auto ema = dynamic_cast<const EndMoveAction*>(action)) stopping = true;
	else if (auto wa = dynamic_cast<const WaitAction*>(action)) stopping = true; 
	if(stopping) {
		auto position = state->GetPosition(su);
		if (!position->parkingAllowed)
			return make_pair(false, "Parking is not allowed on track " + position->toString() + ".");
	}
	return make_pair(true, "");
}

