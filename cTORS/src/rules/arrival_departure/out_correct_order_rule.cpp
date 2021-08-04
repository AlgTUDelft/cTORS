#include "BusinessRules.h"

/**
 * Validates an ExitAction for the given state. 
 * The ExitAction is invalid if
 * 1. The assigned ShuntingUnit does not match the demans of the ShuntingUnit in the Outgoing attribute.
 * 
 * See ShuntingUnit::MatchesShuntingUnit(const ShuntingUnit*) const
 * @return A pair describing 1) whether the action is valid, and 2) if not, why
 */
pair<bool, string> out_correct_order_rule::IsValid(const State* state, const Action* action) const {
	if (auto ea = dynamic_cast<const ExitAction*>(action)) {
		if(!ea->GetOutgoing()->GetShuntingUnit()->MatchesShuntingUnit(ea->GetShuntingUnit()))
			return make_pair(false, "Leaving shunting unit " + ea->GetShuntingUnit()->toString() + " did not match the train unit order of outgoing train " + ea->GetOutgoing()->GetShuntingUnit()->toString());
	}
	return make_pair(true, "");
}

