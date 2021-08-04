/** \file BusinessRules.h
 * Describes the BusinessRules class with its subclasses
 */
#pragma once
#ifndef BUSINESS_RULES_H
#define BUSINESS_RULES_H
#include "State.h"


#ifndef DEFINE_BUSINESSRULE
/** MACRO for defining BusinessRule's subclasses to prevent duplicate code */
#define DEFINE_BUSINESSRULE(name) \
class name : public BusinessRule { \
public: \
	name() = delete; \
	/** Construct this name using the given Config object */ \
	name(const Config* config) : BusinessRule(config) {}; \
	pair<bool, string> IsValid(const State* state, const Action* action) const override; \
};
#endif

/**
 * Abstract base class for all the business rules. The ActionValidator class
 * uses these rules to validate actions
 */
class BusinessRule {
private:
	const Config* config;
public:
	BusinessRule() = delete;
	
	/** Construct this BusinessRule using the given Config object */
	BusinessRule(const Config* config) : config(config) {};
	
	/**
	 * Validate the action for the given state.
	 * @return A pair describing 1) whether the action is valid, and 2) if not, why
	 */
	virtual pair<bool, string> IsValid(const State*, const Action* action) const = 0;
};

//Arrival and departure
/**
 * Rule that verifies that shunting units which stay in the shunting yard after 
 * the scheduling period will be located in the right order on their track.
 */
DEFINE_BUSINESSRULE(end_correct_order_on_track_rule)

/**
 * Rule that verifies that shunting units that are arriving, arrive at the 
 * correct time. Note: shunting units will never arrive too early, so this rule 
 * only checks if a shunting unit arrives too late.
 */
DEFINE_BUSINESSRULE(in_correct_time_rule)

/**
 * Rule that verifies that leaving shunting units have their train units in the 
 * correct order when they leave the shunting yard.
 */
DEFINE_BUSINESSRULE(out_correct_order_rule)

/**
 * Rule that verifies that leaving shunting units leave at the correct time.
 */
DEFINE_BUSINESSRULE(out_correct_time_rule)

/**
 * Rule that verifies that leaving shunting units leave over the correct tracks.
 */
DEFINE_BUSINESSRULE(out_correct_track_rule)

//Track occupation
/** 
 * Rule that verifies that shunting units, upon starting a movement,
 * are not blocked on exit by other shunting units on their current track. 
 */
DEFINE_BUSINESSRULE(blocked_first_track_rule)

/** 
 * Rule that verifies that moving shunting units are not blocked by other shunting units.
 */
DEFINE_BUSINESSRULE(blocked_track_rule)

/** 
 * Rule that verifies that shunting units on a single track do not take up more 
 * space than available on that track.
 */
DEFINE_BUSINESSRULE(length_track_rule)

/** 
 * Rule that verifies that at most one shunting unit can use a piece of track at a given time.
 */
DEFINE_BUSINESSRULE(single_move_track_rule)

//Parking
/**
 * Rule that verifies that shunting units which need electricity park only on electrified tracks.
 */
DEFINE_BUSINESSRULE(electric_track_rule)

/**
 * Rule that verifies that parked shunting units are on a track where parking is allowed.
 */
DEFINE_BUSINESSRULE(legal_on_parking_track_rule)

/**
 * Rule that verifies if a shunting unit is parked on a track where setback is allowed.
 */
DEFINE_BUSINESSRULE(legal_on_setback_track_rule)

//Shunting
/**
 * Rule that verifies that shunting units which need electricity park only on electrified tracks.
 */
DEFINE_BUSINESSRULE(electric_move_rule)

/** 
 * Rule that verifies that a setback action is not performed on a shunting unit 
 * which is already in a neutral state. A shunting unit is in a neutral state if a 
 * setback or service action is performed.
 */
DEFINE_BUSINESSRULE(setback_once_rule)

/** 
 * Rule that verifies that performing a setback action on a shunting unit is 
 * allowed on the track where the shunting unit is at.
 */
DEFINE_BUSINESSRULE(setback_track_rule)

//Facility
/**
 * Rule that verifies that tasks assigned to a facility are only executed when that facility is available.
 */
DEFINE_BUSINESSRULE(available_facility_rule)

/**
 * Rule that verifies that no more tasks are executed at a facility than the facility can handle.
 */
DEFINE_BUSINESSRULE(capacity_facility_rule)

/**
 * Rule that verifies that no tasks are assigned to facilities which are disabled by a disturbance.
 */
DEFINE_BUSINESSRULE(disabled_facility_rule)

//Service tasks
/**
 * Rule that verifies that service tasks are executed at the correct facility.
 */
DEFINE_BUSINESSRULE(correct_facility_rule)

/**
 * Rule that verifies that all required service tasks are performed before a shunting unit leaves the shunting yard.
 */
DEFINE_BUSINESSRULE(mandatory_service_task_rule)

/** 
 * Rule that verifies that all optional service tasks are performed before a shunting unit leaves the shunting yard.
 */
DEFINE_BUSINESSRULE(optional_service_task_rule)

/**
 * Rule that verifies that all tasks have enough employees assigned, with the right skills,
 * such that the task will have all of its required skills available.
 */
DEFINE_BUSINESSRULE(understaffed_rule)

//Combine and split
/**
 * Rule that verifies that combining or splitting shunting units does not change 
 * the order of train units on a track.
 */
DEFINE_BUSINESSRULE(order_preserve_rule)

/**
 * Rule that verifies that combine and split actions on shunting units 
 * are only performed on tracks where parking is allowed.
 */
DEFINE_BUSINESSRULE(park_combine_split_rule)

/**
 * Rule that verifies that combine and split actions on shunting units
 * are only performed on tracks where setback is allowed.
 */
DEFINE_BUSINESSRULE(setback_combine_split_rule)

#endif