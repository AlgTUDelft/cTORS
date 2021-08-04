#include "BusinessRules.h"

/**
 * Validates an ExitAction for the given state. 
 * The ExitAction is invalid if
 * 1. The time of the action is not the same as the time as described by the Outgoing attribute
 * @return A pair describing 1) whether the action is valid, and 2) if not, why
 */
pair<bool, string> out_correct_time_rule::IsValid(const State* state, const Action* action) const {
	if (auto ea = dynamic_cast<const ExitAction*>(action)) {
		if(state->GetTime() != ea->GetOutgoing()->GetTime())
			return make_pair(false, "Shunting unit " + ea->GetOutgoing()->GetShuntingUnit()->toString() + " should leave at time " + to_string(ea->GetOutgoing()->GetTime()) );
	}
	return make_pair(true, "");
}

