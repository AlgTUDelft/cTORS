#include "Action.h"
#include "State.h"

int Action::newUID{ 0 };

const string SimpleAction::GetTrainsToString() const {
    return "[" + Join(trainIDs.begin(), trainIDs.end(), ", ") + "]";
}

const ShuntingUnit* ActionGenerator::InitialCheck(const State* state, const vector<int>& trainIDs) const {
    auto su = state->GetShuntingUnitByTrainIDs(trainIDs);
	if(su == nullptr) throw InvalidActionException("Shunting unit with trains " + Join(trainIDs, "-") + " not found");
    auto& suState = state->GetShuntingUnitState(su);
    if(suState.HasActiveAction()) {
		throw InvalidActionException("The shunting unit " + su->toString()+ " is already active. Active Actions: " 
			+ Join(suState.activeActions.begin(), suState.activeActions.end(), ", "));
	}
    return su;
}