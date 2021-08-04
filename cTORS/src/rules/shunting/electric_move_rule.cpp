#include "BusinessRules.h"

/**
 * Validates an Action for the given state. 
 * The Action is invalid iff
 * 1. The ShuntingUnit needs electrictiy.
 * 2. Any of the reserved tracks is not electrified.
 * @return A pair describing 1) whether the action is valid, and 2) if not, why
 */
pair<bool, string> electric_move_rule::IsValid(const State* state, const Action* action) const {
	auto su = action->GetShuntingUnit();
	if (!su->NeedsElectricity()) return make_pair(true, "");
	for(auto t: action->GetReservedTracks()) {
		if(!t->isElectrified)
		return make_pair(false, "Track " + t->toString() + " is not electrified.");
	}
	return make_pair(true, "");
}

