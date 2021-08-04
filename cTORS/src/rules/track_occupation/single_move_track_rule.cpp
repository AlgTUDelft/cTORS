#include "BusinessRules.h"

/**
 * Validates an Action for the given state. 
 * The Action is invalid iff
 * 1. The Action uses a Track that is reserved in the current State.
 * @return A pair describing 1) whether the action is valid, and 2) if not, why
 * 
 * In the current version this business rule is no different from blocked_track_rule. 
 */
pair<bool, string> single_move_track_rule::IsValid(const State* state, const Action* action) const {
    auto reserves = action->GetReservedTracks();
    for (auto t : reserves) {
        if (state->IsReserved(t))
            return make_pair(false, "Track " + t->toString() + " is reserved");
    }
    return make_pair(true, "");
}

