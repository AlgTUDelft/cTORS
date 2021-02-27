#include "BusinessRules.h"

/*

Rule that verifies that combining or splitting shunting units does not change
the order of train units on a track.

*/

pair<bool, string> order_preserve_rule::IsValid(const State* state, const Action* action) const {
	if(!instanceof<SplitAction>(action) && !instanceof<CombineAction>(action)) return make_pair(true, "");
	auto suState = state->GetShuntingUnitState(action->GetShuntingUnit());
	auto track = suState.position;
	vector<const Train*> originalOrder;
	vector<const Train*> newOrder;
	if(auto sa = dynamic_cast<const SplitAction*>(action)) {
		
	} else if(auto ca = dynamic_cast<const CombineAction*>(action)) {
		
	}
	//TODO

	return make_pair(true, "");
}