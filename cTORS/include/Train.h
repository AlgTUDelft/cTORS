#pragma once
#include <string>
#include <list>
#include <vector>
#include <nlohmann/json.hpp>
using namespace std;
using json = nlohmann::json;

struct TrainUnitType {
	static map<string,TrainUnitType*> types;
	string displayName;		// The name of the train unit type
	int carriages;			// The number of carriages
	double length;			// The length of this train unit, in meters
	int combineDuration;	// The time it takes to combine this type of train unit with another
	int splitDuration;		// The time it takes to split this type of train unit
	int backNormTime;		// the back norm time
	int backAdditionTime;	// The back addition time
	int setbackTime;		// The time needed to setback this type of train unit, calculated as #carriages * backAdditionTime
	int travelSpeed;		// The travel speed of the train
	int startUpTime;		// The time it takes for this type of train unit to start up / shut down
	string typePrefix;		// The prefix for this type of train unit, for example "SLT" or "VIRM"
	bool needsLoco;			// Whether or not this type of train unit needs a locomotive to move
	bool isLoco;			// Whether or not this type of train unit is a locomotive
	bool needsElectricity;	// Whether or not this type of train unit can only drive over electrified tracks

	TrainUnitType() = default;
	TrainUnitType(const string& displayName, int carriages, double length, int combineDuration, int splitDuration, int backNormTime,
		int backAdditionTime, int travelSpeed, int startUpTime, const string& typePrefix, bool needsLoco, bool isLoco, bool needsElectricity) :
		displayName(displayName), carriages(carriages), length(length), combineDuration(combineDuration), splitDuration(splitDuration),
		backNormTime(backNormTime), backAdditionTime(backAdditionTime), setbackTime(carriages*backAdditionTime), travelSpeed(travelSpeed),
		startUpTime(startUpTime), typePrefix(typePrefix), needsLoco(needsLoco), isLoco(isLoco), needsElectricity(needsElectricity) {}
	string toString() { return displayName; }
	bool operator==(const TrainUnitType& t) const { return (displayName == t.displayName); }
	bool operator!=(const TrainUnitType& t) const { return !(*this == t); }

	
};

struct Task {
	string taskType;
	int priority;
	int duration;
	list<string> skills;
	const string &toString() const { return taskType; }
	
	Task() = default;
	Task(const string& taskType, int priority, int duration, list<string> skills) :
		taskType(taskType), priority(priority), duration(duration), skills(skills) {}
	bool operator==(const Task& t) const { return (taskType == t.taskType && priority == t.priority && duration == t.duration); }
	bool operator!=(const Task& t) const { return !(*this == t); }
};

class Train {
private:
	int id;
	TrainUnitType *type;
	bool forcedMatch;
public:
	Train() = default;
	Train(int id, TrainUnitType *type);
	Train(const Train &train);
	~Train();
	void fromJSON(const json& j);
	inline TrainUnitType* GetType() const { return type; }
	bool operator==(const Train& train) const;
	bool operator!=(const Train& t) const { return !(*this == t); }
	const string toString() const;
	inline int GetID() const { return id; }
	inline void SetID(int id) { this->id = id; }
};

inline void from_json(const json& j, Train& t) {
	t.fromJSON(j);
}

void from_json(const json& j, TrainUnitType& tt);

void from_json(const json& j, Task& t);

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