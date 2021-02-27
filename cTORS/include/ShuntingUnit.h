#pragma once
#include <vector>
#include <nlohmann/json.hpp>
#include "Train.h"
#include "Track.h"
#include "Utils.h"
using namespace std;
using json = nlohmann::json;

class Action;

class ShuntingUnit {
private:
	int id;
	vector<const Train*> trains;
	double length;
	bool needsElectricity;
	void UpdateValues();
public:
	ShuntingUnit() = default;
	ShuntingUnit(int id, vector<const Train*> trains);
	ShuntingUnit(const ShuntingUnit& su);
	~ShuntingUnit();
	void fromJSON(const json& j);

	inline const string toString() const {
		return "ShuntingUnit-" + to_string(id);
	}
	const string GetTrainString() const;

	inline bool operator==(const ShuntingUnit& su) const { return (id == su.id); }
	inline bool operator!=(const ShuntingUnit& su) const { return !(*this == su); }

	inline int GetID() const {return id; }
	inline double GetLength() const { return length; }
	inline bool NeedsElectricity() const { return needsElectricity; }
	inline size_t GetNumberOfTrains() const { return trains.size(); }
	inline const vector<const Train*>& GetTrains() const { return trains; }
	int GetSetbackTime(const Train* const frontTrain, bool normTime, bool walkTime, int setbackTime) const;
	inline int GetSetbackTime(const Train* const frontTrain, bool normTime, bool walkTime) const { return GetSetbackTime(frontTrain, normTime, walkTime, 0); }
	inline int GetStartUpTime(const Train* const frontTrain) const { return frontTrain->GetType()->startUpTime; }
	inline void SetTrains(vector<const Train*> trains) {
		this->trains = trains;
		UpdateValues();
	}
	bool MatchesShuntingUnit(const ShuntingUnit* su) const ;
};

inline void from_json(const json& j, ShuntingUnit& su) {
	su.fromJSON(j);
}

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
