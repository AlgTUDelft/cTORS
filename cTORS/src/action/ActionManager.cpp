#include "Action.h"
#include "BusinessRules.h"

void ActionManager::AddGenerator(const string& name, const ActionGenerator* generator) {
	if (config->IsGeneratorActive(name)) {
		generators.push_back(generator);
		generatorMap[name] = generator;
	}
}

void ActionManager::Generate(const State* state, list<const Action*>& out) const {
	for (auto& generator: generators) {
		auto size = out.size();
		generator->Generate(state, out);
		//Filter new Actions
		out.erase(remove_if(next(out.begin(), size), out.end(), [this, state](const Action*& a) {
			auto result = IsValid(state, a);
			if (!result.first) {
				debug_out("Invalid action: " + a->toString() + " - " + result.second);
				delete a;
				return true;
			}
			return false;
		}), out.end());
	}
}

pair<bool, string> ActionManager::IsValid(const State* state, const Action* action) const {
	for (auto rule : validators) {
		auto result = rule->IsValid(state, action);
		if (!result.first)
			return result;
	}
	return make_pair(true,"");
}

#ifndef ADD_GENERATOR
#define ADD_GENERATOR(name, generator)\
AddGenerator(name, new generator(config->GetActionParameters(name), location));
#endif

#ifndef ADD_VALIDATOR
#define ADD_VALIDATOR(validator) \
if (config->IsBusinessRuleActive(#validator)) { \
	validators.push_back(new validator(config)); \
} 
#endif

ActionManager::~ActionManager() {
	DELETE_VECTOR(validators);
	DELETE_VECTOR(generators);
}

void ActionManager::AddGenerators() {
	ADD_GENERATOR("move", MoveActionGenerator);
	ADD_GENERATOR("move_helper", MoveHelperGenerator);
	ADD_GENERATOR("arrive", ArriveActionGenerator);
	ADD_GENERATOR("exit", ExitActionGenerator)
	ADD_GENERATOR("wait", WaitActionGenerator);
	ADD_GENERATOR("service", ServiceActionGenerator);
	ADD_GENERATOR("set_back", SetbackActionGenerator);
	ADD_GENERATOR("split", SplitActionGenerator);
	ADD_GENERATOR("combine", CombineActionGenerator);
}

void ActionManager::AddValidators() {
	//Arrival and departure
	ADD_VALIDATOR(in_correct_time_rule)
	ADD_VALIDATOR(out_correct_time_rule)
	ADD_VALIDATOR(out_correct_order_rule)

	//Track occupation
	ADD_VALIDATOR(single_move_track_rule)
	ADD_VALIDATOR(length_track_rule)
	ADD_VALIDATOR(blocked_track_rule)
	ADD_VALIDATOR(blocked_first_track_rule)

	//Parking
	ADD_VALIDATOR(electric_track_rule)
	ADD_VALIDATOR(legal_on_parking_track_rule)
	ADD_VALIDATOR(legal_on_setback_track_rule)

	//Shunting
	ADD_VALIDATOR(electric_move_rule)
	ADD_VALIDATOR(setback_once_rule)
	ADD_VALIDATOR(setback_track_rule)

	//Service tasks
	ADD_VALIDATOR(correct_facility_rule)
	ADD_VALIDATOR(mandatory_service_task_rule)
	ADD_VALIDATOR(optional_service_task_rule)
	ADD_VALIDATOR(understaffed_rule)

	//Facility
	ADD_VALIDATOR(available_facility_rule)
	ADD_VALIDATOR(capacity_facility_rule)
	ADD_VALIDATOR(disabled_facility_rule)

	//Employee

	//Combine and Split
	ADD_VALIDATOR(order_preserve_rule)
	ADD_VALIDATOR(park_combine_split_rule)
	ADD_VALIDATOR(setback_combine_split_rule)

	//Matching
}
