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
	Event* event = new Event(in->GetTime(), nullptr, EventType::IncomingTrain);
	events.push(event);
	debug_out("Push event " << event->toString() << " at T=" << to_string(event->GetTime()));
}

void State::AddEvent(const Outgoing *out) {
	Event* event = new Event(out->GetTime(), nullptr, EventType::OutgoingTrain);
	events.push(event);
	debug_out("Push event " << event->toString() << " at T=" << to_string(event->GetTime()));
}

void State::AddEvent(const Action* action) {
	Event* event = new Event(time + action->GetDuration(), action, EventType::ActionFinish);
	events.push(event);
	debug_out("Push event " << event->toString() << " at T=" << to_string(event->GetTime()));
}

void State::StartAction(const Action* action) {
	if (action == nullptr) return;
	changed = true;
	action->Start(this);
	AddEvent(action);
}

void State::FinishAction(const Action* action) {
	if (action == nullptr) return;
	changed = true;
	action->Finish(this);
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
	auto it2 = find_if(shuntingUnits.begin(), shuntingUnits.end(), [su](const ShuntingUnit* s) -> bool { return *su == *s; });
	return (it2 != shuntingUnits.end());
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

void State::RemoveActiveAction(const ShuntingUnit* su, const Action* action) {
	auto& lst = shuntingUnitStates.at(su).activeActions;
	auto it = find_if(lst.begin(), lst.end(), [action](const Action* a) -> bool { return *a == *action; } );
	if (it != lst.end()) {
		lst.erase(it);
	}
}

void State::AddTasksToTrains(const unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals>& tasks) {
	for (auto& it : tasks) {
		for (auto& task : it.second) {
			AddTaskToTrain(it.first, task);
		}
	}
}

void State::RemoveTaskFromTrain(const Train* tu, const Task& task) {
	auto& lst = trainStates.at(tu).tasks;
	auto it = find(lst.begin(), lst.end(), task);
	if (it != lst.end())
		lst.erase(it);
}

void State::RemoveActiveTaskFromTrain(const Train* tu, const Task* task) {
	auto& lst = trainStates.at(tu).activeTasks;
	auto it = find(lst.begin(), lst.end(), task);
	if (it != lst.end())
		lst.erase(it);
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
	auto& trains = su->GetTrains();
	auto suState = GetShuntingUnitState(su);
	bool frontFirst = *suState.frontTrain == trains.front();
	if ((suState.previous == nullptr || suState.position->IsASide(suState.previous)) && frontFirst)
		return trains;
	vector<Train>reverse (trains.rbegin(), trains.rend());
	return reverse;
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
	return GetShuntingUnitByTrainID(ids.at(0));
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