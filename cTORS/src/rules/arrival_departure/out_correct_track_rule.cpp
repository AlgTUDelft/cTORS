#include "BusinessRules.h"

/*

Rule that verifies that leaving shunting units leave over the correct tracks.

*/

pair<bool, string> out_correct_track_rule::IsValid(const State* state, const Action* action) const {
	if (auto ea = dynamic_cast<const ExitAction*>(action)) {
		if (state->GetPosition(ea->GetShuntingUnit()) != ea->GetOutgoing()->GetParkingTrack())
			return make_pair(false, "Shunting unit " + ea->GetOutgoing()->GetShuntingUnit()->toString() + " should leave from track " + ea->GetOutgoing()->GetParkingTrack()->toString());
	}
	return make_pair(true, "");
}

