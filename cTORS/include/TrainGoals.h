#pragma once
#include <string>
#include "Track.h"
#include "ShuntingUnit.h"
using namespace std;
using json = nlohmann::json;

class TrainGoal {
protected:
	int id;
	const Track* parkingTrack;
	const Track* sideTrack;
	const ShuntingUnit* shuntingUnit;
	int time;
	int standingIndex;
	bool isInstanding;
	unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals> tasks;
public:
	TrainGoal() = default;
	TrainGoal(int id, const ShuntingUnit* su, const Track* parkingTrack, const Track* sideTrack, int time, bool isInstanding, int standingIndex,
			unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals> tasks) :
		id(id), shuntingUnit(su), parkingTrack(parkingTrack), sideTrack(sideTrack), time(time), isInstanding(isInstanding), standingIndex(standingIndex), tasks(tasks) {};
	TrainGoal(const TrainGoal& traingoal);
	~TrainGoal();
	void fromJSON(const json& j);
	void assignTracks(const Track* park, const Track* side);
	inline void setInstanding(bool b) { isInstanding = b; }
	inline bool IsInstanding() const { return isInstanding; }
	inline int GetTime() const { return time; }
	inline int GetStandingIndex() const { return standingIndex; }
	inline const ShuntingUnit* GetShuntingUnit() const { return shuntingUnit; }
	inline const Track* GetSideTrack() const { return sideTrack; }
	inline const Track* GetParkingTrack() const { return parkingTrack; }
	const inline unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals>& GetTasks() const { return tasks; }
	virtual string toString() const = 0;
};

class Incoming : public TrainGoal {
public:
	Incoming() = default;
	Incoming(int id, const ShuntingUnit* su, const Track* parkingTrack, const Track* sideTrack, int time, bool isInstanding, int standingIndex,
			unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals> tasks) :
		TrainGoal(id, su, parkingTrack, sideTrack, time, isInstanding, standingIndex, tasks) {}
	Incoming(const Incoming& incoming) : TrainGoal(incoming) {}
	~Incoming() = default;
	void fromJSON(const json& j);
	inline string toString() const override { return "Incoming " + GetShuntingUnit()->toString() + " at " + GetParkingTrack()->toString() + " at " + to_string(GetTime()); }
};

class Outgoing : public TrainGoal {
public:
	Outgoing() = default;
	Outgoing(int id, const ShuntingUnit* su, const Track* parkingTrack, const Track* sideTrack, int time, bool isInstanding, int standingIndex) :
		TrainGoal(id, su, parkingTrack, sideTrack, time, isInstanding, standingIndex, unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals> {}) {}
	Outgoing(const Outgoing& outgoing) : TrainGoal(outgoing) {}
	~Outgoing() = default;
	void fromJSON(const json& j);
	inline string toString() const override { return "Outgoing " + GetShuntingUnit()->toString() + " from " + GetParkingTrack()->toString() + " at " + to_string(GetTime()); }
};

void from_json(const json& j, Incoming& in);
void from_json(const json& j, Outgoing& out);