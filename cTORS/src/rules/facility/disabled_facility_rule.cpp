#include "BusinessRules.h"

/*

Rule that verifies that no tasks are assigned to facilities which are disabled by a disturbance.

*/

pair<bool, string> disabled_facility_rule::IsValid(const State* state, const Action* action) const {
	if (auto sa = dynamic_cast<const ServiceAction*>(action)) {
		//TODO	
	}
	return make_pair(true, "");
}

