#include "BusinessRules.h"

/*

Rule that verifies that shunting units that are arriving, arrive at the 
correct time. Note: shunting units will never arrive too early, so this rule 
only checks if a shunting unit arrives too late.

*/

pair<bool, string> in_correct_time_rule::IsValid(const State* state, const Action* action) const {
	for (auto i : state->GetIncomingTrains()) {
		if (i->GetTime() < state->GetTime())
			return make_pair(false, "Shunting unit " + i->GetShuntingUnit()->toString() + " should have already arrived");
	}
	return make_pair(true, "");
}

