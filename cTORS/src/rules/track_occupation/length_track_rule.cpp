#include "BusinessRules.h"

/*

Rule that verifies that shunting units on a single track do not take up more 
space than available on that track.

*/

pair<bool, string> length_track_rule::IsValid(const State* state, const Action* action) const {
	const Track* track;
	const ShuntingUnit* su = action->GetShuntingUnit();
	bool move = false;
	if (auto aa = dynamic_cast<const ArriveAction*>(action)) {
		track = aa->GetDestinationTrack();
	} else if (auto ma = dynamic_cast<const MoveAction*>(action)) {
		track = ma->GetDestinationTrack();
		move = true;
	} else if (state->IsMoving(su) && (dynamic_cast<const WaitAction*>(action) || dynamic_cast<const EndMoveAction*>(action))) {
		track = state->GetPosition(su);
	} else {
		return make_pair(true, "");
	}
	auto& occ = state->GetOccupations(track);
	double length = su->GetLength();
	for (auto& u : occ) {
		length += u->GetLength();
	}
	if (length > track->length) {
		if(!move || occ.size() > 0)
			return make_pair(false, "Adding ShuntingUnit-"+su->toString() + " to Track " + track->toString() + " exceeds the maximum length (" + to_string(length) + " > " +to_string(track->length) + ")");
	}
	return make_pair(true, "");
}

