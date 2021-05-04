#include "Action.h"
#include "State.h"

void CombineAction::Start(State* state) const {
    auto frontSU = GetFrontShuntingUnit();
    auto rearSU = GetRearShuntingUnit();
    auto suState = state->GetShuntingUnitState(frontSU);
    auto track = suState.position;
    auto previous = suState.previous;
    auto frontTrain = suState.frontTrain;
    state->RemoveShuntingUnit(frontSU);
    state->RemoveShuntingUnit(rearSU);
    
    state->AddShuntingUnitOnPosition(&combinedSU, track, previous, frontTrain, position);
    state->SetInNeutral(&combinedSU, inNeutral);
    state->AddActiveAction(&combinedSU, this);
}

void CombineAction::Finish(State* state) const {
    state->RemoveActiveAction(&combinedSU, this);
}

const string CombineAction::toString() const {
	return "CombineAction " + GetCombinedShuntingUnit()->toString() + " from  " + suString;
}

const Action* CombineActionGenerator::Generate(const State* state, const SimpleAction& action) const {
	auto combine = static_cast<const Combine*>(&action);
	auto frontSU = state->GetShuntingUnitByTrainIDs(action.GetTrainIDs());
    auto rearSU = state->GetShuntingUnitByTrainIDs(combine->GetSecondTrainIDs());
    auto& frontTrains = frontSU->GetTrains();
    auto& rearTrains = rearSU->GetTrains();
    auto duration = state->GetFrontTrain(frontSU)->GetType()->combineDuration;
    auto track = state->GetPosition(frontSU);
    auto position = state->GetPositionOnTrack(frontSU);
    auto neutral = state->IsInNeutral(frontSU) && state->IsInNeutral(rearSU);
    vector<Train> combinedTrains(frontTrains);
    combinedTrains.insert(combinedTrains.end(), rearTrains.begin(), rearTrains.end());
    ShuntingUnit combinedSU(frontSU->GetID(), combinedTrains);
    return new CombineAction(frontSU, rearSU, combinedSU, track, duration, position, neutral);
}

void CombineActionGenerator::Generate(const State* state, list<const Action*>& out) const {
	if(state->GetTime()==state->GetEndTime()) return;
    //TODO employees
    for (auto track : location->GetTracks()) {
        auto sus = state->GetOccupations(track);
        if(sus.size() < 2) continue;
        for(auto it=sus.begin(); it!=prev(sus.end()); it++) {
            auto suA = *it;
            auto suB = *next(it);
            auto& suStateA = state->GetShuntingUnitState(suA);
            auto& suStateB = state->GetShuntingUnitState(suB);
            if(suStateA.HasActiveAction() || suStateB.HasActiveAction() || suStateA.moving || suStateB.moving || (suStateA.waiting && suStateB.waiting)) continue;
            // In case both shunting units are in neutral, they can be combined in both directions
            bool neutral = suStateA.inNeutral && suStateB.inNeutral;
            // In case both shunting units are not in neutral, they can only combine if they have the same direction
            bool notNeutral = (!suStateA.inNeutral && !suStateB.inNeutral && suStateA.previous == suStateB.previous);
            const ShuntingUnit *frontSU, *rearSU;
            
            // In case one shunting unit is in neutral, the other one becomes the front shunting unit
            if (neutral || (!suStateA.inNeutral && suStateB.inNeutral)
                || (notNeutral && track->IsASide(suStateA.previous))) {
                frontSU = suA;
                rearSU = suB;
            } else if (neutral || (suStateA.inNeutral && !suStateB.inNeutral)
                || (notNeutral && track->IsBSide(suStateA.previous))) {
                frontSU = suB;
                rearSU = suA;
            } else continue;
			out.push_back(Generate(state, Combine(frontSU, rearSU)));
        }
    }
}

const string Combine::GetSecondTrainsToString() const {
    return "[" + Join(secondTrainIDs.begin(), secondTrainIDs.end(), ", ") + "]";
}