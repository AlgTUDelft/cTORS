#pragma once
#ifndef TRAIN_GOALS_H
#define TRAIN_GOALS_H
#include <string>
#include "Track.h"
#include "ShuntingUnit.h"
#include "Utils.h"
using namespace std;

class TrainGoal {
protected:
	const int id;
	const Track* parkingTrack;
	const Track* sideTrack;
	const ShuntingUnit* shuntingUnit;
	const int time;
	const int standingIndex;
	const bool isInstanding;
	unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals> tasks;
public:
	TrainGoal() = delete;
	TrainGoal(int id, const ShuntingUnit* su, const Track* parkingTrack, const Track* sideTrack, int time, bool isInstanding, int standingIndex,
			unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals> tasks)
			: id(id), shuntingUnit(su), parkingTrack(parkingTrack), sideTrack(sideTrack), time(time),
			isInstanding(isInstanding), standingIndex(standingIndex), tasks(tasks) {};
	TrainGoal(int id, const ShuntingUnit* su, int time, bool isInstanding, int standingIndex,
			unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals> tasks)
			: TrainGoal(id, su, nullptr, nullptr, time, isInstanding, standingIndex, tasks) {};
	TrainGoal(int id, const ShuntingUnit* su, int time, bool isInstanding, int standingIndex)
			: TrainGoal(id, su, nullptr, nullptr, time, isInstanding, standingIndex, unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals> {}) {};
	TrainGoal(const TrainGoal& traingoal);
	~TrainGoal();
	inline int GetID() const { return id; }
	void assignTracks(const Track* park, const Track* side);
	inline bool IsInstanding() const { return isInstanding; }
	inline int GetTime() const { return time; }
	inline int GetStandingIndex() const { return standingIndex; }
	inline const ShuntingUnit* GetShuntingUnit() const { return shuntingUnit; }
	inline const Track* GetSideTrack() const { return sideTrack; }
	inline const Track* GetParkingTrack() const { return parkingTrack; }
	const inline unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals>& GetTasks() const { return tasks; }
	void Serialize(PBTrainGoal* pb_inc) const;
	virtual string toString() const = 0;
	bool operator==(const TrainGoal& tg) const;
	bool operator!=(const TrainGoal& tg) const { return !(*this == tg); }
};

class Incoming : public TrainGoal {
public:
	Incoming() = delete;
	Incoming(int id, const ShuntingUnit* su, const Track* parkingTrack, const Track* sideTrack, int time, bool isInstanding, int standingIndex,
			unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals> tasks) : 
		TrainGoal(id, su, parkingTrack, sideTrack, time, isInstanding, standingIndex, tasks) {}
	Incoming(int id, const ShuntingUnit* su, int time, bool isInstanding, int standingIndex) :
		Incoming(id, su, nullptr, nullptr, time, isInstanding, standingIndex, unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals> {}) {}
	Incoming(const PBTrainGoal& pb_inc, bool isInstanding);
	Incoming(const Incoming& incoming) : TrainGoal(incoming) {}
	~Incoming() = default;
	inline string toString() const override { return "Incoming " + GetShuntingUnit()->toString() + " at " + GetParkingTrack()->toString() + " at " + to_string(GetTime()); }
};

class Outgoing : public TrainGoal {
public:
	Outgoing() = delete;
	Outgoing(int id, const ShuntingUnit* su, const Track* parkingTrack, const Track* sideTrack, int time, bool isInstanding, int standingIndex) :
		TrainGoal(id, su, parkingTrack, sideTrack, time, isInstanding, standingIndex, unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals> {}) {}
	Outgoing(int id, const ShuntingUnit* su, int time, bool isInstanding, int standingIndex) :
		Outgoing(id, su, nullptr, nullptr, time, isInstanding, standingIndex) {}
	Outgoing(const PBTrainGoal& pb_out, bool isInstanding);
	Outgoing(const Outgoing& outgoing) : TrainGoal(outgoing) {}
	~Outgoing() = default;
	inline string toString() const override { return "Outgoing " + GetShuntingUnit()->toString() + " from " + GetParkingTrack()->toString() + " at " + to_string(GetTime()); }
};

#endif