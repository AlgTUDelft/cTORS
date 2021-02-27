#include "TrainGoals.h"

void Outgoing::fromJSON(const json& j) {
	TrainGoal::fromJSON(j);
}

void from_json(const json& j, Outgoing& out) {
	out.fromJSON(j);
}