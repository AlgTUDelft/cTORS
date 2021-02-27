#include "Action.h"

#ifndef ADD_GENERATOR
#define ADD_GENERATOR(name, generator)\
AddGenerator(name, new generator(config->GetActionParameters(name), location));
#endif

ActionManager::~ActionManager() {
	for(auto& [name, generator]: generators)
		delete generator;
	generators.clear();
}

void ActionManager::AddGenerators() {
	ADD_GENERATOR("arrive", ArriveActionGenerator);
	ADD_GENERATOR("exit", ExitActionGenerator)
	ADD_GENERATOR("begin_move", BeginMoveActionGenerator);
	ADD_GENERATOR("end_move", EndMoveActionGenerator);
	ADD_GENERATOR("move", MoveActionGenerator);
	ADD_GENERATOR("wait", WaitActionGenerator);
	ADD_GENERATOR("service", ServiceActionGenerator);
	ADD_GENERATOR("set_back", SetbackActionGenerator);
	ADD_GENERATOR("split", SplitActionGenerator);
	ADD_GENERATOR("combine", CombineActionGenerator);
}

void ActionManager::AddGenerator(const string& name, const ActionGenerator* generator) {
	if (config->IsGeneratorActive(name)) {
		generators[name] = generator;	
	}
}

void ActionManager::Generate(const State* state, list<const Action*>& out) const
{
	for (auto& [name, generator] : generators) {
		generator->Generate(state, out);
	}
}



