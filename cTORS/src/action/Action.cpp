#include "Action.h"

int Action::newUID{ 0 };

const string SimpleAction::GetTrainsToString() const {
    return "[" + Join(trainIDs.begin(), trainIDs.end(), ", ") + "]";
}