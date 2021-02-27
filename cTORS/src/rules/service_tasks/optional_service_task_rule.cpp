#include "BusinessRules.h"

/*

Rule that verifies that all optional service tasks are performed before a shunting unit leaves the shunting yard.

*/

pair<bool, string> optional_service_task_rule::IsValid(const State* state, const Action* action) const {
	if (auto ea = dynamic_cast<const ExitAction*>(action)) {
		auto su = ea->GetShuntingUnit();
		for (auto tu : su->GetTrains()) {
			auto& tasks = state->GetTasksForTrain(tu);
			for (auto& task : tasks) {
				if (task.priority != 0)
					return make_pair(false, "Shunting unit " + su->toString() + " cannot leave, because train " + tu->toString() + "  still has the following optional unfinished task: " + task.toString() + ".");
			}
		}
	}
	return make_pair(true, "");
}

