#include "Action.h"
#include "State.h"

void SetbackAction::Start(State* state) const {
	const ShuntingUnit* su = GetShuntingUnit();
	const ShuntingUnitState& suState = state->GetShuntingUnitState(su);
	state->SetPrevious(su, suState.position->GetOppositeSide(suState.previous));
	state->SwitchFrontTrain(su);
	for (auto e : GetDrivers()) {
		//TODO
	}
	state->AddActiveAction(su, this);
	state->SetBeginMoving(su, false);
}

void SetbackAction::Finish(State* state) const {
	state->RemoveActiveAction(su, this);
}

const string SetbackAction::toString() const {
	return "Setback " + GetShuntingUnit()->toString();
}


SetbackActionGenerator::SetbackActionGenerator(const json& params, const Location* location) : ActionGenerator(params, location) {
	params.at("constant_time").get_to(constantTime);
	params.at("default_time").get_to(defaultTime);
	params.at("norm_time").get_to(normTime);
	params.at("walk_time").get_to(walkTime);
}

int SetbackActionGenerator::GetDuration(const State* state, const ShuntingUnit* su, int numDrivers) const {
	if(defaultTime) return su->GetSetbackTime(state->GetFrontTrain(su), true, numDrivers < 2);
	return su->GetSetbackTime(state->GetFrontTrain(su), normTime, walkTime, constantTime);
}

const Action* SetbackActionGenerator::Generate(const State* state, const SimpleAction& action) const {
	auto su = state->GetShuntingUnitByTrainIDs(action.GetTrainIDs());
	auto suState = state->GetShuntingUnitState(su);
	vector<const Employee*> drivers;
	auto duration = GetDuration(state, su, drivers.size());
	return new SetbackAction(su, drivers, duration);
}

void SetbackActionGenerator::Generate(const State* state, list<const Action*>& out) const {
	if(state->GetTime()==state->GetEndTime()) return;
	bool driver_mandatory = false;//TODO get value from config
	for (const auto& [su, suState] : state->GetShuntingUnitStates()) {
		if (!suState.moving || suState.waiting || suState.HasActiveAction() || suState.inNeutral) continue;
		vector<const Employee*> drivers;
		if (driver_mandatory) {
			//TODO
		}
		out.push_back(Generate(state, Setback(su)));		
	}
}