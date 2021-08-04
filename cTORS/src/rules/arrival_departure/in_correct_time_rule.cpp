#include "BusinessRules.h"

/**
 * Validates an action for the given state. 
 * The action is invalid if
 * 1. The state contains an incoming train that should have already arrived.
 * @return A pair describing 1) whether the action is valid, and 2) if not, why
 */
pair<bool, string> in_correct_time_rule::IsValid(const State* state, const Action* action) const {
	for (auto i : state->GetIncomingTrains()) {
		if (i->GetTime() < state->GetTime())
			return make_pair(false, "Shunting unit " + i->GetShuntingUnit()->toString() + " should have already arrived");
	}
	return make_pair(true, "");
}

