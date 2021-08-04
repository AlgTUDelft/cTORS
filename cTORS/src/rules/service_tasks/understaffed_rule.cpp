#include "BusinessRules.h"

/**
 * Validates a ServiceAction for the given state. 
 * The ServiceAction is invalid iff
 * 1. Not yet implemented
 * @return A pair describing 1) whether the action is valid, and 2) if not, why
 */
pair<bool, string> understaffed_rule::IsValid(const State* state, const Action* action) const {
	//TODO
	return make_pair(true, "");
}

