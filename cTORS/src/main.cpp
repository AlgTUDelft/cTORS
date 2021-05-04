#include <iostream>
#include "Engine.h"

int main()
{
	LocationEngine engine("data/Demo");
	engine.CalcShortestPaths();
	State* state = engine.StartSession();
	cout << "\nBeginning of session\n";
	while (true) {
		try {
			auto& actions = engine.Step(state);
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
			engine.ApplyAction(state, a);
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