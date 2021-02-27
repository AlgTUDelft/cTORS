#pragma once
#include <list>
#include <unordered_map>
#include "Utils.h"
#include "State.h"
#include "Scenario.h"
#include "Action.h"
#include "Event.h"
#include "Location.h"
#include "Exceptions.h"
#include "Config.h"

using namespace std;

class Engine
{
private:
	string path;
	Location location;
	Scenario originalScenario;
	Config config;
	ActionValidator actionValidator;
	ActionManager actionManager;
	unordered_map<State*, list<const Action*>> stateActionMap;

	void ExecuteEvent(State* state, const Event* e);
	void ExecuteImmediateEvents(State * state);
public:
	Engine() = delete;
	Engine(const string &path);
	~Engine();
	list<const Action*> &GetValidActions(State* state);
	list<const Action*> &Step(State* state, Scenario* scenario = nullptr);
	void ApplyAction(State* state, const Action* action);
	void ApplyAction(State* state, const SimpleAction& action);
	State *StartSession(const Scenario& scenario);
	inline State *StartSession() { return StartSession(originalScenario); }
	void EndSession(State* state);
	inline const Location &GetLocation() const { return location; }
	inline const Scenario& GetScenario() const { return originalScenario; }
	void CalcShortestPaths(); 
};

