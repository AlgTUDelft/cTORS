#include "BusinessRules.h"

/**
 * Validates a SplitAction or CombineAction for the given state. 
 * The SplitAction or CombineAction is invalid iff
 * 1. not yet implemented.
 * @return A pair describing 1) whether the action is valid, and 2) if not, why
 */
pair<bool, string> order_preserve_rule::IsValid(const State* state, const Action* action) const {
	if(!instanceof<SplitAction>(action) && !instanceof<CombineAction>(action)) return make_pair(true, "");
	auto& suState = state->GetShuntingUnitState(action->GetShuntingUnit());
	auto track = suState.position;
	vector<const Train*> originalOrder;
	vector<const Train*> newOrder;
	if(auto sa = dynamic_cast<const SplitAction*>(action)) {
		
	} else if(auto ca = dynamic_cast<const CombineAction*>(action)) {
		
	}
	//TODO

	return make_pair(true, "");
}