#include "BusinessRules.h"

/**
 * Validates an ExitAction for the given state. 
 * The ExitAction is invalid iff
 * 1. The Outgoing ShuntingUnit has at least one Train that has an optional service Task that is not yet performed.
 * @return A pair describing 1) whether the action is valid, and 2) if not, why
 */
pair<bool, string> optional_service_task_rule::IsValid(const State* state, const Action* action) const {
	if (auto ea = dynamic_cast<const ExitAction*>(action)) {
		auto su = ea->GetShuntingUnit();
		for (auto& tu : su->GetTrains()) {
			auto& tasks = state->GetTasksForTrain(&tu);
			for (auto& task : tasks) {
				if (task.priority != 0)
					return make_pair(false, "Shunting unit " + su->toString() + " cannot leave, because train " + tu.toString() + "  still has the following optional unfinished task: " + task.toString() + ".");
			}
		}
	}
	return make_pair(true, "");
}

