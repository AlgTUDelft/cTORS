#pragma once
#ifndef TRAIN_H
#define TRAIN_H
#include <string>
#include <list>
#include <vector>
#include "Proto.h"
#include "Utils.h"
using namespace std;

struct TrainUnitType {
	static map<string,TrainUnitType*> types;
	const string displayName;		// The name of the train unit type
	const int carriages;			// The number of carriages
	const double length;			// The length of this train unit, in meters
	const int combineDuration;	// The time it takes to combine this type of train unit with another
	const int splitDuration;		// The time it takes to split this type of train unit
	const int backNormTime;		// the back norm time
	const int backAdditionTime;	// The back addition time
	const int setbackTime;		// The time needed to setback this type of train unit, calculated as #carriages * backAdditionTime
	const int travelSpeed;		// The travel speed of the train
	const int startUpTime;		// The time it takes for this type of train unit to start up / shut down
	const string typePrefix;		// The prefix for this type of train unit, for example "SLT" or "VIRM"
	const bool needsLoco;			// Whether or not this type of train unit needs a locomotive to move
	const bool isLoco;			// Whether or not this type of train unit is a locomotive
	const bool needsElectricity;	// Whether or not this type of train unit can only drive over electrified tracks

	TrainUnitType() = delete;
	TrainUnitType(const string& displayName, int carriages, double length, int combineDuration, int splitDuration, int backNormTime,
		int backAdditionTime, int travelSpeed, int startUpTime, const string& typePrefix, bool needsLoco, bool isLoco, bool needsElectricity) :
		displayName(displayName), carriages(carriages), length(length), combineDuration(combineDuration), splitDuration(splitDuration),
		backNormTime(backNormTime), backAdditionTime(backAdditionTime), setbackTime(carriages*backAdditionTime), travelSpeed(travelSpeed),
		startUpTime(startUpTime), typePrefix(typePrefix), needsLoco(needsLoco), isLoco(isLoco), needsElectricity(needsElectricity) {}
	TrainUnitType(const PBTrainUnitType& pb_tt) : TrainUnitType(pb_tt.displayname(), pb_tt.carriages(), pb_tt.length(), pb_tt.combineduration(), 
		pb_tt.splitduration(), pb_tt.backnormtime(), pb_tt.backadditiontime(), pb_tt.travelspeed(), pb_tt.startuptime(), pb_tt.typeprefix(), 
		pb_tt.needsloco(), pb_tt.isloco(), pb_tt.needselectricity()) {}
	const string toString() const { return displayName; }
	bool operator==(const TrainUnitType& t) const { return (displayName == t.displayName); }
	bool operator!=(const TrainUnitType& t) const { return !(*this == t); }
	void Serialize(PBTrainUnitType* pb_tt) const;
};

inline string ConvertPBTaskType(const PBTaskType& pb_task_type) {
	return pb_task_type.other();
}

inline vector<string> ConvertPBTaskTypes(const PBList<PBTaskType>& pb_task_types) {
	vector<string> out(pb_task_types.size());
	for(int i=0; i<pb_task_types.size(); i++)
		out[i] = ConvertPBTaskType(pb_task_types[i]);
	return out;
}

struct Task {
	string taskType;
	int priority;
	int duration;
	list<string> skills;
		
	Task() = delete;
	Task(const string& taskType, int priority, int duration, list<string> skills) :
		taskType(taskType), priority(priority), duration(duration), skills(skills) {}
	Task(const PBTask& pb_task) :
		Task(ConvertPBTaskType(pb_task.type()), pb_task.priority(), pb_task.duration(), PBToStringList(pb_task.requiredskills())) {}
	bool operator==(const Task& t) const { return (taskType == t.taskType && priority == t.priority && duration == t.duration); }
	bool operator!=(const Task& t) const { return !(*this == t); }
	const string &toString() const { return taskType; }
	void Serialize(PBTask* pb_task) const;
};

class Train {
private:
	int id;
	TrainUnitType *type;
	bool forcedMatch;
public:
	Train() = delete;
	Train(int id, TrainUnitType *type) : id(id), type(type), forcedMatch(false) {}
	Train(const PBTrainUnit& pb_train);
	Train(const Train &train) = default;
	~Train() = default;
	inline const TrainUnitType* GetType() const { return type; }
	bool operator==(const Train& train) const;
	bool operator!=(const Train& t) const { return !(*this == t); }
	const string toString() const;
	inline int GetID() const { return id; }
	void Serialize(PBTrainUnit* pb_t) const;
};

struct TrainHash {
	std::size_t operator()(const Train* const & k) const {
		return std::hash<int>()(k->GetID());
	}
};

struct TrainEquals {
	bool operator()(const Train* const & lhs, const Train* const & rhs) const {
		return lhs->GetID() == rhs->GetID();
	}
};

#endif