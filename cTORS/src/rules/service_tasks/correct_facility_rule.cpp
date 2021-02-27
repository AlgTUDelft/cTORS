#include "BusinessRules.h"

/*

Rule that verifies that service tasks are executed at the correct facility.

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

