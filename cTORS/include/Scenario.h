#pragma once
#include <list>
#include <string>
#include <filesystem>
#include <fstream>
#include "EventQueue.h"
#include "Location.h"
#include "Exceptions.h"
#include "Employee.h"
#include "TrainGoals.h"
#include "Utils.h"
namespace fs = std::filesystem;
using namespace std;
using json = nlohmann::json;

class Scenario
{
private:
	static const string scenarioFileString;

	int startTime, endTime;
	EventQueue disturbances;
	vector<const Employee*> employees;
	vector<const Incoming*> incomingTrains;
	vector<const Outgoing*> outgoingTrains;
	vector<const Event*> disturbanceList;

	void importEmployeesFromJSON(const json& j, const Location& location);
	void importShuntingUnitsFromJSON(const json& j, const Location& location);

public:
	Scenario();
	Scenario(string path, const Location& location);
	Scenario(const Scenario& scenario);
	~Scenario();
	
	inline int GetStartTime() const { return startTime; }
	inline int GetEndTime() const { return endTime; }
	inline const EventQueue& GetDisturbances() const { return disturbances; }
	inline const vector<const Event*>& GetDisturbanceVector() const { return disturbanceList; }
	inline const vector<const Outgoing*>& GetOutgoingTrains() const { return outgoingTrains; }
	inline const vector<const Incoming*>& GetIncomingTrains() const { return incomingTrains; }
	inline const vector<const Employee*>& GetEmployees() const { return employees; }
	const size_t GetNumberOfTrains() const;

	inline void SetStartTime(int startTime) { this->startTime = startTime; }
	inline void SetEndTime(int endTime) { this->endTime = endTime; }
	inline void SetOutgoingTrains(vector<const Outgoing*> outgoingTrains) { this->outgoingTrains = outgoingTrains; }
	inline void SetIncomingTrains(vector<const Incoming*> incomingTrains) { this->incomingTrains = incomingTrains; }
	inline void SetEmployees(vector<const Employee*> employees) { this->employees = employees; }
	inline void SetDisturbances(EventQueue disturbances) { this->disturbances = disturbances; }

	inline void AddOutgoingTrain(const Outgoing* outgoingTrain) { outgoingTrains.push_back(outgoingTrain); }
	inline void AddIncomingTrain(const Incoming* incomingTrain) { incomingTrains.push_back(incomingTrain); }
	inline void AddDisturbance(const Event* disturbance) { disturbances.push(disturbance); }
	inline void AddEmployee(const Employee* employee) { employees.push_back(employee); }
};

