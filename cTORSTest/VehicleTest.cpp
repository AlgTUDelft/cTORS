#include "doctest/doctest.h"
#include "Train.h"

namespace cTORSTest {
    TEST_CASE("Train Test") {
        TrainUnitType elecTrainType("ElecTrainType", 1, 100, 100, 100, 100, 100, 50, 100, "ETT", false, false, true);
        TrainUnitType nonElecTrainType("NonElecTrainType", 1, 100, 100, 100, 100, 100, 50, 100, "NETT", false, false, false);
        CHECK(nonElecTrainType != elecTrainType);
        Train elecTrain(0, &elecTrainType);
        Train nonElecTrain(1, &nonElecTrainType);
        CHECK(elecTrain != nonElecTrain);
        CHECK(elecTrain == Train(elecTrain));
        CHECK(Train(-1, &nonElecTrainType) != Train(-1, &nonElecTrainType));

        TrainUnitType::types[elecTrainType.displayName] = &elecTrainType;
        TrainUnitType::types[nonElecTrainType.displayName] = &nonElecTrainType;

        //TODO test shunting units, length, needsElectric?
    }

    TEST_CASE("Json Vehicle Test") {
        json jtt;
        jtt["displayName"] = "SLT4";
        jtt["carriages"] = 4;
        jtt["length"] = 70;
        jtt["combineDuration"] = "180";
        jtt["splitDuration"] = "120";
        jtt["backNormTime"] = "120";
        jtt["backAdditionTime"] = "16";
        jtt["travelSpeed"] = "0";
        jtt["startUpTime"] = "0";
        jtt["typePrefix"] = "SLT";
        jtt["needsLoco"] = false;
        jtt["isLoco"] = false;
        jtt["needsElectricity"] = true;
        TrainUnitType tut;
        jtt.get_to(tut);
        TrainUnitType::types[tut.displayName] = &tut;
        
        CHECK(tut.displayName=="SLT4");
        CHECK(tut.carriages==4);
        CHECK(tut.length==70);
        CHECK(tut.combineDuration==180);
        CHECK(tut.splitDuration==120);
        CHECK(tut.backNormTime==120);
        CHECK(tut.backAdditionTime==16);
        CHECK(tut.travelSpeed==0);
        CHECK(tut.startUpTime==0);
        CHECK(tut.typePrefix=="SLT");
        CHECK(!tut.needsLoco);
        CHECK(!tut.isLoco);
        CHECK(tut.needsElectricity);

        json jt1;
		jt1["id"] = "1";
		jt1["typeDisplayName"] = "SLT4";
        Train t1;
        jt1.get_to(t1);
        CHECK(t1.GetID()==1);
        CHECK(t1.GetType()==&tut);

        json jt2;
        jt2["id"] = "****";
        jt2["typeDisplayName"] = "SLT4";
        Train t2 = jt2.get_to(t2);
        CHECK(t2.GetID()==-1);

        //TODO incoming and outgoing shunting units
    }

}