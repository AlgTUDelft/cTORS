#include "TrainGoals.h"

void Incoming::fromJSON(const json& j) {
	TrainGoal::fromJSON(j);
}

void from_json(const json& j, Incoming& in) {
	in.fromJSON(j);
}