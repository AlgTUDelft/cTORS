#include "BusinessRules.h"

/*

Rule that verifies that leaving shunting units have their train units in the 
correct order when they leave the shunting yard.

*/

pair<bool, string> out_correct_order_rule::IsValid(const State* state, const Action* action) const {
	if (auto ea = dynamic_cast<const ExitAction*>(action)) {
		if(!ea->GetOutgoing()->GetShuntingUnit()->MatchesShuntingUnit(ea->GetShuntingUnit()))
			return make_pair(false, "Leaving shunting unit " + ea->GetShuntingUnit()->toString() + " did not match the train unit order of outgoing train " + ea->GetOutgoing()->GetShuntingUnit()->toString());
	}
	return make_pair(true, "");
}

