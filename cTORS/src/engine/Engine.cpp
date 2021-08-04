#include "Engine.h"
using namespace std;

LocationEngine::LocationEngine(const string &path) : path(path), location(Location(path, true)), 
	config(Config(path)), actionManager(ActionManager(&config, &location)) {}


LocationEngine::~LocationEngine() {
	debug_out("Deleting LocationEngine");
	for(auto& [name, type]: TrainUnitType::types) {
		delete type;	
	}
	TrainUnitType::types.clear();
	vector<State*> states;
	for(auto& [state, action_list]: stateActionMap) {
		states.push_back(state);
	}
	for(auto state: states)
		EndSession(state);
	stateActionMap.clear();
	for(auto& [file, scenario]: scenarios) {
		delete scenario;
	}
	scenarios.clear();
	results.clear();
	debug_out("Done deleting LocationEngine");
}

const Scenario& LocationEngine::GetScenario(const string& scenarioFileString) {
	auto it = scenarios.find(scenarioFileString);
	if(it == scenarios.end()) {
		auto scenario = new Scenario(scenarioFileString, location);
		scenarios[scenarioFileString] = scenario;
		return *scenario;
	}
	return *it->second;
}

inline void CheckScenarioEnded(const State* state) {
	if (state->GetTime() > state->GetEndTime()) {
		if ((state->GetIncomingTrains().size() + state->GetOutgoingTrains().size()) > 0) {
			throw ScenarioFailedException("End of Scenario reached, but there are remaining incoming or outgoing trains.");
		} 
	}
}

void LocationEngine::Step(State * state) {
	ExecuteImmediateEvents(state);
	CheckScenarioEnded(state);
	EventQueue disturbances; // Currently an empty queue of disturbances. TODO get the disturbances from the Scenario
	while (!state->IsActionRequired() && state->GetNumberOfEvents() > 0) {
		debug_out("All shunting units are still active, but still " << state->GetNumberOfEvents() 
			<< " events available at T" << state->GetTime() << ".");
		const Event* evnt;
		if (disturbances.size() > 0 && disturbances.top()->GetTime() <= state->PeekEvent()->GetTime())
			evnt = disturbances.top();
		else
			evnt = state->PopEvent();
		ExecuteEvent(state, evnt);
		ExecuteImmediateEvents(state);
		CheckScenarioEnded(state);
	} 
	debug_out("Step done.");
}

bool LocationEngine::IsStateActive(const State* state) const {
	CheckScenarioEnded(state);
	return state->IsActionRequired();
}

void LocationEngine::ApplyAction(State* state, const Action* action) {
	debug_out("\tApplying action " + action->toString());
	int startTime = state->GetTime();
	auto sa = action->CreateSimple();
	state->StartAction(action);
	int duration = action->GetDuration();
	POSAction posaction(startTime, startTime + duration, duration, sa);
	results[state]->AddAction(posaction);
}

void LocationEngine::ApplyAction(State* state, const SimpleAction& action) {
	debug_out("\tApplying action " + action.toString());
	const Action* _action = GenerateAction(state, action);
	auto is_valid = actionManager.IsValid(state, _action);
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

void LocationEngine::ApplyWaitAllUntil(State* state, int time) {
	for(auto& [su, suState]: state->GetShuntingUnitStates()) {
		if(!suState.waiting && time - state->GetTime() > 0 && !suState.HasActiveAction()) {
			const auto& action = new WaitAction(su, time - state->GetTime());
			debug_out("Applying action " << action->toString() << " at T" << state->GetTime());
			ApplyAction(state, action);
		}
	}
	Step(state);
}

pair<bool, string> LocationEngine::IsValidAction(const State* state, const SimpleAction& action) const {
	const Action* _action;
	try {
		_action = GenerateAction(state, action);
	} catch(InvalidActionException& e) {
		return make_pair(false, e.what());
	}
	auto result = actionManager.IsValid(state, _action);
	delete _action;
	return result;
}

pair<bool, string> LocationEngine::IsValidAction(const State* state, const Action* action) const {
	return actionManager.IsValid(state, action);
}

const Action* LocationEngine::GenerateAction(const State* state, const SimpleAction& action) const {
	if(!config.IsGeneratorActive(action.GetGeneratorName()))
		throw InvalidActionException("Error in generating action (" + action.toString() +"): Action generator " + action.GetGeneratorName() + " is disabled.");
	try {
		return actionManager.GetGenerator(action.GetGeneratorName())->Generate(state, action);
	} catch(exception& e) {
		throw InvalidActionException("Error in generating action (" + action.toString() + "): " + e.what());
	}
}

list<const Action*> &LocationEngine::GetValidActions(State* state) {
	debug_out("Starting GetValidActions");
	if (state->IsChanged()) {
		auto& actions = stateActionMap.at(state);
		DELETE_LIST(actions)
		actionManager.Generate(state, actions);
		debug_out("Generated "+ to_string(actions.size())+" actions");
		state->SetUnchanged();
	}
	debug_out("Return valid actions: ");
	auto& actions = stateActionMap.at(state);
	int i=0;
	for (auto a : actions) {
		debug_out("\t" << setw(3) << right << to_string(i++) << ": " + a->toString());
	}
	return actions;
}

void LocationEngine::ExecuteImmediateEvents(State* state) {
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

void LocationEngine::ExecuteEvent(State* state, const Event* e) {
	auto a = e->GetAction();
	if (a != nullptr) {
		debug_out("\tFinishing action " + a->toString());
		state->FinishAction(a);
	}
	state->SetTime(e->GetTime());
	delete e;
}


void LocationEngine::ApplyActionAndStep(State* state, const Action* action) {
	ApplyAction(state, action);
	Step(state);
}

void LocationEngine::ApplyActionAndStep(State* state, const SimpleAction& action) {
	ApplyAction(state, action);
	Step(state);
}

bool LocationEngine::EvaluatePlan(const Scenario& scenario, const POSPlan& plan) {
	auto state = StartSession(scenario);
	Step(state);
	auto it = plan.GetActions().begin();
    while(it != plan.GetActions().end()) {
        try {
			if(DEBUG) state->PrintStateInfo();
            if(state->GetTime() >= it->GetSuggestedStart()) {
                debug_out("Applying action " << (it->GetAction())->toString() << " at T" << state->GetTime() 
					<< " [" << it->GetSuggestedStart() << "-" << it->GetSuggestedEnd() << "]");
				ApplyActionAndStep(state, *(it->GetAction()));
                it++;
            } else {
				ApplyWaitAllUntil(state, it->GetSuggestedStart());
			}
        } catch (ScenarioFailedException& e) {
			cout << "Scenario failed." << endl;
			return false;
			break;
		} catch (InvalidActionException& e) {
			cout << "Scenario failed. Invalid action: " << e.what() << "." << endl;
			return false;
			break;
		}
    }
	bool result = (state->GetShuntingUnits().size() == 0);
	EndSession(state);
	return result;
}

State* LocationEngine::StartSession(const Scenario& scenario) {
	debug_out("Start Session. (Currently " << stateActionMap.size() << " sessions)");
	State* state = new State(scenario, location.GetTracks());
	stateActionMap[state];
	results[state] = new RunResult(path, scenario);
	return state;
}

void LocationEngine::EndSession(State* state) {
	debug_out("End session. (Currently " << stateActionMap.size() << " sessions)");
	auto& actions = stateActionMap[state];
	auto& schedule = results[state];
	DELETE_LIST(actions);
	delete schedule;
	stateActionMap.erase(state);
	results.erase(state);
	delete state;
}

void LocationEngine::CalcShortestPaths() { 
	for(const auto& [trainTypeName, trainType]: TrainUnitType::types) {
		location.CalcShortestPaths(trainType);
	}
} 

void LocationEngine::CalcAllPossiblePaths() { 
	location.CalcAllPossiblePaths();
} 

const Path LocationEngine::GetPath(const State* state, const Move& move) const {
	static auto moveGenerator = static_cast<const MoveActionGenerator*>(actionManager.GetGenerator(move.GetGeneratorName()));
	return moveGenerator->GeneratePath(state, move);
}

RunResult* LocationEngine::ImportResult(const string& path) {
	PBRun run;
	parse_json_to_pb(path, &run);
	return RunResult::CreateRunResult(&location, run);
}

LocationEngine* Engine::GetOrLoadLocationEngine(const string& location) {
	auto it = engines.find(location);
	if(it== engines.end()) {
		engines.emplace(location, location);
		return &engines.at(location);
	}
	return &it->second;
}

State* Engine::StartSession(const string& location, const Scenario& scenario) {
	auto e = GetOrLoadLocationEngine(location);
	auto state = e->StartSession(scenario);
	engineMap[state] = e;
	return state;
}

void Engine::EndSession(State* state) {
	auto e = engineMap.at(state);
	engineMap.erase(state);
	e->EndSession(state);
}

void Engine::CalcShortestPaths() {
	for(auto& [loc, engine]: engines) {
		engine.CalcShortestPaths();
	}
}

void Engine::CalcAllPossiblePaths() {
	for(auto& [loc, engine]: engines) {
		engine.CalcAllPossiblePaths();
	}
}

RunResult* Engine::ImportResult(const string& path) {
	PBRun run;
	parse_json_to_pb(path, &run);
	return RunResult::CreateRunResult(*this, run);
}