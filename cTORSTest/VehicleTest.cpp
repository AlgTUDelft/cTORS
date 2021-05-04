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

}