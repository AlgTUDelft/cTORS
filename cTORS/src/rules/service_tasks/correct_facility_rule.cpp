#include "BusinessRules.h"

/**
 * Validates a ServiceAction for the given state. 
 * The ServiceAction is invalid iff
 * 1. The Facility cannot execute the service Task.
 * @return A pair describing 1) whether the action is valid, and 2) if not, why
 */
pair<bool, string> correct_facility_rule::IsValid(const State* state, const Action* action) const {
	if (auto sa = dynamic_cast<const  ServiceAction*>(action)) {
		auto task = sa->GetTask();
		auto facility = sa->GetFacility();
		if (!facility->ExecutesTask(task))
			return make_pair(false, facility->toString() + " cannot perform task " + task->toString() + ".");
	}
	return make_pair(true, "");
}

