#include "BusinessRules.h"

/**
 * Validates an ExitAction for the given state. 
 * The ExitAction is invalid if
 * 1. The ShuntingUnit's current track is not the same as the Outgoing's parking track
 * @return A pair describing 1) whether the action is valid, and 2) if not, why
 */
pair<bool, string> out_correct_track_rule::IsValid(const State* state, const Action* action) const {
	if (auto ea = dynamic_cast<const ExitAction*>(action)) {
		if (state->GetPosition(ea->GetShuntingUnit()) != ea->GetOutgoing()->GetParkingTrack())
			return make_pair(false, "Shunting unit " + ea->GetOutgoing()->GetShuntingUnit()->toString() + " should leave from track " + ea->GetOutgoing()->GetParkingTrack()->toString());
	}
	return make_pair(true, "");
}

