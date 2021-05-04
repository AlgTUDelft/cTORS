#pragma once
#ifndef FACILITY_H
#define FACILITY_H
#include <vector>
#include <string>
#include "Track.h"
#include "Train.h"
#include "Utils.h"

class Facility
{
private:
	const int id;
	const string type;
	vector<Track*> tracks;
	const vector<string> tasks;
	const int simultaneousUsageCount;
	const double tStart;
	const double tEnd;
public:
	Facility() = delete;
	Facility(const int id, const string& type,
		const vector<string>& tasks, int simultaneousUsageCount, double tStart, double tEnd) : 
			id(id), type(type), tasks(tasks), simultaneousUsageCount(simultaneousUsageCount), tStart(tStart), tEnd(tEnd) {}
	Facility(const PBFacility& pb_facility) : Facility(pb_facility.id(), pb_facility.type(), ConvertPBTaskTypes(pb_facility.tasktypes()),
			pb_facility.simultaneoususagecount(), pb_facility.timewindow().start(), pb_facility.timewindow().end()) {}
	~Facility() = default;
	
	void AssignTracks(vector<Track*> tracks);
	
	inline int GetCapacity() const { return simultaneousUsageCount; }
	const inline vector<string> &GetTasks() const { return tasks; }
	const inline vector<Track*> &GetTracks() const { return tracks; }
	bool ExecutesTask(const Task* task) const;
	inline bool IsAvailable(int start, int duration) const { return start >= tStart && start + duration < tEnd; }
	
	inline const string toString() const { return "Facility " + to_string(id); }
	inline const int GetID() const { return id; }
	inline const string& GetType() const { return type; }
	
	inline bool operator==(const Facility& t) const { return (id == t.id); }
	inline bool operator!=(const Facility& t) const { return !(*this == t); }
};

#endif