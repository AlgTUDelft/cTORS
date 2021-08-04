#include "BusinessRules.h"

/**
 * Validates an ArriveAction, MoveAction, WaitAction and EndMoveActin for the given state. 
 * The ArriveAction is invalid iff
 * 1. The total length of the ShuntingUnits on the destination track including this one would exceed the Track's length.
 * 
 * The MoveAction is invalid iff
 * 1. The destination already contains ShuntingUnits and the addition of this ShuntingUnit would exceed the Track's length.
 * (in case of step-by-step moving, if the destination Track does not contain ShuntingUnits, but is too short to hold this ShuntingUnit,
 * the ShuntingUnit can still travel to this track, but cannot park on it.)
 * 2. OR (if not step-by-step moving, which means the Track is a destination Track), see the conditions for WaitAction or EndMoveAction
 * 
 * The WaitAction or EndMoveAction is invalid iff
 * 1. The total length of the ShuntingUnits on the current track, including this one, would exceed the Track's length.
 * @return A pair describing 1) whether the action is valid, and 2) if not, why
 */
pair<bool, string> length_track_rule::IsValid(const State* state, const Action* action) const {
	const Track* track;
	const ShuntingUnit* su = action->GetShuntingUnit();
	bool move = false;
	double length = su->GetLength();
	if (auto aa = dynamic_cast<const ArriveAction*>(action)) {
		track = aa->GetDestinationTrack();
	} else if (auto ma = dynamic_cast<const MoveAction*>(action)) {
		track = ma->GetDestinationTrack();
		move = ma->IsStepMove();
	} else if (state->IsMoving(su) && (dynamic_cast<const WaitAction*>(action) || dynamic_cast<const EndMoveAction*>(action))) {
		track = state->GetPosition(su);
		length = 0; // Do not count this ShuntingUnit's length, as it is already part of the current set of ShuntingUnits
	} else {
		return make_pair(true, "");
	}
	auto& occ = state->GetOccupations(track);
	for (auto& u : occ) {
		length += u->GetLength();
	}
	if (length > track->length) {
		if(!move || occ.size() > 0)
			return make_pair(false, "Adding ShuntingUnit-"+su->toString() + " to Track " + track->toString() + " exceeds the maximum length (" + to_string(length) + " > " +to_string(track->length) + ")");
	}
	return make_pair(true, "");
}

