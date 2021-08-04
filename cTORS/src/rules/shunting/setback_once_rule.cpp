#include "BusinessRules.h"

/**
 * Validates a SetbackAction for the given state. 
 * The SetbackAction is invalid iff
 * 1. The ShuntingUnit does not have a previous position
 * @return A pair describing 1) whether the action is valid, and 2) if not, why
 * 
 * Note: current implementation is not up to date with how ShuntingUnit direction is implemented.
 */
pair<bool, string> setback_once_rule::IsValid(const State* state, const Action* action) const {
	// TODO, at the moment the direction of the train, the previous parameter is not set too null if setback, current check is no longer valid
	if (auto sa = dynamic_cast<const SetbackAction*>(action)) {
		auto su = action->GetShuntingUnit();
		auto dir = state->GetPrevious(su);
		if(dir == nullptr)
			return make_pair(false, "Shunting Unit " + su->toString() + " is already set back.");
	}
	return make_pair(true, "");
}

