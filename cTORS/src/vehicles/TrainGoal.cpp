#include "TrainGoals.h"

TrainGoal::TrainGoal(const TrainGoal& traingoal) :
	id(traingoal.id), parkingTrack(traingoal.parkingTrack), sideTrack(traingoal.sideTrack),
	time(traingoal.time), standingIndex(traingoal.standingIndex), isInstanding(traingoal.isInstanding)
{
	shuntingUnit = new ShuntingUnit(*traingoal.shuntingUnit);
}

TrainGoal::~TrainGoal() {
	delete shuntingUnit;
}

void TrainGoal::fromJSON(const json& j) {
	id = stoi(j.at("id").get<string>());
	time = stoi(j.at("time").get<string>());
	ShuntingUnit* su = new ShuntingUnit();
	j.get_to(*su);
	shuntingUnit = su;
	if (j.find("standingIndex") != j.end())
		standingIndex = stoi(j.at("standingIndex").get<string>());
	int i = 0;
	for (auto jit : j.at("members")) {
		auto tu = shuntingUnit->GetTrains()[i++];
		auto& trainTasks = tasks[tu];
		jit.at("tasks").get_to(trainTasks);
	}
}

void TrainGoal::assignTracks(const Track* park, const Track* side) {
	parkingTrack = park;
	sideTrack = side;
}