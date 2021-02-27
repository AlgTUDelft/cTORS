#include "doctest/doctest.h"
#include "nlohmann/json.hpp"
#include "Track.h"
using json = nlohmann::json;

namespace cTORSTest
{
	TEST_CASE("Track Test") {
		Track t1 = Track("r0", TrackPartType::Railroad, 100, "rail0", false, false, false, false);
		Track t1copy = Track(t1);
		CHECK(t1 == t1copy);
		Track t2 = Track("s1", TrackPartType::Switch, 50, "switch1", false, false, false, false);
		Track t3 = Track("b2", TrackPartType::Bumper, 10, "bumper2", false, false, false, false);
		Track t4 = Track("b3", TrackPartType::Bumper, 10, "bumper3", false, false, false, false);
		Track t5 = Track("b4", TrackPartType::Bumper, 10, "bumper4", false, false, false, false);
		CHECK(t1 != t2);
		CHECK(t3 != t4);
		t1.AssignNeighbors({&t3}, {&t2});
		t2.AssignNeighbors({&t1}, {&t4,&t5});
		t3.AssignNeighbors({&t1},{});
		t4.AssignNeighbors({&t2},{});
		t5.AssignNeighbors({&t2},{});
		CHECK(t2.GetNeighbors().size()==3);
		auto ns = t5.GetNeighbors();
		CHECK(ns.size()==1);
		CHECK(ns[0]==&t2);
		ns = t2.GetNextTrackParts(&t4);
		CHECK(ns.size()==1);
		CHECK(ns[0]==&t1);
		CHECK(t2.IsASide(&t1));
		CHECK(!t2.IsASide(&t5));
		CHECK(t2.IsBSide(&t4));
		CHECK(!t2.IsBSide(&t3));
	}

	TEST_CASE("Json Track Test") {
		json j;
		j["id"] = "r0";
		j["type"] = "RailRoad";
		j["length"] = 100;
		j["name"] = "rail0";
		j["sawMovementAllowed"] = true;
		j["parkingAllowed"] = true;
		j["isElectrified"] = true;
		j["standingAllowed"] = true;
		Track t;
		j.get_to(t);
		CHECK(t.id=="r0");
		CHECK(t.type==TrackPartType::Railroad);
		CHECK(t.length==100);
		CHECK(t.name=="rail0");
		CHECK(t.sawMovementAllowed);
		CHECK(t.parkingAllowed);
		CHECK(t.isElectrified);
		CHECK(t.standingAllowed);
	}
}
