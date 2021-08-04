/** \file ShuntingUnit.h
 * This file describes the ShuntingUnit class
 */
#pragma once
#ifndef SHUNTING_UNIT_H
#define SHUNTING_UNIT_H
#include "Train.h"
#include "Track.h"
using namespace std;
class Action;

/**
 * A ShuntingUnit describes connected Train%s that are shunted as one unit
 */
class ShuntingUnit {
private:
	int id;
	vector<Train> trains;
	double length;
	bool needsElectricity;
	string trainString;
	vector<int> trainIDs;
	void UpdateValues();
public:
	ShuntingUnit() = delete;
	/** Construct a ShuntingUnit with the given id and the given Train%s */
	ShuntingUnit(int id, const vector<Train>& trains) : id(id), trains(trains) { UpdateValues(); }
	/** Construct a ShuntingUnit from the given protobuf object */
	ShuntingUnit(const PBTrainGoal& pb_tg);
	/** The default copy constructor */
	ShuntingUnit(const ShuntingUnit& su) = default;

	/** Get a string representation of the ShuntingUnit */	
	inline const string toString() const {
		return "ShuntingUnit-" + to_string(id);
	}

	/** Get a string representation of the ShuntingUnit's Train%s */
	inline const string& GetTrainString() const { return trainString; }
	/** Get all the ids of the ShuntingUnit's Train%s */
	inline const vector<int>& GetTrainIDs() const { return trainIDs; };

	/** @return true iff the two ShuntingUnit%s have the same id */
	inline bool operator==(const ShuntingUnit& su) const { return (id == su.id); }
	/** @return true iff the two ShuntingUnit%s do not have the same id */
	inline bool operator!=(const ShuntingUnit& su) const { return !(*this == su); }

	/** Get the ID of the ShuntingUnit */
	inline int GetID() const {return id; }
	/** Get the total length of the ShuntingUnit */
	inline double GetLength() const { return length; }
	/** Returns true iff any of this ShuntingUnit's Train%s needs electricity */
	inline bool NeedsElectricity() const { return needsElectricity; }
	/** Get the number of Train%s in this ShuntingUnit */
	inline size_t GetNumberOfTrains() const { return trains.size(); }
	/** Get the Train%s in this ShuntingUnit */
	inline const vector<Train>& GetTrains() const { return trains; }
	/** Checks if this ShuntingUnit has a Train with the given id */
	bool HasTrainByID(int id) const;
	/** Checks if this ShuntingUnit has a Train with any of the given ids */
	bool HasAnyTrainByIDs(vector<int> ids) const;
	/** Get the Setback time for this ShuntingUnit, based on the current front-train and the given parameters */
	int GetSetbackTime(const Train* const frontTrain, bool normTime, bool walkTime, int setbackTime) const;
	/** Get the Setback time for this ShuntingUnit, based on the current front-train and the given parameters */
	inline int GetSetbackTime(const Train* const frontTrain, bool normTime, bool walkTime) const { return GetSetbackTime(frontTrain, normTime, walkTime, 0); }
	/** Get the start-up time for this ShuntingUnit, based on the current front-train */
	inline int GetStartUpTime(const Train* const frontTrain) const { return frontTrain->GetType()->startUpTime; }
	/** Set the Train%s of this ShuntingUnit */
	inline void SetTrains(vector<Train>& trains) {
		this->trains = trains;
		UpdateValues();
	}
	
	/**
	 * Checks if two ShuntingUnit%s match (based on id or type)
	 * @return true if the two ShuntingUnits match. They match iff
	 * 1. All Trains have the same ID.
	 * 2. OR the ID of the Trains are -1 and the types of the Train match.
	 * 
	 * The matching does consider order, but does not consider direction, thus
	 * 1. 1-2-3 and 3-2-1 match.
	 * 2. 1-3-2 and 1-2-3 do not match.
	 */
	bool MatchesShuntingUnit(const ShuntingUnit* su) const ;
	
	/**
	 * Checks if this ShuntingUnit matches a list of given train ids and train types.
	 * @return true if this ShuntingUnit and ShuntingUnit described by the vector of ids and types match.
	 * They match iff
	 * 1. All Trains have the same ID.
	 * 2. OR the ID of the Trains are -1 and the types of the Train match.
	 * 
	 * The matching does consider order, but does not consider direction, thus
	 * 1. 1-2-3 and 3-2-1 match.
	 * 2. 1-3-2 and 1-2-3 do not match.
	 */
	bool MatchesTrainIDs(const vector<int>& ids, const vector<const TrainUnitType*>& types) const;
	
	/** Get a Train reference of a Train in this ShuntingUnit by its id */
	const Train* GetTrainByID(int id) const;
	/** Get the index of a Train in this ShuntingUnit by its id */
	int GetTrainIndexByID(int id) const;
};

//!\cond NO_DOC
struct ShuntingUnitHash {
	std::size_t operator()(const ShuntingUnit* const & k) const {
		return std::hash<int>()(k->GetID());
	}
};

struct ShuntingUnitEquals {
	bool operator()(const ShuntingUnit* const & lhs, const ShuntingUnit* const & rhs) const {
		return lhs->GetID() == rhs->GetID();
	}
};

STREAM_OPERATOR(ShuntingUnit);

//!\endcond

#endif