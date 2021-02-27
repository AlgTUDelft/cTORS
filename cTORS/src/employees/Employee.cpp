#include "Employee.h"

Employee::~Employee() {
	skills.clear();
	shifts.clear();
	breakWindows.clear();
}

void Employee::fromJSON(const json& j) {
	j.at("id").get_to(id);
	j.at("type").get_to(type);
	j.at("name").get_to(name);
	j.at("skills").get_to(skills);
	j.at("shifts").get_to(shifts);
	j.at("breakWindows").get_to(shifts);
	j.at("breakDuration").get_to(breakDuration);
	j.at("canMoveTrains").get_to(canMoveTrains);
}

void from_json(const json& j, TimeShift& ts) {
	j.at("start").get_to(ts.start);
	j.at("end").get_to(ts.end);
}

void from_json(const json& j, Employee& ts) {
	ts.fromJSON(j);
}
