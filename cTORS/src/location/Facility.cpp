#include "Facility.h"

Facility::Facility() :
	id(""), type(""), simultaneousUsageCount(0),
	tStart(0), tEnd(0) {}

Facility::Facility(const string& id, const string& type, vector<Track*> tracks,
	vector<string> tasks, int simultaneousUsageCount, int tStart, int tEnd) :
	id(id), type(type), tracks(tracks), tasks(tasks),
	simultaneousUsageCount(simultaneousUsageCount), tStart(tStart), tEnd(tEnd) {}

Facility::~Facility() {
	tasks.clear();
}

bool Facility::ExecutesTask(const Task* task) const {
	const string taskName = task->toString();
	auto it = find_if(tasks.begin(), tasks.end(), [taskName](const string& s) -> bool { return s == taskName; });
	if (it == tasks.end()) return false;
	return true;	
}

void Facility::AssignTracks(vector<Track*> tracks) {
	this->tracks = tracks;
}

void Facility::fromJSON(const json& j) {
	j.at("id").get_to(id);
	j.at("type").get_to(type);
	for (auto jit : j.at("taskTypes")) {
		tasks.push_back(jit.at("other"));
	}
	j.at("simultaneousUsageCount").get_to(simultaneousUsageCount);
	j.at("timeWindow").at("start").get_to(tStart);
	j.at("timeWindow").at("end").get_to(tEnd);
}

void from_json(const json& j, Facility& f) {
	f.fromJSON(j);
}