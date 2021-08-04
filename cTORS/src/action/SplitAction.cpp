#include "Action.h"
#include "State.h"

void SplitAction::Start(State* state) const {
	auto su = GetShuntingUnit();
	auto& suState = state->GetShuntingUnitState(su);
	auto track = suState.position;
	auto previous = suState.previous;
	int positionOnTrack = state->GetPositionOnTrack(su);
	bool front = *suState.frontTrain == su->GetTrains().front();
	bool inNeutral = suState.inNeutral;
	bool fromASide = track->IsASide(previous);
	state->RemoveShuntingUnit(su);
	/*
	Suppose a SU T1-T2-T3, with front train T1, and split-index 1
	resulting in suA: T1, with front train T1. suB: T2-T3, with front train T2
	The track setup is as follows: A < --- SU ( T3 - T2 - T1> ) ---- > B
	This means it previous track is at the Aside, but the front train is directed to the Bside.
	In this case front = true, fromASide = true
	The resulting setup must be: A < --- suB ( T3 - T2> ) - suA ( T1> ) ---- > B
	*/
	state->AddShuntingUnitOnPosition(&suA, track, previous, front ? &suA.GetTrains().front() : &suA.GetTrains().back(), positionOnTrack);
	state->AddShuntingUnitOnPosition(&suB, track, previous, front ? &suB.GetTrains().front() : &suB.GetTrains().back(), positionOnTrack+ (fromASide ? 0 : 1));
	state->SetInNeutral(&suA, inNeutral);
	state->SetInNeutral(&suB, inNeutral);

	state->AddActiveAction(&suA, this);
	state->AddActiveAction(&suB, this);
}

void SplitAction::Finish(State* state) const {
	state->RemoveActiveAction(&suA, this);
	state->RemoveActiveAction(&suB, this);
}

const string SplitAction::toString() const {
	return "Split " + suString + " into " + suA.GetTrainString() + " and " +suB.GetTrainString();
}

const Action* SplitActionGenerator::Generate(const State* state, const SimpleAction& action) const {
	auto split = static_cast<const Split*>(&action);
	auto su = InitialCheck(state, action);
	auto& suState = state->GetShuntingUnitState(su);
	if(su->GetTrains().size() <= 1) throw InvalidActionException("The shunting unit consists of only one train unit.");
	if(suState.moving || suState.waiting) throw InvalidActionException("The shunting unit is already occupied.");
	auto duration = suState.frontTrain->GetType()->splitDuration;
	auto splitPosition = split->GetSplitIndex();
	if(splitPosition < 1 || splitPosition >= su->GetTrains().size()) throw InvalidActionException("The split index is invalid.");
	const vector<Train> trains = state->GetTrainUnitsInOrder(su);
	ShuntingUnit suA = ShuntingUnit(su->GetID(), vector<Train>(trains.begin(), trains.begin() + splitPosition));
	int suBid = su->GetID() + 1;
	while(state->HasShuntingUnitByID(suBid)) suBid++;
	ShuntingUnit suB = ShuntingUnit(suBid, vector<Train>(trains.begin()+splitPosition, trains.end()));
	return new SplitAction(su, suState.position, duration, suA, suB);
}

void SplitActionGenerator::Generate(const State* state, list<const Action*>& out) const {
	if(state->GetTime()==state->GetEndTime()) return;
	//TODO employees
	for (const auto& [su, suState] : state->GetShuntingUnitStates()) {
		auto size = su->GetTrains().size();
		if (size <= 1 || suState.moving || suState.waiting || suState.HasActiveAction()) continue;
		auto duration = suState.frontTrain->GetType()->splitDuration;
		for(int splitPosition = 1; splitPosition < size; splitPosition++) {
			out.push_back(Generate(state, Split(su, splitPosition)));
		}
	}
}