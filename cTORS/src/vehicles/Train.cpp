#include "Train.h"

map<string, TrainUnitType *> TrainUnitType::types;

Train::Train(int id, TrainUnitType* type)
{
	this->id = id;
	this->type = type;
	this->forcedMatch = false;
}

Train::Train(const Train &train) : id(train.id), type(train.type), forcedMatch(train.forcedMatch) {}

Train::~Train() {}

bool Train::operator==(const Train& train) const {
	return (id != -1 && id == train.id) || this == &train;
}

const string Train::toString() const {
	return "(" + to_string(id) + ", " + type->toString() + ")";
}

void Train::fromJSON(const json& j) {
	string temp = j.at("id").get<string>();
	if (temp == "****")
		id = -1;
	else id = stoi(temp);
	type = TrainUnitType::types.at(j.at("typeDisplayName"));
}

void from_json(const json& j, TrainUnitType& tt) {
	j.at("displayName").get_to(tt.displayName);
	j.at("carriages").get_to(tt.carriages);
	j.at("length").get_to(tt.length);
	tt.combineDuration = stoi(j.at("combineDuration").get<string>());
	tt.splitDuration = stoi(j.at("splitDuration").get<string>());
	tt.backNormTime = stoi(j.at("backNormTime").get<string>());
	tt.backAdditionTime = stoi(j.at("backAdditionTime").get<string>());
	tt.setbackTime = tt.carriages*tt.backAdditionTime;
	tt.travelSpeed = stoi(j.at("travelSpeed").get<string>());
	tt.startUpTime = stoi(j.at("startUpTime").get<string>());
	j.at("typePrefix").get_to(tt.typePrefix);
	j.at("needsLoco").get_to(tt.needsLoco);
	j.at("isLoco").get_to(tt.isLoco);
	j.at("needsElectricity").get_to(tt.needsElectricity);
}

void from_json(const json& j, Task& t) {
	j.at("priority").get_to(t.priority);
	j.at("type").at("other").get_to(t.taskType);
	t.duration = stoi(j.at("duration").get<string>());
	j.at("requiredSkills").get_to(t.skills);
}
