#include "BusinessRules.h"

/**
 * Validates a ServiceAction for the given state. 
 * The ServiceAction is invalid iff
 * 1. Facility is not available at the current time up and until completion of the task.
 * @return A pair describing 1) whether the action is valid, and 2) if not, why
 */
pair<bool, string> available_facility_rule::IsValid(const State* state, const Action* action) const {
	if (auto sa = dynamic_cast<const ServiceAction*>(action)) {
		auto ta = sa->GetTask();
		auto fa = sa->GetFacility();
		if (!fa->IsAvailable(state->GetTime(), ta->duration))
			return make_pair(false, fa->toString() + " is not available from " + to_string(state->GetTime()) +
				" to " + to_string(state->GetTime() + ta->duration) + ".");
	}
	return make_pair(true, "");
}

