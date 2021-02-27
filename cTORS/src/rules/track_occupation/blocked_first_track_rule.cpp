#include "BusinessRules.h"

/*

Rule that verifies that shunting units, upon starting a movement,
are not blocked on exit by other shunting units on their current track.

*/

pair<bool, string> blocked_first_track_rule::IsValid(const State* state, const Action* action) const {
	if(instanceof<ArriveAction>(action)) return make_pair(true, "");
	auto su = action->GetShuntingUnit();
	const Track* start = state->GetPosition(su);
	auto& occ = state->GetOccupations(start);
	if (instanceof<MoveAction>(action) || instanceof<ExitAction>(action)) {
		const Track* next;
		if (auto ma = dynamic_cast<const MoveAction*>(action)) {
			next = ma->GetTracks()[1];
		} else if (auto ea = dynamic_cast<const ExitAction*>(action)) {
			next = ea->GetOutgoing()->GetSideTrack();
		}
		if (occ.size() > 1) {
			if (start->IsASide(next)) {
				if (occ.front() != su)
					return make_pair(false, "ShuntingUnit-" + su->toString() + " cannot leave Track " + start->toString() + " at the A-side to Track " + next->toString() + ": blocked.");
			}
			else {
				if (occ.back() != su)
					return make_pair(false, "ShuntingUnit-" + su->toString() + " cannot leave Track " + start->toString() + " at the B-side to Track " + next->toString() + ": blocked.");
			}
		}
	} else if (auto bma = dynamic_cast<const BeginMoveAction*>(action)) {
		if (occ.size() > 2) {
			if(occ.front() != su && occ.back() != su)
				return make_pair(false, "ShuntingUnit-" + su->toString() + " cannot leave Track " + start->toString() + ". Both sides blocked.");
		} // TODO add check if one side is a bumper
	}
	return make_pair(true, "");
}

