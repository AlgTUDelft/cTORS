#include "BusinessRules.h"

/*

Rule that verifies that at most one shunting unit can use a piece of track at a given time.

*/

pair<bool, string> single_move_track_rule::IsValid(const State* state, const Action* action) const {
    auto reserves = action->GetReservedTracks();
    for (auto t : reserves) {
        if (state->IsReserved(t))
            return make_pair(false, "Track " + t->toString() + " is reserved");
    }
    return make_pair(true, "");
}

