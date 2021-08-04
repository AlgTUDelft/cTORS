/** \file Employee.h
 * Describes the Employee class
 */
#pragma once
#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include "Track.h"
using namespace std;

/** A TimeShift describes a time window with a start and end time in seconds */
struct TimeShift {
	int start;	/**< This TimeShift's start time in seconds */
	int end;	/**< This TimeShift's end time in seconds */
};

/**
 * An Employee on the shunting yard (not yet implemented)
 */
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
	/** Construct an Employee object from the given parameters */
	Employee(const string& id, const string& name, const string& type, const vector<string>& skills, 
		const vector<TimeShift>& shifts, const vector<TimeShift>& breakWindows, const int breakDuration, const bool canMoveTrains) :
		id(id), name(name), type(type), skills(skills), shifts(shifts), breakWindows(breakWindows), breakDuration(breakDuration), canMoveTrains(canMoveTrains) {}
	/** Construct an Employee object from the given protobuf object */
	Employee(const PBMemberOfStaff& pb_employee);
	/** Default copy constructor */
	Employee(const Employee& employee) = default;
	/** Employee destructor */
	~Employee();

	/** Get a string representation of this Employee */
	inline string toString() {
		return "Employee-" + id + " " + name;
	}

	/** Two Employee%s are equal if they have the same id */
	inline bool operator==(const Employee& e) const { return (id == e.id); }
	/** Two Employee%s are not equal if they do not have the same id */
	inline bool operator!=(const Employee& e) const { return !(*this == e); }

	/** Assign the start and (required) end location of this Employee */
	inline void AssignTracks(Track* start, Track* end) {
		startLocation = start;
		endLocation = end;
	}
};


#endif