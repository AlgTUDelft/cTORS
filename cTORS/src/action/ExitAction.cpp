#include "Action.h"
#include "State.h"

void ExitAction::Start(State* state) const {
	const Track* parkingTrack = outgoing->GetParkingTrack();
	const Track* sideTrack = outgoing->GetSideTrack();
	const ShuntingUnit* su = GetShuntingUnit();
	if (!outgoing->IsInstanding())
		state->ReserveTracks(vector({ parkingTrack, sideTrack }));
	state->RemoveOutgoing(outgoing);
	state->AddActiveAction(su, this);
}

void ExitAction::Finish(State* state) const {
	state->RemoveActiveAction(su, this);
	if (!outgoing->IsInstanding())
		state->FreeTracks(vector({ outgoing->GetParkingTrack(), outgoing->GetSideTrack() }));
	state->RemoveShuntingUnit(su);
}

const string ExitAction::toString() const {
	return "Exit from " + su->toString() + " at " + outgoing->GetParkingTrack()->toString() + " at T" + to_string(outgoing->GetTime());
}

const Action* ExitActionGenerator::Generate(const State* state, const SimpleAction& action) const {
	auto exit = static_cast<const Exit*>(&action);
	auto su = state->GetShuntingUnitByTrainIDs(action.GetTrainIDs());
	auto out = state->GetOutgoingByID(exit->GetOutgoingID());
	return new ExitAction(su, 0, out);
}

void ExitActionGenerator::Generate(const State* state, list<const Action*>& out) const {
	auto& outgoing = state->GetOutgoingTrains();
	if (outgoing.size() == 0) return;
	int minIndex = outgoing.at(0)->GetStandingIndex();
	for (auto ou : outgoing) minIndex = (ou->GetStandingIndex() < minIndex) ? ou->GetStandingIndex() : minIndex;
	for (auto ou : outgoing) {
		if (ou->IsInstanding()) {
			if (state->GetTime() < state->GetEndTime()) continue;
			if (ou->GetStandingIndex() > minIndex) continue;
		} else if (state->GetTime() < ou->GetTime()) continue;
		for (const auto& [su, suState] : state->GetShuntingUnitStates()) {
			if (suState.HasActiveAction()) continue;
			if (su->GetNumberOfTrains() != ou->GetShuntingUnit()->GetNumberOfTrains()) continue;
			if (suState.position != ou->GetParkingTrack()) continue;
			out.push_back(Generate(state, Exit(su, ou)));
		}
	}
}