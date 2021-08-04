#include "State.h"

State::State(const Scenario& scenario, const vector<Track*>& tracks) {
	time = scenario.GetStartTime();
	startTime = scenario.GetStartTime();
	endTime = scenario.GetEndTime();
	for(auto inc: scenario.GetIncomingTrains())
		incomingTrains.push_back(new Incoming(*inc));
	for(auto out: scenario.GetOutgoingTrains())
		outgoingTrains.push_back(new Outgoing(*out));
	for(auto e: scenario.GetEmployees())
		employees.push_back(new Employee(*e));
	for (auto in : incomingTrains)
		AddEvent(in);
	for (auto out : outgoingTrains)
		AddEvent(out);
	changed = true;
	for(auto track : tracks) trackStates[track];
}

State::~State() {
	debug_out("Deleting state");
	DELETE_VECTOR(shuntingUnits);
	DELETE_VECTOR(incomingTrains);
	DELETE_VECTOR(outgoingTrains);
	DELETE_VECTOR(employees);
}

void State::SetTime(int time) {
	if (time != this->time) {
		changed = true;
		this->time = time;
	}
}

const Event* State::PeekEvent() const
{
	if (events.size() == 0)
		return nullptr;
	return events.top();
}

const Event* State::PopEvent()
{
	auto evnt = events.top();
	events.pop();
	return evnt;
}

void State::AddEvent(const Incoming *in) {
	Event* event = new Event(in);
	events.push(event);
	debug_out("Push event " << event->toString() << " at T=" << to_string(event->GetTime()));
}

void State::AddEvent(const Outgoing *out) {
	Event* event = new Event(out);
	events.push(event);
	debug_out("Push event " << event->toString() << " at T=" << to_string(event->GetTime()));
}

void State::AddEvent(const Action* action) {
	Event* event = new Event(time + action->GetDuration(), action);
	events.push(event);
	debug_out("Push event " << event->toString() << " at T=" << to_string(event->GetTime()));
}

void State::StartAction(const Action* action) {
	if (action == nullptr) return;
	changed = true;
	try {
		action->Start(this);
	} catch(const exception& e) {
		throw InvalidActionException("Exception in starting action.", e);
	}
	AddEvent(action);
}

void State::FinishAction(const Action* action) {
	if (action == nullptr) return;
	changed = true;
	try {
		action->Finish(this);
	} catch(const exception& e) {
		throw InvalidActionException("Exception in finishing action.", e);
	}
}

const ShuntingUnit* State::AddShuntingUnitToState(const ShuntingUnit* su, const Track* track, const Track* previous, const Train* frontTrain) {
	auto shuntingUnit = new ShuntingUnit(*su);
	shuntingUnits.push_back(shuntingUnit);
	auto& trains = shuntingUnit->GetTrains();
	shuntingUnitStates.emplace(shuntingUnit, ShuntingUnitState(track, previous, trains.front() == *frontTrain ? &trains.front() : &trains.back()));
	for(auto& train: trains) {
		trainStates[&train];
		trainIDToShuntingUnit[train.GetID()] = shuntingUnit;
		trainIDToTrain[train.GetID()] = &train;
	}
	return shuntingUnit;
}

void State::AddShuntingUnit(const ShuntingUnit* su, const Track* track, const Track* previous, const Train* frontTrain) {
	su = AddShuntingUnitToState(su, track, previous, frontTrain);
	OccupyTrack(su, track, previous);
}

void State::AddShuntingUnitOnPosition(const ShuntingUnit* su, const Track* track, const Track* previous,
		const Train* frontTrain, int positionOnTrack) {
	su = AddShuntingUnitToState(su, track, previous, frontTrain);
	InsertOnTrack(su, track, previous, positionOnTrack);
}

void State::MoveShuntingUnit(const ShuntingUnit* su, const Track* to, const Track* previous) {
	RemoveOccupation(su);
	OccupyTrack(su, to, previous);
}

void State::RemoveOccupation(const ShuntingUnit* su) {
	const Track* current = GetPosition(su);
	auto& occ = trackStates[current].occupations;
	auto it = find_if(occ.begin(), occ.end(), [su](const ShuntingUnit* s) -> bool { return *su == *s; });
	if (it != occ.end()) {
		occ.erase(it);
	}
}

void State::OccupyTrack(const ShuntingUnit* su, const Track* track, const Track* previous) {
	if(track->IsASide(previous))
		InsertOnTrack(su, track, previous, 0);
	else
		InsertOnTrack(su, track, previous, trackStates[track].occupations.size());
}

void State::InsertOnTrack(const ShuntingUnit* su, const Track* track, const Track* previous, int positionOnTrack) {
	auto& occ = trackStates[track].occupations;
	if(positionOnTrack == occ.size())
		occ.push_back(su);
	else {
		auto it = occ.begin();
		advance(it, positionOnTrack);
		occ.insert(it, su);
	}
	SetPosition(su, track);
	SetPrevious(su, previous);
}

void State::ReserveTracks(const vector<const Track*>& tracks) {
	for (auto t : tracks)
		ReserveTrack(t);
}

void State::ReserveTracks(const list<const Track*>& tracks) {
	for (auto t : tracks)
		ReserveTrack(t);
}

void State::FreeTracks(const vector<const Track*>& tracks) {
	for (auto t : tracks)
		FreeTrack(t);
}

void State::FreeTracks(const list<const Track*>& tracks) {
	for (auto t : tracks)
		FreeTrack(t);
}

void State::RemoveIncoming(const Incoming* incoming) {
	auto it = find_if(incomingTrains.begin(), incomingTrains.end(), [incoming](const Incoming* inc) -> bool { return *inc == *incoming; });
	if(it != incomingTrains.end())
		incomingTrains.erase(it);
}

void State::RemoveOutgoing(const Outgoing* outgoing) {
	auto it = find_if(outgoingTrains.begin(), outgoingTrains.end(), [outgoing](const Outgoing* out) -> bool { return *out == *outgoing; });
	if (it != outgoingTrains.end())
		outgoingTrains.erase(it);
}

void State::RemoveShuntingUnit(const ShuntingUnit* su) {
	RemoveOccupation(su);
	shuntingUnitStates.erase(su);
	for(auto& train: su->GetTrains()) {
		trainIDToShuntingUnit.erase(train.GetID());
		trainIDToTrain.erase(train.GetID());
		trainStates.erase(&train);
	}
	auto it = find_if(shuntingUnits.begin(), shuntingUnits.end(), [su](const ShuntingUnit* s) -> bool { return *su == *s; });
	if (it != shuntingUnits.end()) {
		delete *it;
		shuntingUnits.erase(it);
	}
}

bool State::HasShuntingUnit(const ShuntingUnit* su) const {
	auto it2 = find_if(shuntingUnits.begin(), shuntingUnits.end(), [su](const ShuntingUnit* s) -> bool 
		{ return *su == *s && su->MatchesShuntingUnit(s); });
	return (it2 != shuntingUnits.end());
}

const ShuntingUnit* State::GetShuntingUnitByID(int id) const {
	auto it = find_if(shuntingUnits.begin(), shuntingUnits.end(), [id](const ShuntingUnit* s) -> bool 
		{ return s->GetID() == id; });
	if(it == shuntingUnits.end()) return nullptr;
	return *it;
}

const ShuntingUnit* State::GetMatchingShuntingUnit(const ShuntingUnit* su) const {
	auto it = shuntingUnitStates.find(su);
	if(it != shuntingUnitStates.end()) return it->first;
	auto it2 = find_if(shuntingUnits.begin(), shuntingUnits.end(), [su](const ShuntingUnit* s) -> bool { return su->MatchesShuntingUnit(s); });
	if(it2 != shuntingUnits.end()) return *it2;
	throw runtime_error("Shunting unit " + su->toString() + " not found.");
}

bool State::IsActive() const {
	for (auto& su : shuntingUnits) {
		if (HasActiveAction(su) || IsWaiting(su)) return true;
	}
	return false;
}

bool State::IsAnyInactive() const {
	if (shuntingUnits.size() == 0) return false;
	for (auto& su : shuntingUnits) {
		if (!HasActiveAction(su) && !IsWaiting(su)) return true;
	}
	return false;
}

bool State::IsActionRequired() const {
	if(IsAnyInactive()) return true;
	for(auto inc: incomingTrains) {
		if(inc->GetTime() == GetTime()) return true;
	}
	return false;
}

void State::RemoveActiveAction(const ShuntingUnit* su, const Action* action) {
	ce(
		auto& lst = shuntingUnitStates.at(su).activeActions;
		auto it = find_if(lst.begin(), lst.end(), [action](const Action* a) -> bool { return *a == *action; } );
		if (it != lst.end()) {
			lst.erase(it);
		}
	)
}

void State::AddTasksToTrains(const unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals>& tasks) {
	for (auto& it : tasks) {
		for (auto& task : it.second) {
			AddTaskToTrain(it.first, task);
		}
	}
}

void State::RemoveTaskFromTrain(const Train* tu, const Task& task) {
	ce(
		auto& lst = trainStates.at(tu).tasks;
		auto it = find(lst.begin(), lst.end(), task);
		if (it != lst.end())
			lst.erase(it);
	)
}

void State::RemoveActiveTaskFromTrain(const Train* tu, const Task& task) {
	ce(
		auto& lst = trainStates.at(tu).activeTasks;
		auto it = find(lst.begin(), lst.end(), task);
		if (it != lst.end())
			lst.erase(it);
	)
}

const vector<const Track*> State::GetReservedTracks() const {
	vector<const Track*> reserved;
	for(auto& [track, trackState]: trackStates) {
		if(trackState.reserved) reserved.emplace_back(track);
	}
	return reserved;
}

int State::GetPositionOnTrack(const ShuntingUnit* su) const {
	auto& sus = GetOccupations(GetPosition(su));
	auto it = find_if(sus.begin(), sus.end(), [su](const ShuntingUnit* s) -> bool { return *su == *s; });
	return static_cast<int>(distance(sus.begin(), it));
}

bool State::CanMoveToSide(const ShuntingUnit* su, const Track* side) const {
	auto park = GetPosition(su);
	auto& sus = GetOccupations(park);
	if (park->IsASide(side))
		return sus.front() == su;
	return sus.back() == su;
}

const vector<Train> State::GetTrainUnitsInOrder(const ShuntingUnit* su) const {
	ce(
		auto& trains = su->GetTrains();
		auto& suState = GetShuntingUnitState(su);
		bool frontFirst = *suState.frontTrain == trains.front();
		if(frontFirst)
			return trains;
		vector<Train> reverse (trains.rbegin(), trains.rend());
		return reverse;
	)
}

void State::SwitchFrontTrain(const ShuntingUnit* su) {
	auto front = &su->GetTrains().front();
	auto back = &su->GetTrains().back();
	SetFrontTrain(su, GetFrontTrain(su) == front ? back : front);
}

const ShuntingUnit* State::GetShuntingUnitByTrainIDs(const vector<int>& ids) const {
	#if(DEBUG)
	//Check that all train ids refer to the same shunting unit
	assert(ids.size() > 0);
	const ShuntingUnit* su = nullptr;
	for(int id: ids) {
		auto current = GetShuntingUnitByTrainID(id); 
		assert(su == nullptr || su == current);
		su = current;
	}
	#endif
	try {
		return GetShuntingUnitByTrainID(ids.at(0));
	} catch (exception& e) {
		return nullptr;
	}
}

const Incoming* State::GetIncomingByID(int id) const {
	auto it = find_if(incomingTrains.begin(), incomingTrains.end(),
		[id](const Incoming* inc) -> bool { return inc->GetID() == id;});
	if(it==incomingTrains.end()) return nullptr;
	return *it;
}

const Outgoing* State::GetOutgoingByID(int id) const {
	auto it = find_if(outgoingTrains.begin(), outgoingTrains.end(),
		[id](const Outgoing* out) -> bool { return out->GetID() == id;});
	if(it==outgoingTrains.end()) return nullptr;
	return *it;
}

void State::PrintStateInfo() const {
	cout << "|---------------------------|" << endl;
	cout << "|   State at T" << setw(6) << left << GetTime() << "        |" << endl;
	cout << "|---------------------------|" << endl;
	if(GetShuntingUnits().size() == 0)
		cout << "No shunting units on the yard." << endl;
	for(auto& [su, suState]: GetShuntingUnitStates()) {
		cout << su << ": " << endl
			<< "\twaiting\t\t=\t" << suState.waiting << endl
			<< "\tmoving\t\t=\t" << suState.moving << endl
			<< "\tbegin moving\t=\t" << suState.beginMoving << endl
			<< "\tin neutral\t=\t" << suState.inNeutral << endl
			<< "\tfront train\t=\t" << suState.frontTrain << endl
			<< "\tposition\t=\t" << suState.position << endl
			<< "\tprevious\t=\t" << suState.previous << endl
			<< "\tactive actions\t=\t" << (suState.activeActions.size() == 0 ? "None" : Join(suState.activeActions.begin(), suState.activeActions.end(), ", ")) << endl;
		for(auto& train: su->GetTrains()) {
			cout << "\t> " << train;
			auto& tasks = GetTasksForTrain(&train);
			auto& activeTasks = GetActiveTasksForTrain(&train);
			if(tasks.size() + activeTasks.size() > 0) cout << ": ";
			else cout << ": no tasks.";
			if(tasks.size() > 0) cout << "Tasks: " << Join(tasks,", ");
			if(tasks.size() > 0 && activeTasks.size() > 0) cout << " / ";
			if(activeTasks.size() > 0) cout << "Active: " << Join(activeTasks, ", ") << endl;
			cout << endl;
		}
	}
	if(shuntingUnits.size() > 0)
		cout << endl << "Track occupations:" << endl;
	for(auto& [track, trackState]: trackStates) {
		if(trackState.reserved || trackState.occupations.size() > 0) {
			cout << "\t" << track;
			if(trackState.reserved) cout << " (reserved) \t|";
			else cout << "            \t|";
			if(trackState.occupations.size() > 0) {
				//A < --- suB ( T3 - T2> ) - suA ( T1> ) ---- > B
				cout << "  A <--";
				for(auto su: trackState.occupations) {
					cout << " SU-" << su->GetID() << " ( ";
					auto trains = GetTrainUnitsInOrder(su);
					bool direction = track->IsASide(GetPrevious(su));
					auto frontTrain = GetFrontTrain(su);
					if(!direction)
						trains = vector<Train>(trains.rbegin(), trains.rend());
					for(size_t i=trains.size(); i--; ) {
						if(i == trains.size()-1 && trains[i] == *frontTrain && !direction)
							cout << "<";
						cout << trains[i].GetID();
						if(i == 0 && trains[i] == *frontTrain && direction)
							cout << ">";
						if(i>0) cout << " - ";
					}
					cout << " ) -";
				}
				cout << "-> B";
			}
			cout << endl;
		}
	}
	cout << endl;
	if(GetNumberOfEvents() == 0)
		cout << "No events in the Event Queue" << endl;
	else {
		auto evt = PeekEvent();
		cout << "Next event at T" << evt->GetTime() << ": " << evt << endl;
		cout << GetNumberOfEvents() << " remaining events." << endl;
	}
	cout << endl;

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
				cout << " (outstanding)";
			cout << endl;
		}
		cout << endl;
	}
}