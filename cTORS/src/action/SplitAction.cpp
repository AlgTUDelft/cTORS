#include "Action.h"
#include "State.h"

void SplitAction::Start(State* state) const {
	auto su = GetShuntingUnit();
	auto& suState = state->GetShuntingUnitState(su);
	auto track = suState.position;
	auto previous = suState.previous;
	int positionOnTrack = state->GetPositionOnTrack(su);
	bool front = suState.frontTrain == su->GetTrains().front();
	bool inNeutral = suState.inNeutral;
	bool fromASide = track->IsASide(previous);
	state->RemoveShuntingUnit(su);
	state->AddShuntingUnitOnPosition(&suA, track, previous, front ? suA.GetTrains().front() : suA.GetTrains().back(), positionOnTrack);
	state->AddShuntingUnitOnPosition(&suB, track, previous, front ? suB.GetTrains().front() : suB.GetTrains().back(), positionOnTrack + (fromASide ? 1 : 0));
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
	return "SplitAction " + GetShuntingUnit()->toString() + " into " + suA.GetTrainString() + " and " +suB.GetTrainString();
}

const Action* SplitActionGenerator::Generate(const State* state, const SimpleAction& action) const {
	auto split = static_cast<const Split*>(&action);
	auto su = split->GetShuntingUnit();
	if(!state->HasShuntingUnit(su)) throw InvalidActionException("The shunting unit does not exist.");
	auto suState = state->GetShuntingUnitState(su);
	if(su->GetTrains().size() <= 1) throw InvalidActionException("The shunting unit consists of only one train unit.");
	if(suState.moving || suState.waiting || suState.HasActiveAction()) throw InvalidActionException("The shunting unit is already occupied.");
	auto duration = suState.frontTrain->GetType()->splitDuration;
	auto splitPosition = split->GetSplitIndex();
	if(splitPosition < 1 || splitPosition >= su->GetTrains().size()) throw InvalidActionException("The split index is invalid.");
	vector<const Train*> trains = copy_of(su->GetTrains());
	ShuntingUnit suA = ShuntingUnit(su->GetID(), vector<const Train*>(trains.begin(), trains.begin() + splitPosition));
	ShuntingUnit suB = ShuntingUnit(su->GetID()+1, vector<const Train*>(trains.begin()+splitPosition, trains.end()));
	return new SplitAction(su, suState.position, duration, suA, suB);
}

void SplitActionGenerator::Generate(const State* state, list<const Action*>& out) const {
	if(state->GetTime()==state->GetEndTime()) return;
	//TODO employees
	auto& sus = state->GetShuntingUnits();
	for (auto su : sus) {
		auto& suState = state->GetShuntingUnitState(su);
		auto size = su->GetTrains().size();
		if (size <= 1 || suState.moving || suState.waiting || suState.HasActiveAction()) continue;
		auto duration = suState.frontTrain->GetType()->splitDuration;
		for(int splitPosition = 1; splitPosition < size; splitPosition++) {
			out.push_back(Generate(state, Split(su, splitPosition)));
		}
	}
}