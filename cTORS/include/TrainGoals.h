/** \file TrainGoals.h
 * Describes the Incoming and Outgoing class with their abstract base class TrainGoal
 */
#pragma once
#ifndef TRAIN_GOALS_H
#define TRAIN_GOALS_H
#include "ShuntingUnit.h"
using namespace std;

/**
 * The abstract base class for the Incoming and Outgoing classes
 */
class TrainGoal {
protected:
	const int id;						/**< The id of this TrainGoal */
	const Track* parkingTrack;			/**< The parking track for the ShuntingUnit */
	const Track* sideTrack;				/**< The side track for teh ShuntingUnit (Bumper track)
										 * If Incoming, the Track from which the ShuntingUnit arrives
	 									 * If Outgoing, the Track from which the ShuntingUnit departs
										 */
	const ShuntingUnit* shuntingUnit;	/**< The arriving/departing ShuntingUnit */
	const int time;						/**< The time of arriving/departing */
	const int standingIndex;			/**< If instanding, the position index on the Track of this ShuntingUnit */
	const bool isInstanding;			/**< True if this ShuntingUnit is standing on the shunting yard before the 
											Scenario starts (Incoming) or after it ends (Outgoing) */
	/** All Task%s for the Train%s in this ShuntingUnit */
	unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals> tasks;
public:
	TrainGoal() = delete;
	/** Construct this TrainGoal from the provided parameters */
	TrainGoal(int id, const ShuntingUnit* su, const Track* parkingTrack, const Track* sideTrack, int time, bool isInstanding, int standingIndex,
			unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals> tasks)
			: id(id), shuntingUnit(su), parkingTrack(parkingTrack), sideTrack(sideTrack), time(time),
			isInstanding(isInstanding), standingIndex(standingIndex), tasks(tasks) {};
	/** Construct this TrainGoal from the provided parameters */
	TrainGoal(int id, const ShuntingUnit* su, int time, bool isInstanding, int standingIndex,
			unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals> tasks)
			: TrainGoal(id, su, nullptr, nullptr, time, isInstanding, standingIndex, tasks) {};
	/** Construct this TrainGoal from the provided parameters */
	TrainGoal(int id, const ShuntingUnit* su, int time, bool isInstanding, int standingIndex)
			: TrainGoal(id, su, nullptr, nullptr, time, isInstanding, standingIndex, unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals> {}) {};
	/** Copy this TrainGoal */
	TrainGoal(const TrainGoal& traingoal);
	/** Destroy this TrainGoal */
	~TrainGoal();
	/** Get the id of this TrainGoal */
	inline int GetID() const { return id; }
	/** Assign the Track%s for this TrainGoal */
	void assignTracks(const Track* park, const Track* side);
	/** Return true iff this ShuntingUnit is instanding */
	inline bool IsInstanding() const { return isInstanding; }
	/** Return the time of arrival (Incoming) or departure (Outgoing) */
	inline int GetTime() const { return time; }
	/** If instanding, return the position index on the Track of this ShuntingUnit */
	inline int GetStandingIndex() const { return standingIndex; }
	/** Return the ShuntingUnit */
	inline const ShuntingUnit* GetShuntingUnit() const { return shuntingUnit; }
	/** Get the side Track for this TrainGoal.
	 * 
	 * If Incoming, the Track from which the ShuntingUnit arrives
	 * If Outgoing, the Track from which the ShuntingUnit departs
	 */
	inline const Track* GetSideTrack() const { return sideTrack; }
	/** Get the parking Track for this TrainGoal */
	inline const Track* GetParkingTrack() const { return parkingTrack; }
	/** Get all the tasks, defined per Train, for this ShuntingUnit (only Incoming) */
	const inline unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals>& GetTasks() const { return tasks; }
	/** Serialize this object to a protobuf object */
	void Serialize(PBTrainGoal* pb_inc) const;
	/** Get a string representation of this object */
	virtual string toString() const = 0;
	/** Return true iff both are the same type (Incoming vs Outgoing) and have the same id */
	bool operator==(const TrainGoal& tg) const;
	/** Return false iff both are the same type (Incoming vs Outgoing) or have the same id */
	bool operator!=(const TrainGoal& tg) const { return !(*this == tg); }
};

/**
 * Describes a future Incoming ShuntingUnit
 */
class Incoming : public TrainGoal {
public:
	Incoming() = delete;
	/** Construct an Incoming object from the given parameters */
	Incoming(int id, const ShuntingUnit* su, const Track* parkingTrack, const Track* sideTrack, int time, bool isInstanding, int standingIndex,
			unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals> tasks) : 
		TrainGoal(id, su, parkingTrack, sideTrack, time, isInstanding, standingIndex, tasks) {}
	/** Construct an Incoming object from the given parameters */
	Incoming(int id, const ShuntingUnit* su, int time, bool isInstanding, int standingIndex) :
		Incoming(id, su, nullptr, nullptr, time, isInstanding, standingIndex, unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals> {}) {}
	/** Construct an Incoming object from the given protobuf object */
	Incoming(const PBTrainGoal& pb_inc, bool isInstanding);
	/** Copy constructor */
	Incoming(const Incoming& incoming) : TrainGoal(incoming) {}
	/** Default destructor */
	~Incoming() = default;
	inline string toString() const override { return "Incoming " + GetShuntingUnit()->toString() + " at " + GetParkingTrack()->toString() + " at " + to_string(GetTime()); }
};

/**
 * Describes a future Outgoing ShuntingUnit
 * 
 * Note that the ids of the Train%s in the ShuntingUnit may be set to -1,
 * in case the precise ids of the outgoing trains do not matter.
 */
class Outgoing : public TrainGoal {
public:
	Outgoing() = delete;
	/** Construct an Outgoing object from the given parameters */
	Outgoing(int id, const ShuntingUnit* su, const Track* parkingTrack, const Track* sideTrack, int time, bool isInstanding, int standingIndex) :
		TrainGoal(id, su, parkingTrack, sideTrack, time, isInstanding, standingIndex, unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals> {}) {}
	/** Construct an Outgoing object from the given parameters */
	Outgoing(int id, const ShuntingUnit* su, int time, bool isInstanding, int standingIndex) :
		Outgoing(id, su, nullptr, nullptr, time, isInstanding, standingIndex) {}
	/** Construct an Outgoing object from the given protobuf object */
	Outgoing(const PBTrainGoal& pb_out, bool isInstanding);
	/** Copy constructor */
	Outgoing(const Outgoing& outgoing) : TrainGoal(outgoing) {}
	/** Default destructor */
	~Outgoing() = default;
	inline string toString() const override { return "Outgoing " + GetShuntingUnit()->toString() + " from " + GetParkingTrack()->toString() + " at " + to_string(GetTime()); }
};

#endif