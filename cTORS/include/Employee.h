#pragma once
#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <string>
#include <vector>
#include "Track.h"
#include "Utils.h"
using namespace std;

struct TimeShift {
	int start;
	int end;
};

class Employee
{
private:
	const string id;
	const string name;
	const string type;
	const vector<string> skills;
	const vector<TimeShift> shifts;
	const vector<TimeShift> breakWindows;
	const int breakDuration;
	Track* startLocation;
	Track* endLocation;
	const bool canMoveTrains;

public:
	Employee() = delete;
	Employee(const string& id, const string& name, const string& type, const vector<string>& skills, 
		const vector<TimeShift>& shifts, const vector<TimeShift>& breakWindows, const int breakDuration, const bool canMoveTrains) :
		id(id), name(name), type(type), skills(skills), shifts(shifts), breakWindows(breakWindows), breakDuration(breakDuration), canMoveTrains(canMoveTrains) {}
	Employee(const PBMemberOfStaff& pb_employee);
	Employee(const Employee& employee) = default;
	~Employee();

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


#endif