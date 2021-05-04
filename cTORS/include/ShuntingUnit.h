#pragma once
#ifndef SHUNTING_UNIT_H
#define SHUNTING_UNIT_H
#include <vector>
#include "Train.h"
#include "Track.h"
#include "Utils.h"
using namespace std;
class Action;

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
	ShuntingUnit(int id, const vector<Train>& trains) : id(id), trains(trains) { UpdateValues(); }
	ShuntingUnit(const PBTrainGoal& pb_tg);
	ShuntingUnit(const ShuntingUnit& su) = default;
	~ShuntingUnit() = default;
	
	inline const string toString() const {
		return "ShuntingUnit-" + to_string(id);
	}
	inline const string& GetTrainString() const { return trainString; }
	inline const vector<int>& GetTrainIDs() const { return trainIDs; };

	inline bool operator==(const ShuntingUnit& su) const { return (id == su.id); }
	inline bool operator!=(const ShuntingUnit& su) const { return !(*this == su); }

	inline int GetID() const {return id; }
	inline double GetLength() const { return length; }
	inline bool NeedsElectricity() const { return needsElectricity; }
	inline size_t GetNumberOfTrains() const { return trains.size(); }
	inline const vector<Train>& GetTrains() const { return trains; }
	int GetSetbackTime(const Train* const frontTrain, bool normTime, bool walkTime, int setbackTime) const;
	inline int GetSetbackTime(const Train* const frontTrain, bool normTime, bool walkTime) const { return GetSetbackTime(frontTrain, normTime, walkTime, 0); }
	inline int GetStartUpTime(const Train* const frontTrain) const { return frontTrain->GetType()->startUpTime; }
	inline void SetTrains(vector<Train>& trains) {
		this->trains = trains;
		UpdateValues();
	}
	bool MatchesShuntingUnit(const ShuntingUnit* su) const ;
	bool MatchesTrainIDs(const vector<int>& ids, const vector<const TrainUnitType*>& types) const;
	const Train* GetTrainByID(int id) const;
	int GetTrainIndexByID(int id) const;
};

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

#endif