#include "Facility.h"

bool Facility::ExecutesTask(const Task* task) const {
	const string taskName = task->toString();
	auto it = find_if(tasks.begin(), tasks.end(), [taskName](const string& s) -> bool { return s == taskName; });
	if (it == tasks.end()) return false;
	return true;	
}

void Facility::AssignTracks(vector<Track*> tracks) {
	this->tracks = tracks;
}