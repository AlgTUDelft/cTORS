#include <iostream>
#include "Engine.h"

int main()
{
	LocationEngine engine("data/Demo");
	auto& scenario = engine.GetScenario("data/Demo/scenario.json");
	State* state = engine.StartSession(scenario);
	cout << "\nBeginning of session\n";
	engine.Step(state);
	while (true) {
		try {
			state->PrintStateInfo();
			auto& actions = engine.GetValidActions(state);
			cout << "[T = "  + to_string(state->GetTime()) + "]\tChoosing from " << actions.size() << " actions.\n";
			if (actions.size() == 0) break;
			const Action* a;
			if (actions.size() == 1) a = actions.front();
			else {
				int i = 0;
				for (auto a : actions) {
					cout << to_string(i) + ":\t" + a->toString() + ".\n";
					i++;
				}
				cout << "Which action?\n";
				while (!(cin >> i) || i >= actions.size()) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Invalid input.  Try again: Which action?\n";
				}
				a = *next(actions.begin(), i);
			}
			engine.ApplyActionAndStep(state, a);
		}
		catch (ScenarioFailedException e) {
			cout << "Scenario failed.\n";
			break;
		}
	}
	PBRun pb_run;
	engine.GetResult(state)->Serialize(engine, &pb_run);
	engine.EndSession(state);
	cout << "End of session\n";
	string out;
	cin >> out;
}