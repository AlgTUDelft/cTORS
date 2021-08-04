/** \file Facility.h
 * Describes the Facility class
 */
#pragma once
#ifndef FACILITY_H
#define FACILITY_H
#include "Track.h"
#include "Train.h"

/** 
 * A service Facility on the Location that can service certain Task%s
 */
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
	/** Construct a Facility given the parameters */
	Facility(const int id, const string& type,
		const vector<string>& tasks, int simultaneousUsageCount, double tStart, double tEnd) : 
			id(id), type(type), tasks(tasks), simultaneousUsageCount(simultaneousUsageCount), tStart(tStart), tEnd(tEnd) {}
	/** Construct a Facility given the protobuf object */
	Facility(const PBFacility& pb_facility) : Facility(pb_facility.id(), pb_facility.type(), ConvertPBTaskTypes(pb_facility.tasktypes()),
			pb_facility.simultaneoususagecount(), pb_facility.timewindow().start(), pb_facility.timewindow().end()) {}
	/** Default destructor */
	~Facility() = default;
	
	/** Assign the Track%s that for this Facility */
	void AssignTracks(vector<Track*> tracks);
	
	/** Get the capacity of this Facility (the number of Train%s it can serve simultaneously) */
	inline int GetCapacity() const { return simultaneousUsageCount; }
	/** Get a list of Task types that this Facility can service */
	const inline vector<string> &GetTasks() const { return tasks; }
	/** Get the list of Track%s of this Facility */
	const inline vector<Track*> &GetTracks() const { return tracks; }
	/** Returns true iff this Facility services the given Task */
	bool ExecutesTask(const Task* task) const;
	/** Returns true iff this Facility is active during the given time window */
	inline bool IsAvailable(int start, int duration) const { return start >= tStart && start + duration < tEnd; }
	/** Get a string representation of this Facility */
	inline const string toString() const { return "Facility " + to_string(id); }
	/** Get the id of this Facility */
	inline const int GetID() const { return id; }
	/** Get the type of this Facility */
	inline const string& GetType() const { return type; }
	
	/** Returns true iff the two Facilities have the same id */
	inline bool operator==(const Facility& t) const { return (id == t.id); }
	/** Returns true iff the two Facilities do not have the same id */
	inline bool operator!=(const Facility& t) const { return !(*this == t); }
};

//!\cond NO_DOC
STREAM_OPERATOR(Facility);
//!\endcond

#endif