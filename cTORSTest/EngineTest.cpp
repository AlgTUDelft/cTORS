#include "doctest/doctest.h"
#include "Engine.h"

namespace cTORSTest
{
	TEST_CASE("Engine Test") {
		LocationEngine engine("data/Demo");
		auto& tracks = engine.GetLocation().GetTracks();
		auto& scenario = engine.GetScenario("data/Demo/scenario.json");
		Track* railTrack;
		auto it  = find_if(tracks.begin(), tracks.end(),
				[](auto t) -> bool { return t->GetType()==TrackPartType::Railroad; });
		{
			REQUIRE(it!=tracks.end());
			railTrack = *it;
		}
		auto previous = railTrack->GetNeighbors().front();
		SUBCASE("Test create state") {
			auto state = engine.StartSession(scenario);
			SUBCASE("Test active actions") {
				ShuntingUnit* su = new ShuntingUnit(0, {Train(0, TrainUnitType::types.begin()->second)});
				state->AddShuntingUnit(su, railTrack, previous);
				BeginMoveAction* beginMoveAction = new BeginMoveAction(su, 25);
				state->AddActiveAction(su, beginMoveAction);
				CHECK(state->HasActiveAction(su));
				state->RemoveActiveAction(su, beginMoveAction);
				CHECK(!state->HasActiveAction(su));
			}
			SUBCASE("Test get actions") {
				engine.Step(state);
				list<const Action*> &actions = engine.GetValidActions(state);
				CHECK(actions.size() > 0);
				auto a = actions.front();
				engine.ApplyAction(state, a);
			}
			engine.EndSession(state);
		}		
	}

	TEST_CASE("Scenario test") {
		LocationEngine engine("data/Demo");
		auto& sc1 = engine.GetScenario("data/Demo/scenario.json");
		{
			Scenario sc2(sc1);
		}
		CHECK(sc1.GetIncomingTrains().front()->GetShuntingUnit()->GetTrains().front().GetType()!=nullptr);
		auto st1 = engine.StartSession(sc1);
		engine.EndSession(st1);
		CHECK(sc1.GetIncomingTrains().front()->GetShuntingUnit()->GetTrains().front().GetType()!=nullptr);
		for(int i=0; i<5; i++) {
			CAPTURE("Test " + to_string(i));
			Scenario sc3(sc1);
			auto st2 = engine.StartSession(sc3);
			engine.Step(st2);
			int counter = 0;
			while(true) {
				try{
					list<const Action*> &actions = engine.GetValidActions(st2);
					if(actions.size() == 0) break;
					auto it = actions.begin();
					advance(it, counter++ % actions.size());
					auto a = *it;
					engine.ApplyActionAndStep(st2, a);
				} catch(ScenarioFailedException& e) { break; }
			}
			engine.EndSession(st2);
			CHECK(sc1.GetIncomingTrains().front()->GetShuntingUnit()->GetTrains().front().GetType()!=nullptr);
		}
	}

	TEST_CASE("Actions test") {
		LocationEngine engine("data/Demo");
		auto& scenario = engine.GetScenario("data/Demo/scenario.json");
		auto s1 = engine.GetLocation().GetTrackByID("2");
		auto s2 = engine.GetLocation().GetTrackByID("6");
		auto r1 = engine.GetLocation().GetTrackByID("1");
		auto r2 = engine.GetLocation().GetTrackByID("4");
		auto r3 = engine.GetLocation().GetTrackByID("3");
		auto r4 = engine.GetLocation().GetTrackByID("7");
		auto r5 = engine.GetLocation().GetTrackByID("8");
		auto r6 = engine.GetLocation().GetTrackByID("11");
		auto state = engine.StartSession(scenario);
		engine.Step(state);
		auto inc1 = state->GetIncomingTrains().at(0);
		engine.ApplyAction(state, Arrive(inc1));
		auto su1 = state->GetMatchingShuntingUnit(inc1->GetShuntingUnit());
		CHECK(state->GetPosition(su1) == inc1->GetParkingTrack());
		CHECK(state->GetPrevious(su1) == inc1->GetSideTrack());
		CHECK(!state->IsInNeutral(su1));
		
		engine.Step(state);
		CHECK(!state->IsMoving(su1));
		engine.ApplyAction(state, BeginMove(su1));
		CHECK(state->IsMoving(su1));
		
		engine.Step(state);
		engine.ApplyAction(state, Move(su1, r3));
		CHECK(state->GetPosition(su1)==r3);
		CHECK(state->GetPrevious(su1)==s1);
		CHECK(!state->IsInNeutral(su1));

		engine.Step(state);
		auto inc2 = state->GetIncomingTrains().at(0);
		engine.ApplyAction(state, Arrive(inc2));
		auto su2 = state->GetMatchingShuntingUnit(inc2->GetShuntingUnit());
		CHECK(state->GetPosition(su2) == inc2->GetParkingTrack());
		CHECK(state->GetPrevious(su2) == inc2->GetSideTrack());
		CHECK(!state->IsInNeutral(su2));

		engine.Step(state);
		engine.ApplyAction(state, Wait(su2));
		CHECK(state->IsWaiting(su2));

		engine.Step(state);
		engine.ApplyAction(state, Move(su1, r5));
		CHECK(state->GetPosition(su1)==r5);
		CHECK(state->GetPrevious(su1)==s2);
		CHECK(!state->IsInNeutral(su1));

		engine.Step(state);
		CHECK(!state->IsMoving(su2));
		engine.ApplyAction(state, BeginMove(su2));
		CHECK(state->IsMoving(su2));

		engine.Step(state);
		engine.ApplyAction(state, Move(su2, r3));
		CHECK(state->GetPosition(su2)==r3);
		CHECK(state->GetPrevious(su2)==s1);
		CHECK(!state->IsInNeutral(su2));

		engine.Step(state);
		CHECK(state->IsMoving(su1));
		engine.ApplyAction(state, EndMove(su1));
		CHECK(!state->IsMoving(su1));
		CHECK(!state->IsInNeutral(su1));

		// Here su1 has arrived at rail_5, with the front train facing bumper_4
		// After the split, the front train should be unable to move, because it is stuck between
		// The bumper and the back train. 
		// The direction of the two trains should be as follows:
		// Front: same (prev: same)
		// Back: same (prev: same)
		engine.Step(state);
		auto& ids = su1->GetTrainIDs();
		auto frontID = state->GetFrontTrain(su1)->GetID();
		auto backID = su1->GetTrains().at(1 - su1->GetTrainIndexByID(frontID)).GetID();
		CHECK(su1->GetNumberOfTrains() == 2);
		engine.ApplyAction(state, Split(su1, 1));
		auto su1a = state->GetShuntingUnitByTrainID(frontID);
		auto su1b = state->GetShuntingUnitByTrainID(backID);
		CHECK(su1a->GetNumberOfTrains() == 1);
		CHECK(state->GetPosition(su1a) == r5);
		CHECK(state->GetPrevious(su1a) == s2);
		CHECK(!state->IsInNeutral(su1a));
		CHECK(state->GetPositionOnTrack(su1a) == 1);
		CHECK(su1b->GetNumberOfTrains() == 1);
		CHECK(state->GetPosition(su1b) == r5);
		CHECK(state->GetPrevious(su1b) == s2);
		CHECK(!state->IsInNeutral(su1b));
		CHECK(state->GetPositionOnTrack(su1b) == 0);

		engine.Step(state);
		engine.ApplyAction(state, Move(su2, r4));
		CHECK(state->GetPosition(su2)==r4);
		CHECK(state->GetPrevious(su2)==s2);
		CHECK(!state->IsInNeutral(su2));

		engine.Step(state);
		engine.ApplyActionAndStep(state, Setback(su2));
		engine.ApplyActionAndStep(state, BeginMove(su1b));
		engine.ApplyActionAndStep(state, Setback(su1b));
		engine.ApplyActionAndStep(state, Wait(su1a));
		engine.ApplyActionAndStep(state, EndMove(su2));
		engine.ApplyActionAndStep(state, Wait(su1a));
		engine.ApplyActionAndStep(state, Wait(su2));
		engine.ApplyActionAndStep(state, Move(su1b, r3));
		engine.ApplyActionAndStep(state, Wait(su1a));
		engine.ApplyActionAndStep(state, Wait(su2));
		engine.ApplyActionAndStep(state, Move(su1b, r1));
		engine.ApplyActionAndStep(state, Wait(su1a));
		engine.ApplyActionAndStep(state, Wait(su2));
		engine.ApplyActionAndStep(state, EndMove(su1b));
		engine.ApplyActionAndStep(state, BeginMove(su2));
		engine.ApplyActionAndStep(state, Move(su2, r3));
		engine.ApplyActionAndStep(state, Wait(su1a));
		engine.ApplyActionAndStep(state, Wait(su1b));
		engine.ApplyActionAndStep(state, Move(su2, r6));
		engine.ApplyActionAndStep(state, Wait(su1a));
		engine.ApplyActionAndStep(state, Wait(su1b));
		engine.ApplyActionAndStep(state, Exit(su1b, state->GetOutgoingTrains().at(0)));
		engine.ApplyActionAndStep(state, Wait(su1a));
		engine.ApplyActionAndStep(state, EndMove(su2));

		auto& train = su2->GetTrains().at(0);
		auto& task = state->GetTasksForTrain(&train).at(0);
		auto facility = engine.GetLocation().GetFacilities().at(0);
		engine.ApplyAction(state, Service(su2, task, train, facility));
		CHECK(state->GetTasksForTrain(&train).size() == 0);

		engine.Step(state);
		engine.ApplyActionAndStep(state, Wait(su1a));
		engine.ApplyActionAndStep(state, Wait(su1a));
		engine.ApplyActionAndStep(state, Wait(su2));
		engine.ApplyActionAndStep(state, Exit(su2, state->GetOutgoingTrains().at(0)));
		engine.ApplyActionAndStep(state, Exit(su1a, state->GetOutgoingTrains().at(0)));

		PBRun pb_run;
		engine.GetResult(state)->Serialize(engine, &pb_run);
		engine.EndSession(state);
		auto runResult = RunResult::CreateRunResult(&engine.GetLocation(), pb_run);
		CHECK(engine.EvaluatePlan(runResult->GetScenario(), runResult->GetPlan()));
		delete runResult;
	}
}
