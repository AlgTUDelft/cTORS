#include "doctest/doctest.h"
#include "Engine.h"

namespace cTORSTest
{
	TEST_CASE("Engine Test") {
		Engine engine("data/Demo");
		auto& tracks = engine.GetLocation().GetTracks();
		auto& scenario = engine.GetScenario();
		Track* railTrack;
		auto it  = find_if(tracks.begin(), tracks.end(),
				[](auto t) -> bool { return t->GetType()==TrackPartType::Railroad; });
		{
			REQUIRE(it!=tracks.end());
			railTrack = *it;
		}
		auto previous = railTrack->GetNeighbors().front();
		SUBCASE("Test create state") {
			auto state = engine.StartSession();
			SUBCASE("Test active actions") {
				ShuntingUnit* su = new ShuntingUnit(0, {new Train(0, TrainUnitType::types.begin()->second)});
				state->AddShuntingUnit(su, railTrack, previous);
				BeginMoveAction* beginMoveAction = new BeginMoveAction(su, 25);
				state->AddActiveAction(su, beginMoveAction);
				CHECK(state->HasActiveAction(su));
				state->RemoveActiveAction(su, beginMoveAction);
				CHECK(!state->HasActiveAction(su));
			}
			SUBCASE("Test get actions") {
				list<const Action*> &actions = engine.Step(state);
				CHECK(actions.size() > 0);
				auto a = actions.front();
				engine.ApplyAction(state, a);
			}
			engine.EndSession(state);
		}		
	}

	TEST_CASE("Scenario test") {
		Engine engine("data/Demo");
		auto& sc1 = engine.GetScenario();
		{
			Scenario sc2(sc1);
		}
		CHECK(sc1.GetIncomingTrains().front()->GetShuntingUnit()->GetTrains().front()->GetType()!=nullptr);
		auto st1 = engine.StartSession(sc1);
		engine.EndSession(st1);
		CHECK(sc1.GetIncomingTrains().front()->GetShuntingUnit()->GetTrains().front()->GetType()!=nullptr);
		for(int i=0; i<5; i++) {
			CAPTURE("Test " + to_string(i));
			Scenario sc3(sc1);
			auto st2 = engine.StartSession(sc3);
			int counter = 0;
			while(true) {
				try{
					list<const Action*> &actions = engine.Step(st2);
					if(actions.size() == 0) break;
					auto it = actions.begin();
					advance(it, counter++ % actions.size());
					auto a = *it;
					engine.ApplyAction(st2, a);
				} catch(ScenarioFailedException& e) { break; }
			}
			engine.EndSession(st2);
			CHECK(sc1.GetIncomingTrains().front()->GetShuntingUnit()->GetTrains().front()->GetType()!=nullptr);
		}
	}
}
