#include "BusinessRules.h"

/**
 * Validates an Action for the given state. 
 * The Action is invalid iff
 * 1. The Action uses a Track that is reserved in the current State.
 * @return A pair describing 1) whether the action is valid, and 2) if not, why
 * 
 * In the current version this business rule is no different from blocked_track_rule. 
 * If multi-move actions are introduced, this rule will check that the path is never blocked by other 
 * ShuntingUnits
 */
pair<bool, string> blocked_track_rule::IsValid(const State* state, const Action* action) const {
	auto ress = action->GetReservedTracks();
	for (auto res : ress) {
		if (state->IsReserved(res)) return make_pair(false, "Track " + res->toString() + " is reserved.");
	}
	return make_pair(true, "");
}

