#include "Action.h"
#include "State.h"

void ServiceAction::Start(State* state) const {
	const ShuntingUnit* su = GetShuntingUnit();
	auto tu = GetTrain();
	auto ta = GetTask();
	state->AddActiveTaskToTrain(tu, *ta);
	for (auto e : GetEmployees()) {
		//TODO
	}
	state->RemoveTaskFromTrain(tu, *ta);
	state->AddActiveAction(su, this);
}

void ServiceAction::Finish(State* state) const {
	auto tu = GetTrain();
	auto ta = GetTask();
	state->RemoveActiveAction(su, this);
	state->RemoveActiveTaskFromTrain(tu, *ta);
}

const string ServiceAction::toString() const {
	return "Service task " + task.toString() + " to train " + GetTrain()->toString() + " of " + GetShuntingUnit()->toString()+  " at facility "+ facility->toString() ;
}

const Action* ServiceActionGenerator::Generate(const State* state, const SimpleAction& action) const {
	auto service = static_cast<const Service*>(&action);
	auto su = state->GetShuntingUnitByTrainIDs(action.GetTrainIDs());
	if(su == nullptr) throw InvalidActionException("Shunting unit with trains " + Join(action.GetTrainIDs(), "-") + " not found");
	auto train = su->GetTrainByID(service->GetTrain().GetID());
	auto tasks = state->GetTasksForTrain(train);
	auto& task = service->GetTask();
	auto it = find(tasks.begin(), tasks.end(), task);
	if(it==tasks.end())
		throw invalid_argument("The service task " + task.toString() + " for train " + train->toString() + " could not be found.");
	auto facility = location->GetFacilityByID(service->GetFacilityID());
	return new ServiceAction(su, train, *it, facility, vector<const Employee*> {});
}

void ServiceActionGenerator::Generate(const State* state, list<const Action*>& out) const {
	if(state->GetTime()==state->GetEndTime()) return;
	for (const auto& [su, suState] : state->GetShuntingUnitStates()) {
		if (suState.moving || suState.waiting || suState.HasActiveAction()) continue;
		auto tr = suState.position;
		auto& fas = tr->GetFacilities();
		for (auto& tu : su->GetTrains()) {
			for (const Task& task : state->GetTasksForTrain(&tu)) {
				for (auto fa : fas) {
					out.push_back(Generate(state, Service(su, task, tu, fa)));
				}
			}
		}
	}
}