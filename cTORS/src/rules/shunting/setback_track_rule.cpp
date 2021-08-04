#include "BusinessRules.h"

/**
 * Validates a SetbackAction for the given state. 
 * The SetbackAction is invalid iff
 * 1. The ShuntingUnit's current Track does not allow for saw movements.
 * @return A pair describing 1) whether the action is valid, and 2) if not, why
 */
pair<bool, string> setback_track_rule::IsValid(const State* state, const Action* action) const {
	if (auto sa = dynamic_cast<const SetbackAction*>(action)) {
		auto su = action->GetShuntingUnit();
		auto track = state->GetPosition(su);
		if (!track->sawMovementAllowed)
			return make_pair(false, "Saw movements are not allowed on track " + track->toString()+".");
	}
	return make_pair(true, "");
}

