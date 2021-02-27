#include "BusinessRules.h"

/*

Rule that verifies that combine and split actions on shunting units
are only performed on tracks where parking is allowed.

*/

pair<bool, string> park_combine_split_rule::IsValid(const State* state, const Action* action) const {
	if(!instanceof<SplitAction>(action) && !instanceof<CombineAction>(action)) return make_pair(true, "");
	auto su = action->GetShuntingUnit();
	auto track = state->GetPosition(su);
	if(!track->parkingAllowed)
		return make_pair(false, "Shunting unit " + su->toString() + " is not allowed to combine/split on track "
			+ track->toString() + " because it is not a parking track.");
	return make_pair(true, "");
}