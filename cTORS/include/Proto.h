#ifndef PROTO_H
#define PROTO_H
#include <string>
#include <vector>
#include <list>
#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>
#include "Location.pb.h"
#include "Scenario.pb.h"
#include "TrainUnitTypes.pb.h"
#include "PartialOrderSchedule.pb.h"
#include "Plan.pb.h"
#include "Run.pb.h"
using namespace std;

typedef proto_tors::Scenario PBScenario;
typedef proto_tors::Train PBTrainGoal;
typedef proto_tors::ShuntingUnit PBShuntingUnit;

typedef proto_tors::TrainUnit PBTrainUnit;
typedef proto_tors::TrainUnit PBSTrainUnit;
typedef proto_tors::TrainUnitType PBTrainUnitType;
typedef proto_tors::TaskType PBTaskType;
typedef proto_tors::PredefinedTaskType PBPredefinedTaskType;
typedef proto_tors::TaskSpec PBTask;

typedef proto_tors::MemberOfStaff PBMemberOfStaff;
typedef proto_tors::TimeInterval PBTimeInterval;

typedef proto_tors::Location PBLocation;
typedef proto_tors::Facility PBFacility;
typedef proto_tors::TrackPart PBTrack;
typedef proto_tors::TrackPartType PBTrackPartType;
typedef proto_tors::Side PBSide;

typedef proto_tors::PartialOrderSchedule PBPOSPlan;
typedef proto_tors::Match PBMatch;
typedef proto_tors::Action PBAction;
typedef proto_tors::MovementAction PBMovementAction;
typedef proto_tors::TaskAction PBTaskAction;
typedef proto_tors::BreakAction PBBreakAction;
typedef proto_tors::POSPrecedenceConstraint PBPOSPrecedenceConstraint;
typedef proto_tors::Run PBRun;

typedef google::protobuf::uint64 UInt;
template<class Item> using PBList =  google::protobuf::RepeatedPtrField<Item>;

inline vector<string> PBToStringVector(const PBList<string>& items) {
    return vector<string>(items.begin(), items.end());
}

inline list<string> PBToStringList(const PBList<string>& items) {
    return list<string>(items.begin(), items.end());
}

#endif