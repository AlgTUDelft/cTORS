#pragma once
#ifndef ENGINE_H
#define ENGINE_H
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include "Utils.h"
#include "State.h"
#include "Scenario.h"
#include "Action.h"
#include "Event.h"
#include "Location.h"
#include "Exceptions.h"
#include "Config.h"
#include "Plan.h"

using namespace std;

class RunResult;
class POSPlan;

class LocationEngine
{
private:
	string path;
	Location location;
	Scenario originalScenario;
	Config config;
	ActionValidator actionValidator;
	ActionManager actionManager;
	unordered_map<State*, list<const Action*>> stateActionMap;
	unordered_map<State*, RunResult*> results;

	void ExecuteEvent(State* state, const Event* e);
	void ExecuteImmediateEvents(State * state);
public:
	LocationEngine() = delete;
	LocationEngine(const string &path);
	~LocationEngine();
	list<const Action*> &GetValidActions(State* state);
	list<const Action*> &Step(State* state, Scenario* scenario = nullptr);
	void ApplyAction(State* state, const Action* action);
	void ApplyAction(State* state, const SimpleAction& action);
	const Action* GenerateAction(const State* state, const SimpleAction& action) const;
	bool EvaluatePlan(const Scenario& scenario, const POSPlan& plan);
	State* StartSession(const Scenario& scenario);
	inline State* StartSession() { return StartSession(originalScenario); }
	void EndSession(State* state);
	inline const Location& GetLocation() const { return location; }
	inline const Scenario& GetScenario() const { return originalScenario; }
	void CalcShortestPaths();
	const Path GetPath(const State* state, const Move& move) const;
	RunResult* GetResult(State* state) const { return results.at(state); }
	RunResult* ImportResult(const string& path);
};

class Engine
{
private:
	map<const string, LocationEngine> engines;
	map<const State*, LocationEngine*> engineMap;
public:
	Engine() = default;
	Engine(const string& path);
	~Engine() = default;
	LocationEngine* GetOrLoadLocationEngine(const string& location);
	inline list<const Action*> &GetValidActions(State* state) const { return engineMap.at(state)->GetValidActions(state); }
	inline list<const Action*> &Step(State* state, Scenario* scenario = nullptr) const { return engineMap.at(state)->Step(state, scenario); }
	inline void ApplyAction(State* state, const Action* action) const { engineMap.at(state)->ApplyAction(state, action); }
	inline void ApplyAction(State* state, const SimpleAction& action) const {engineMap.at(state)->ApplyAction(state, action); }
	inline const Action* GenerateAction(const State* state, const SimpleAction& action) const { return engineMap.at(state)->GenerateAction(state, action); }
	
	inline bool EvaluatePlan(const string& location, const Scenario& scenario, const POSPlan& plan) { 
		return GetOrLoadLocationEngine(location)->EvaluatePlan(scenario, plan); }
	
	State* StartSession(const string& location, const Scenario& scenario);
	State* StartSession(const string& location );

	void EndSession(State* state);
	
	inline const Location& GetLocation(const string& location) const { return engines.at(location).GetLocation(); }
	inline const Scenario& GetScenario(const string& location) const { return engines.at(location).GetScenario(); }
	void CalcShortestPaths();
	inline const Path GetPath(const State* state, const Move& move) const { return engineMap.at(state)->GetPath(state, move); }
	inline RunResult* GetResult(State* state) const { return engineMap.at(state)->GetResult(state); }
	RunResult* ImportResult(const string& path);
};

#endif