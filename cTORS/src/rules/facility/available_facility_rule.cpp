#include "BusinessRules.h"

/*

Rule that verifies that tasks assigned to a facility are only executed when that facility is available.

*/

pair<bool, string> available_facility_rule::IsValid(const State* state, const Action* action) const {
	if (auto sa = dynamic_cast<const ServiceAction*>(action)) {
		auto ta = sa->GetTask();
		auto fa = sa->GetFacility();
		if (!fa->IsAvailable(state->GetTime(), ta->duration))
			return make_pair(false, fa->toString() + " is not available from " + to_string(state->GetTime()) + " to " + to_string(ta->duration) + ".");
	}
	return make_pair(true, "");
}

