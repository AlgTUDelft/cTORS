#include "doctest/doctest.h"
#include "Engine.h"
#include "BusinessRules.h"

namespace cTORSTest
{
	TEST_CASE("Rules Test") {
		Scenario scenario;
		
		/* Set up a small track
		b0==r0===s0==r1=======b1
		           \=r2=============b2
		*/
		Track r0("r0", TrackPartType::Railroad, 100, "rail0", false, false, false, false);
		Track r1("r1", TrackPartType::Railroad, 200, "rail1", false, false, true, false);
		Track r2("r2", TrackPartType::Railroad, 300, "rail2", false, false, true, false);
		Track s0("s0", TrackPartType::Switch, 50, "switch0", false, false, true, false);
		Track b0("b0", TrackPartType::Bumper, 10, "bumper0", false, false, true, false);
		Track b1("b1", TrackPartType::Bumper, 10, "bumper1", false, false, true, false);
		Track b2("b2", TrackPartType::Bumper, 10, "bumper2", false, false, true, false);
		r0.AssignNeighbors({&b0}, {&s0});
		r1.AssignNeighbors({&b1}, {&s0});
		r2.AssignNeighbors({&b2}, {&s0});
		s0.AssignNeighbors({&r0}, {&r1,&r2});
		b0.AssignNeighbors({&r0},{});
		b1.AssignNeighbors({&r1},{});
		b2.AssignNeighbors({&r2},{});

		vector<Track*> tracks = {&r0, &r1, &r2, &s0, &b0, &b1, &b2};
		State state(scenario, tracks);

		//Setup a two test trains
		TrainUnitType elecTrainType("ElecTrainType", 1, 100, 100, 100, 100, 100, 50, 100, "ETT", false, false, true);
		Train* elecTrain = new Train(0, &elecTrainType);
		ShuntingUnit* elecSU = new ShuntingUnit(0, {elecTrain});

		TrainUnitType nonElecTrainType("NonElecTrainType", 1, 100, 100, 100, 100, 100, 50, 100, "NETT", false, false, false);
		Train* nonElecTrain = new Train(0, &nonElecTrainType);
		ShuntingUnit* nonElecSU = new ShuntingUnit(1, {nonElecTrain});
		
		state.AddShuntingUnit(elecSU, &r1, &b1);
		state.AddShuntingUnit(nonElecSU, &r0, &b0);

		vector<const Track*> elecMove = {&r1, &s0, &r0};
		vector<const Track*> nonElecMove = {&r0, &s0, &r2};
		MoveAction elecMoveAction(elecSU, elecMove);
		MoveAction nonElecMoveAction(nonElecSU, nonElecMove);

		Config config;
		electric_move_rule emr(&config);
		CHECK(!emr.IsValid(&state, &elecMoveAction).first);
		CHECK(emr.IsValid(&state, &nonElecMoveAction).first);
		length_track_rule ltr(&config);
		CHECK(!ltr.IsValid(&state, &elecMoveAction).first);
		CHECK(ltr.IsValid(&state, &nonElecMoveAction).first);
		cout << " Executed all tests "  << endl;
	}
}
