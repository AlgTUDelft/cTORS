#include "TrainGoals.h"

const Train* GetTrainById(const vector<Train>& trains, const string& id) {
	int iid = id == "****" ? -1 : stoi(id);
	for(auto& train: trains) {
		if(train.GetID() == iid) return &train;
	}
	return nullptr;
}

unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals> ConvertPBTrainTasks(const ShuntingUnit* su, const PBTrainGoal& pb_inc) {
	unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals> map;
	for(auto& train: pb_inc.members()) {
		for(auto& t: train.tasks()) {
			auto tu = GetTrainById(su->GetTrains(), train.id());
			map[tu].push_back({t});
		}
	}
	return map;
}

Incoming::Incoming(const PBTrainGoal& pb_inc, bool isInstanding) : Incoming(stoi(pb_inc.id()), new ShuntingUnit(pb_inc),
 	pb_inc.time(), isInstanding, pb_inc.standingindex()) {
		 tasks = ConvertPBTrainTasks(shuntingUnit, pb_inc);
	 }

Outgoing::Outgoing(const PBTrainGoal& pb_out, bool isInstanding) : Outgoing(stoi(pb_out.id()), new ShuntingUnit(pb_out),
 	pb_out.time(), isInstanding, pb_out.standingindex()) {}

TrainGoal::TrainGoal(const TrainGoal& traingoal) :
	id(traingoal.id), parkingTrack(traingoal.parkingTrack), sideTrack(traingoal.sideTrack),
	time(traingoal.time), standingIndex(traingoal.standingIndex), isInstanding(traingoal.isInstanding)
{
	shuntingUnit = new ShuntingUnit(*traingoal.shuntingUnit);
	for(auto& [train, tasks]: traingoal.tasks) {
		auto newTrain = shuntingUnit->GetTrainByID(train->GetID());
		this->tasks[newTrain] = tasks;
	}
}

TrainGoal::~TrainGoal() {
	delete shuntingUnit;
}

void TrainGoal::assignTracks(const Track* park, const Track* side) {
	parkingTrack = park;
	sideTrack = side;
}

void TrainGoal::Serialize(PBTrainGoal* pb_tg) const {
	pb_tg->set_parkingtrackpart(stoi(parkingTrack->GetID()));
	pb_tg->set_sidetrackpart(stoi(sideTrack->GetID()));
	pb_tg->set_time(time);
	pb_tg->set_id(to_string(id));
	map<const Train*, PBTrainUnit*> trainMap;
	for(auto& t: shuntingUnit->GetTrains()) {
		auto pb_t = pb_tg->add_members();
		t.Serialize(pb_t);
		trainMap[&t] = pb_t;
	}
	pb_tg->set_candepartfromanytrack(false);
	pb_tg->set_standingindex(standingIndex);
	for(auto& [train, tasks]: this->tasks) {
		for(auto& task: tasks) {
			auto pb_task = trainMap.at(train)->add_tasks();
			task.Serialize(pb_task);
		}
	}
}

bool TrainGoal::operator==(const TrainGoal& tg) const {
	if(tg.id != this->id) return false;
	return((instanceof<Incoming>(this) && instanceof<Incoming>(&tg)) ||
		(instanceof<Outgoing>(this) && instanceof<Outgoing>(&tg)));
}
