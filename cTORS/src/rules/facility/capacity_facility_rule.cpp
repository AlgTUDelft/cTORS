#include "BusinessRules.h"

/**
 * Validates a ServiceAction for the given state. 
 * The ServiceAction is invalid iff
 * 1. The servicing of this Train would make the facility exceed its capacity.
 * @return A pair describing 1) whether the action is valid, and 2) if not, why
 */
pair<bool, string> capacity_facility_rule::IsValid(const State* state, const Action* action) const {
	if (auto sa = dynamic_cast<const ServiceAction*>(action)) {
		auto fa = sa->GetFacility();
		auto& sus = state->GetShuntingUnits();
		int count = 0;
		for (auto su : sus) {
			auto& actions = state->GetActiveActions(su);
			for (auto a : actions) {
				if (auto _sa = dynamic_cast<const ServiceAction*>(a)) {
					if (_sa->GetFacility() == fa) 
						if(++count > fa->GetCapacity())
							return make_pair(false, fa->toString() + " is at its capacity and cannot service train " + sa->GetTrain()->toString() + " from " + sa->GetShuntingUnit()->toString()+".");
				}
			}
		}
	}
	return make_pair(true, "");
}

