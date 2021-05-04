#pragma once
#ifndef BUSINESS_RULES_H
#define BUSINESS_RULES_H
#include "Action.h"
#include "State.h"


#ifndef DEFINE_BUSINESSRULE
#define DEFINE_BUSINESSRULE(name) \
class name : public BusinessRule { \
public: \
	name() = delete; \
	name(const Config* config) : BusinessRule(config) {}; \
	pair<bool, string> IsValid(const State* state, const Action* action) const override; \
};
#endif

	
class BusinessRule {
private:
	const Config* config;
public:
	BusinessRule() = delete;
	BusinessRule(const Config* config) : config(config) {};
	virtual pair<bool, string> IsValid(const State*, const Action* action) const = 0;
};

//Arrival and departure
DEFINE_BUSINESSRULE(end_correct_order_on_track_rule)
DEFINE_BUSINESSRULE(in_correct_time_rule)
DEFINE_BUSINESSRULE(out_correct_order_rule)
DEFINE_BUSINESSRULE(out_correct_time_rule)
DEFINE_BUSINESSRULE(out_correct_track_rule)

//Track occupation
DEFINE_BUSINESSRULE(blocked_first_track_rule)
DEFINE_BUSINESSRULE(blocked_track_rule)
DEFINE_BUSINESSRULE(length_track_rule)
DEFINE_BUSINESSRULE(single_move_track_rule)

//Parking
DEFINE_BUSINESSRULE(electric_track_rule)
DEFINE_BUSINESSRULE(legal_on_parking_track_rule)
DEFINE_BUSINESSRULE(legal_on_setback_track_rule)

//Shunting
DEFINE_BUSINESSRULE(electric_move_rule)
DEFINE_BUSINESSRULE(setback_once_rule)
DEFINE_BUSINESSRULE(setback_track_rule)

//Facility
DEFINE_BUSINESSRULE(available_facility_rule)
DEFINE_BUSINESSRULE(capacity_facility_rule)
DEFINE_BUSINESSRULE(disabled_facility_rule)

//Service tasks
DEFINE_BUSINESSRULE(correct_facility_rule)
DEFINE_BUSINESSRULE(mandatory_service_task_rule)
DEFINE_BUSINESSRULE(optional_service_task_rule)
DEFINE_BUSINESSRULE(understaffed_rule)

//Combine and split
DEFINE_BUSINESSRULE(order_preserve_rule)
DEFINE_BUSINESSRULE(park_combine_split_rule)
DEFINE_BUSINESSRULE(setback_combine_split_rule)

#endif