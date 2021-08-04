#include "Scenario.h"

Scenario::Scenario() : startTime(0), endTime(0) {}

Scenario::Scenario(string scenarioFileString, const Location& location) {
	PBScenario pb_scenario;
	parse_json_to_pb(fs::path(scenarioFileString), &pb_scenario);
	Init(pb_scenario, location);
}

Scenario::Scenario(const PBScenario& pb_scenario, const Location& location) {
	Init(pb_scenario, location);
}

void Scenario::Init(const PBScenario& pb_scenario, const Location& location) {
	try {
		ImportEmployees(pb_scenario, location);
		ImportShuntingUnits(pb_scenario, location);
		startTime = pb_scenario.starttime();
		endTime = pb_scenario.endtime();
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

const Train* Scenario::GetTrainByID(int id) const {
	for(auto inc : incomingTrains) {
		for(auto& t: inc->GetShuntingUnit()->GetTrains()) {
			if(t.GetID() == id) return &t;
		}
	}
	return nullptr;
}

const Incoming* Scenario::GetIncomingBySU(const ShuntingUnit* su) const {
	for(auto inc : incomingTrains) {
		if(inc->GetShuntingUnit()->MatchesShuntingUnit(su)) return inc;
	}
	return nullptr;
}

const Outgoing* Scenario::GetOutgoingBySU(const ShuntingUnit* su) const {
	for(auto out : outgoingTrains) {
		if(out->GetShuntingUnit()->MatchesShuntingUnit(su)) return out;
	}
	return nullptr;
}

const Incoming* Scenario::GetIncomingByTrainID(int id) const {
	for(auto inc : incomingTrains) {
		if(inc->GetShuntingUnit()->GetTrainByID(id) != nullptr) return inc;
	}
	return nullptr;
}

const Outgoing* Scenario::GetOutgoingByTrainID(int id) const {
	for(auto out : outgoingTrains) {
		if(out->GetShuntingUnit()->GetTrainByID(id) != nullptr) return out;
	}
	return nullptr;
}

void Scenario::ImportEmployees(const PBScenario& pb_scenario, const Location& location) {
	for (auto& pb_e: pb_scenario.workers()) {
		Employee* e = new Employee(pb_e);
		string start = to_string(pb_e.startlocationid());
		string end = to_string(pb_e.endlocationid());
		e->AssignTracks(location.GetTrackByID(start), location.GetTrackByID(end));
		employees.push_back(e);
		debug_out("Imported Employee " << e->toString());
	}
	debug_out("finished loading employees from JSON");
}

template<class PBTrainGoal>
TrainGoal* ImportTrainGoal(const Location& location, const PBTrainGoal& m, bool in, bool standing) {
	TrainGoal* g = in ? static_cast<TrainGoal*>(new Incoming(m, standing)) : new Outgoing(m, standing);
	string park = to_string(m.parkingtrackpart());
	string side = to_string(m.sidetrackpart());
	g->assignTracks(location.GetTrackByID(park), location.GetTrackByID(side));
	return g;
}

void Scenario::ImportShuntingUnits(const PBScenario& pb_scenario, const Location& location) {
	for (auto& pb_train_type: pb_scenario.trainunittypes()) {
		if (TrainUnitType::types.find(pb_train_type.displayname()) == TrainUnitType::types.end()) {
			TrainUnitType* tt = new TrainUnitType(pb_train_type);
			TrainUnitType::types[tt->displayName] = tt;
		}
	}
	for(auto& pb_in : pb_scenario.in())
		incomingTrains.push_back(dynamic_cast<Incoming*>(ImportTrainGoal(location, pb_in, true, false)));
	for(auto& pb_in : pb_scenario.instanding())
		incomingTrains.push_back(dynamic_cast<Incoming*>(ImportTrainGoal(location, pb_in, true, true)));
	for(auto& pb_out : pb_scenario.out())
		outgoingTrains.push_back(dynamic_cast<Outgoing*>(ImportTrainGoal(location, pb_out, false, false)));
	for(auto& pb_out : pb_scenario.outstanding())
		outgoingTrains.push_back(dynamic_cast<Outgoing*>(ImportTrainGoal(location, pb_out, false, true)));
}

void Scenario::Serialize(PBScenario* pb_scenario) const {
	for(auto inc: incomingTrains) {
		inc->Serialize(
			inc->IsInstanding() ?
			pb_scenario->add_instanding() :
			pb_scenario->add_in()
		);
	}
	for(auto out: outgoingTrains) {
		out->Serialize(
			out->IsInstanding() ?
			pb_scenario->add_outstanding() :
			pb_scenario->add_out()
		);
	}
	pb_scenario->set_starttime(GetStartTime());
	pb_scenario->set_endtime(GetEndTime());
	for(auto& [name, type]: TrainUnitType::types) {
		type->Serialize(pb_scenario->add_trainunittypes());
	}
}

void Scenario::PrintScenarioInfo() const {
	cout << "|---------------------------|" << endl;
	cout << "|   Scenario                |" << endl;
	cout << "|---------------------------|" << endl;
	cout << "Start time: " << startTime << ", end time: " << endTime << endl << endl;
	if(incomingTrains.size() == 0)
		cout << "No arrivals" << endl << endl;
	else {
		cout << "Arrivals:" << endl;
		for(auto inc: incomingTrains) {
			cout << "\tT" << inc->GetTime() << ": \t" << inc->GetShuntingUnit() << " (" << inc->GetShuntingUnit()->GetTrainString() << ") at " 
				<< inc->GetParkingTrack() << " from " << inc->GetSideTrack();
			if(inc->IsInstanding())
				cout << " (instanding)";
			cout << endl;
		}
		cout << endl;
	}

	if(outgoingTrains.size() == 0)
		cout << "No departures" << endl << endl;
	else {
		cout << "Departures:" << endl;
		for(auto out: outgoingTrains) {
			cout << "\tT" << out->GetTime() << ": \t" << out->GetShuntingUnit() << " (" << out->GetShuntingUnit()->GetTrainString() << ") at " 
				<< out->GetParkingTrack() << " to " << out->GetSideTrack();
			if(out->IsInstanding())
				cout << " (outstanding ix-" << to_string(out->GetStandingIndex()) << ")";
			cout << endl;
		}
		cout << endl;
	}
}