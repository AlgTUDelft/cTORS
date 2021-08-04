#include "BusinessRules.h"

/**
 * Validates a ServiceAction for the given state. 
 * The ServiceAction is invalid iff
 * 1. The facility is not available (Check not yet implemented)
 * @return A pair describing 1) whether the action is valid, and 2) if not, why
 */
pair<bool, string> disabled_facility_rule::IsValid(const State* state, const Action* action) const {
	if (auto sa = dynamic_cast<const ServiceAction*>(action)) {
		//TODO	
	}
	return make_pair(true, "");
}

