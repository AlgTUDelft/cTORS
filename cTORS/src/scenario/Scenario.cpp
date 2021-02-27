#include "Scenario.h"

const string Scenario::scenarioFileString = "scenario.json";

Scenario::Scenario() : startTime(0), endTime(0)
{
}

Scenario::Scenario(string folderName, const Location& location) {
	try {
		ifstream fileInput(fs::path(folderName) / fs::path(scenarioFileString));
		if (!fileInput.good())
			throw InvalidScenarioException("The specified file '" + folderName + "' does not exist");
		json j;
		fileInput >> j;
		importEmployeesFromJSON(j["workers"], location);
		importShuntingUnitsFromJSON(j, location);
		startTime = stoi(j.at("startTime").get<string>());
		endTime = stoi(j.at("endTime").get<string>());
	}
	catch (exception& e) {
		cout << "Error in loading scenario: " << e.what() << "\n";
		throw e;
	}
}

Scenario::Scenario(const Scenario& scenario) : 
	startTime(scenario.startTime), endTime(scenario.endTime)
{
	disturbances = scenario.disturbances;
	for (auto inc : scenario.incomingTrains)
		incomingTrains.push_back(new Incoming(*inc));
	for (auto out : scenario.outgoingTrains)
		outgoingTrains.push_back(new Outgoing(*out));
	for (auto e : scenario.employees)
		employees.push_back(new Employee(*e));
	//TODO disturbances
	//TODO tasks
}


Scenario::~Scenario()
{
	DELETE_VECTOR(incomingTrains)
	DELETE_VECTOR(outgoingTrains)
	DELETE_VECTOR(employees)
	//TODO  disturbances
	//TODO tasks
}

const size_t Scenario::GetNumberOfTrains() const {
	size_t res = 0;
	for (auto inc : incomingTrains)
		res += inc->GetShuntingUnit()->GetNumberOfTrains();
	return res;
}

void Scenario::importEmployeesFromJSON(const json& j, const Location& location) {
	for (auto& jit : j) {
		Employee* e = new Employee();
		jit.get_to(*e);
		string start = jit["startLocationId"].get<string>();
		string end = jit["endLocationId"].get<string>();
		e->AssignTracks(location.GetTrackByID(start), location.GetTrackByID(end));
		employees.push_back(e);
		debug_out("Imported Employee " << e->toString());
	}
	debug_out("finished loading employees from JSON");
}

void Scenario::importShuntingUnitsFromJSON(const json& j, const Location& location) {
	for (auto& jit : j["trainUnitTypes"]) {
		TrainUnitType* tt = new TrainUnitType();
		jit.get_to(*tt);
		if (TrainUnitType::types.find(tt->displayName) != TrainUnitType::types.end())
			delete TrainUnitType::types.at(tt->displayName);
		TrainUnitType::types[tt->displayName] = tt;
	}
	
	for (string in : {"in", "inStanding"}) {
		for (auto& jit : j[in]) {
			Incoming* inc = new Incoming();
			jit.get_to(*inc);
			string park = jit.at("parkingTrackPart").get<string>();
			string side = jit.at("sideTrackPart").get<string>();
			inc->assignTracks(location.GetTrackByID(park), location.GetTrackByID(side));
			inc->setInstanding(in == "inStanding");
			incomingTrains.push_back(inc);
		}
	}
	for (auto out : { "out", "outStanding" }) {
		for (auto& jit : j[out]) {
			Outgoing* outg = new Outgoing();
			jit.get_to(*outg);
			string park = jit.at("parkingTrackPart").get<string>();
			string side = jit.at("sideTrackPart").get<string>();
			outg->assignTracks(location.GetTrackByID(park), location.GetTrackByID(side));
			outg->setInstanding(out == "outStanding");
			outgoingTrains.push_back(outg);
		}
	}
}

