#include "BusinessRules.h"

/*

Rule that verifies that shunting units which need electricity park only on electrified tracks.

*/

pair<bool, string> electric_track_rule::IsValid(const State* state, const Action* action) const {
	auto su = action->GetShuntingUnit();
	if (!su->NeedsElectricity()) return make_pair(true, "");
	const Track* destination = nullptr;
	if (auto ma = dynamic_cast<const MoveAction*>(action)) {
		destination = ma->GetDestinationTrack();
	} else if (auto aa = dynamic_cast<const ArriveAction*>(action)) {
		destination = aa->GetDestinationTrack();
	} else if (auto ea = dynamic_cast<const ExitAction*>(action)) {
		destination = ea->GetDestinationTrack();
	} else {
		return make_pair(true, "");
	}
	if (!destination->isElectrified)
		return make_pair(false, "Destination track " + destination->toString() + " is not electrified.");
	return make_pair(true, "");
}

