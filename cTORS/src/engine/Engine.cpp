#include <list>
#include "Engine.h"
using namespace std;

Engine::Engine(const string &path) : path(path), location(Location(path)), 
	originalScenario(Scenario(path, location)), config(Config(path)),
	actionValidator(ActionValidator(&config)), actionManager(ActionManager(&config, &location)) {}


Engine::~Engine() {
	TrainUnitType::types.clear();
	stateActionMap.clear();
}

list<const Action*> &Engine::Step(State * state, Scenario * scenario) {
	ExecuteImmediateEvents(state);
	auto& actions = GetValidActions(state);
	debug_out("Got valid actions succesfully");
	EventQueue disturbances = scenario ? scenario->GetDisturbances() : EventQueue();
	while (actions.empty() && state->GetNumberOfEvents() > 0) {
		debug_out("No actions but still events available");
		const Event* evnt;
		if (disturbances.size() > 0 && disturbances.top()->GetTime() <= state->PeekEvent()->GetTime())
			evnt = disturbances.top();
		else
			evnt = state->PopEvent();
		if (evnt->GetTime() != state->GetTime() && state->IsAnyInactive())
			throw ScenarioFailedException();
		ExecuteEvent(state, evnt);
		ExecuteImmediateEvents(state);
		if (state->GetTime() > state->GetEndTime()) {
			if ((state->GetIncomingTrains().size() + state->GetOutgoingTrains().size()) > 0) {
				throw ScenarioFailedException();
			} else {
				DELETE_LIST(actions)
			}
		} else {
			actions = GetValidActions(state);
		}
	} 
	debug_out("Done getting actions. Found " << to_string(actions.size()) << " actions.");
	return actions;
}

void Engine::ApplyAction(State* state, const Action* action) {
	debug_out("\tApplying action " + action->toString());
	state->StartAction(action);
}

void Engine::ApplyAction(State* state, const SimpleAction& action) {
	const Action* _action;
	try {
		debug_out("\tApplying action " + action.toString());
		_action = actionManager.GetGenerator(action.GetGeneratorName())->Generate(state, action);
	} catch(exception& e) {
		throw InvalidActionException("Error in generating action (" + action.toString() + "): " + e.what());
	}
	auto is_valid = actionValidator.IsValid(state, _action);
	if(!is_valid.first) {
		delete _action;
		throw InvalidActionException(is_valid.second);
	}
	try {
		ApplyAction(state, _action);
	} catch(exception& e) {
		throw InvalidActionException("Error in applying action (" + _action->toString() + "): " + e.what());
	}
}

list<const Action*> &Engine::GetValidActions(State* state) {
	debug_out("Starting GetValidActions");
	if (state->IsChanged()) {
		auto& actions = stateActionMap.at(state);
		DELETE_LIST(actions)
		actionManager.Generate(state, actions);
		debug_out("Generated "+ to_string(actions.size())+" actions");
		actionValidator.FilterValid(state, actions);
		debug_out("GetValidActions list filtered");
		state->SetUnchanged();
	}
	return stateActionMap.at(state);
}

void Engine::ExecuteImmediateEvents(State* state) {
	if (state == nullptr) {
		throw runtime_error("state == null, something went wrong");
	}
	debug_out("Execute immediate events (" << to_string(state->GetNumberOfEvents()) << " events queued)");
	while (state->GetNumberOfEvents() > 0) {
		auto evnt = state->PeekEvent();
		debug_out("Next event at T=" << to_string(evnt->GetTime()) << ": " << evnt->toString());
		if (evnt->GetTime() > state->GetTime()) break;
		evnt = state->PopEvent();
		ExecuteEvent(state, evnt);
	}
}

void Engine::ExecuteEvent(State* state, const Event* e) {
	auto a = e->GetAction();
	if (a != nullptr) {
		debug_out("\tFinishing action " + a->toString());
		state->FinishAction(a);
	}
	state->SetTime(e->GetTime());
	delete e;
}

State* Engine::StartSession(const Scenario& scenario) {
	State* state = new State(scenario, location.GetTracks());
	stateActionMap[state];
	return state;
}

void Engine::EndSession(State* state) {
	auto& actions = stateActionMap[state];
	DELETE_LIST(actions);
	stateActionMap.erase(state);
	delete state;
}

void Engine::CalcShortestPaths() { 
	bool byTrackType = true; //TODO read parameter for distance matrix from config file
	for(const auto& [trainTypeName, trainType]: TrainUnitType::types) {
		location.CalcShortestPaths(byTrackType, trainType);
	}
} 