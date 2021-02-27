#include "BusinessRules.h"

/*

Rule that verifies that moving shunting units are not blocked by other shunting units.

*/

pair<bool, string> blocked_track_rule::IsValid(const State* state, const Action* action) const {
	auto ress = action->GetReservedTracks();
	for (auto res : ress) {
		if (state->IsReserved(res)) return make_pair(false, "Track " + res->toString() + " is reserved.");
	}
	return make_pair(true, "");
}

