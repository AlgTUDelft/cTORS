#pragma once
#include <vector>
#include <string>
#include "Track.h"
#include "Train.h"
using json = nlohmann::json;

class Facility
{
private:
	string id;
	string type;
	vector<Track*> tracks;
	vector<string> tasks;
	int simultaneousUsageCount;
	int tStart;
	int tEnd;
public:
	Facility();
	Facility(const string& id, const string& type, vector<Track*> tracks,
		vector<string> tasks, int simultaneousUsageCount, int tStart, int tEnd);
	~Facility();
	
	void AssignTracks(vector<Track*> tracks);
	
	inline int GetCapacity() const { return simultaneousUsageCount; }
	const inline vector<string> &GetTasks() const { return tasks; }
	const inline vector<Track*> &GetTracks() const { return tracks; }
	bool ExecutesTask(const Task* task) const;
	inline bool IsAvailable(int start, int duration) const { return start >= tStart && start + duration < tEnd; }
	
	void fromJSON(const json& j);
	
	inline const string toString() const { return "Facility " + id; }
	inline const string& GetID() const { return id; }
	inline const string& GetType() const { return type; }
	
	inline bool operator==(const Facility& t) const { return (id == t.id); }
	inline bool operator!=(const Facility& t) const { return !(*this == t); }
};

void from_json(const json& j, Facility& f);



