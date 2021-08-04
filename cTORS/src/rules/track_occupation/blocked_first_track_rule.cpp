#include "BusinessRules.h"

/**
 * Validates a MoveAction, BeginMoveAction, or ExitAction for the given state. 
 * The MoveAction or ExitAction is invalid iff
 * 1. The ShuntingUnit is blocked by another ShuntingUnit and cannot leave the current track towards the specified direction
 * 
 * The BeginMoveAction is invalid iff
 * 1. The ShuntingUnit is blocked on both sides of the track by other ShuntingUnits
 * 2. The ShuntingUnit is blocked by another ShuntingUnit on one side, and the other side of the track leads to a Bumper.
 * @return A pair describing 1) whether the action is valid, and 2) if not, why
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
		if (occ.size() > 2 && occ.front() != su && occ.back() != su) { //More than 2 SU's and not standing at the end of the Track
			return make_pair(false, "ShuntingUnit-" + su->toString() + " cannot leave Track " + start->toString() + ". Both sides blocked.");
		} else if (occ.size() >= 2) { //Blocked from at least one side, but standing at the end of the Track
			auto& sides = (occ.front() == su ? start->GetASideTracks() : start->GetBSideTracks());
			assert(sides.size() == 1); //Current location is a RailRoad track, thus contains only one aSide and one bSide.
			if(sides.at(0)->GetType() == TrackPartType::Bumper) {
				return make_pair(false, "ShuntingUnit-" + su->toString() + " cannot leave Track " + start->toString() + 
					". One side is blocked by another train, the other side is a Bumper.");
			}
		}
	}
	return make_pair(true, "");
}

