#pragma once
#include <string>
#include <vector>
#include "Track.h"
using namespace std;
using json = nlohmann::json;

struct TimeShift {
	int start;
	int end;
};

class Employee
{
private:
	string id;
	string name;
	string type;
	vector<string> skills;
	vector<TimeShift> shifts;
	vector<TimeShift> breakWindows;
	int breakDuration;
	Track* startLocation;
	Track* endLocation;
	bool canMoveTrains;

public:
	Employee() = default;
	Employee(const Employee& employee) = default;
	~Employee();

	void fromJSON(const json& j);

	inline string toString() {
		return "Employee-" + id + " " + name;
	}

	inline bool operator==(const Employee& e) const { return (id == e.id); }
	inline bool operator!=(const Employee& e) const { return !(*this == e); }

	inline void AssignTracks(Track* start, Track* end) {
		startLocation = start;
		endLocation = end;
	}
};

void from_json(const json& j, TimeShift& ts);
void from_json(const json& j, Employee& e);


