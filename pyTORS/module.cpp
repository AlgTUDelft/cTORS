#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Python.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/iostream.h>

#include "Engine.h"
#include "docstrings.h"

#ifndef BIND_ACTION
#define BIND_ACTION(name) \
	py::class_<name, Action>(m, #name, DOC(name)) \
	.def("__str__", &name::toString, DOC(name, toString)) \
	.def("__repr__", &name::toString, DOC(name, toString));
#endif

#ifndef BIND_SIMPLE_ACTION
#define BIND_SIMPLE_ACTION(name) \
	py::class_<name, SimpleAction>(m, #name, DOC(name)) \
	.def(py::init<const ShuntingUnit*>(), DOC(SimpleAction, SimpleAction, 2)) \
	.def(py::init<const vector<int>&>(), DOC(SimpleAction, SimpleAction, 3)) \
	.def("__str__", &name::toString, DOC(name, toString)) \
	.def("__repr__", &name::toString, DOC(name, toString));
#endif


namespace py = pybind11;
using namespace std;



PYBIND11_MODULE(pyTORS, m) {
    m.doc() = "TORS implemented in C++"; 

	////////////////////////////////////
	//// ShuntingUnit               ////
	////////////////////////////////////
	py::class_<ShuntingUnit>(m, "ShuntingUnit", DOC(ShuntingUnit))
		.def(py::init<int, const vector<Train>&>(), DOC(ShuntingUnit, ShuntingUnit))
		.def_property_readonly("id", &ShuntingUnit::GetID, DOC(ShuntingUnit,GetID))
		.def_property("trains", &ShuntingUnit::GetTrains, &ShuntingUnit::SetTrains, DOC(ShuntingUnit,GetTrains), py::return_value_policy::reference, py::keep_alive<1, 2>())
		.def_property_readonly("length", &ShuntingUnit::GetLength, DOC(ShuntingUnit, GetLength))
		.def_property_readonly("needs_electricity", &ShuntingUnit::NeedsElectricity, DOC(ShuntingUnit, NeedsElectricity))
		.def_property_readonly("number_of_trains", &ShuntingUnit::GetNumberOfTrains, DOC(ShuntingUnit, GetNumberOfTrains))
		.def("get_copy", [](const ShuntingUnit& su) { return new ShuntingUnit(su); }, py::return_value_policy::take_ownership)
		.def("matches_shunting_unit", &ShuntingUnit::MatchesShuntingUnit, py::arg("su"), DOC(ShuntingUnit, MatchesShuntingUnit))
		.def("__str__", &ShuntingUnit::toString, DOC(ShuntingUnit, toString))
		.def("__repr__", &ShuntingUnit::toString, DOC(ShuntingUnit, toString))
		.def("__eq__", &ShuntingUnit::operator==, DOC(ShuntingUnit, operator_eq), py::arg("other"))
		.def("__ne__", &ShuntingUnit::operator!=, DOC(ShuntingUnit, operator_ne), py::arg("other"));
		

	////////////////////////////////////
	//// Train                      ////
	////////////////////////////////////
	py::class_<Train>(m, "Train", DOC(Train))
		.def(py::init<int, TrainUnitType*>(), DOC(Train, Train))
		.def_property("id", &Train::GetID, &Train::SetID)
		.def_property_readonly("type", &Train::GetType, DOC(Train, GetType), py::return_value_policy::reference)
		.def("get_type", &Train::GetType, DOC(Train, GetType), py::return_value_policy::reference)
		.def("get_id", &Train::GetID, DOC(Train, GetID))
		.def("get_copy", [](const Train& tu) { return new Train(tu); }, DOC(Train, Train, 4), py::return_value_policy::take_ownership)
		.def("__str__", &Train::toString, DOC(Train, toString))
		.def("__repr__", &Train::toString, DOC(Train, toString))
		.def("__eq__", &Train::operator==, py::arg("other"), DOC(Train, operator_eq))
		.def("__ne__", &Train::operator!=, py::arg("other"), DOC(Train, operator_ne))
		.def("__hash__", [](const Train* train) { return TrainHash()(train); });
		

	////////////////////////////////////
	//// TrainUnitType              ////
	////////////////////////////////////
	py::class_<TrainUnitType>(m, "TrainUnitType", DOC(TrainUnitType))
		.def(py::init<const string&, int, double, int, int, int,
			int, int, int, const string&, bool, bool, bool>(), DOC(TrainUnitType, TrainUnitType))
		.def_readonly("display_name", &TrainUnitType::displayName, DOC(TrainUnitType, displayName))
		.def_readonly("carriages", &TrainUnitType::carriages, DOC(TrainUnitType, carriages))
		.def_readonly("length", &TrainUnitType::length, DOC(TrainUnitType, length))
		.def_readonly("combine_duration", &TrainUnitType::combineDuration, DOC(TrainUnitType, combineDuration))
		.def_readonly("split_duration", &TrainUnitType::splitDuration, DOC(TrainUnitType, splitDuration))
		.def_readonly("back_norm_time", &TrainUnitType::backNormTime, DOC(TrainUnitType, backNormTime))
		.def_readonly("back_addition_time", &TrainUnitType::backAdditionTime, DOC(TrainUnitType, backAdditionTime))
		.def_readonly("travel_speed", &TrainUnitType::travelSpeed, DOC(TrainUnitType, travelSpeed))
		.def_readonly("startup_time", &TrainUnitType::startUpTime, DOC(TrainUnitType, startUpTime))
		.def_readonly("type_prefix", &TrainUnitType::typePrefix, DOC(TrainUnitType, typePrefix))
		.def_readonly("needs_loco", &TrainUnitType::needsLoco, DOC(TrainUnitType, needsLoco))
		.def_readonly("is_loco", &TrainUnitType::isLoco, DOC(TrainUnitType, isLoco))
		.def_readonly("needs_electricity", &TrainUnitType::needsElectricity, DOC(TrainUnitType, needsElectricity))
		.def("__str__", &TrainUnitType::toString, DOC(TrainUnitType, toString))
		.def("__repr__", &TrainUnitType::toString, DOC(TrainUnitType, toString))
		.def("__eq__", &TrainUnitType::operator==, DOC(TrainUnitType, operator_eq))
		.def("__ne__", &TrainUnitType::operator!=, DOC(TrainUnitType, operator_ne));

	////////////////////////////////////
	//// Task                       ////
	////////////////////////////////////
	py::class_<Task>(m, "Task", DOC(Task))
		.def(py::init<const string&, int, int, list<string>>(), DOC(Task, Task, 2))
		.def_readonly("task_type", &Task::taskType, DOC(Task, taskType))
		.def_readonly("priority", &Task::priority, DOC(Task, priority))
		.def_readonly("duration", &Task::duration, DOC(Task, duration))
		.def_readonly("skills", &Task::skills, DOC(Task, skills), py::return_value_policy::copy)
		.def("__str__", &Task::toString, DOC(Task, toString))
		.def("__repr__", &Task::toString, DOC(Task, toString))
		.def("__eq__", &Task::operator==, DOC(Task, operator_eq))
		.def("__ne__", &Task::operator!=, DOC(Task, operator_ne));

	////////////////////////////////////
	//// Incoming, Outgoing         ////
	////////////////////////////////////
	py::class_<TrainGoal>(m, "TrainGoal", DOC(TrainGoal))
		.def_property_readonly("id", &TrainGoal::GetID, DOC(TrainGoal, GetID))
		.def_property_readonly("shunting_unit", &TrainGoal::GetShuntingUnit, DOC(TrainGoal, GetShuntingUnit), py::return_value_policy::reference)
		.def_property_readonly("time", &TrainGoal::GetTime, DOC(TrainGoal, GetTime))
		.def_property_readonly("instanding", &TrainGoal::IsInstanding, DOC(TrainGoal, IsInstanding))
		.def_property_readonly("standing_index", &TrainGoal::GetStandingIndex, DOC(TrainGoal, GetStandingIndex))
		.def_property_readonly("side_track", &TrainGoal::GetSideTrack, DOC(TrainGoal, GetSideTrack), py::return_value_policy::reference)
		.def_property_readonly("parking_track", &TrainGoal::GetParkingTrack, DOC(TrainGoal, GetParkingTrack), py::return_value_policy::reference);
	py::class_<Incoming, TrainGoal>(m, "Incoming", DOC(Incoming))
		.def(py::init<int, const ShuntingUnit*, const Track*, const Track*,
				int, bool, int, unordered_map<const Train*, 
				vector<Task>, TrainHash, TrainEquals>>(), DOC(Incoming, Incoming, 2))
		.def("__str__", &Incoming::toString, DOC(Incoming, toString))
		.def("__repr__", &Incoming::toString, DOC(Incoming, toString));
	py::class_<Outgoing, TrainGoal>(m, "Outgoing", DOC(Outgoing))
		.def(py::init<int, const ShuntingUnit*, const Track*, const Track*,
				int, bool, int>(), DOC(Outgoing, Outgoing, 2))
		.def("__str__", &Outgoing::toString, DOC(Outgoing, toString))
		.def("__repr__", &Outgoing::toString, DOC(Outgoing, toString));

	////////////////////////////////////
	//// State                      ////
	////////////////////////////////////
	py::class_<State>(m, "State", DOC(State))
		.def_property_readonly("time", &State::GetTime, DOC(State, GetTime))
		.def_property_readonly("start_time", &State::GetStartTime, DOC(State, GetStartTime))
		.def_property_readonly("end_time", &State::GetEndTime, DOC(State, GetEndTime))
		.def_property_readonly("incoming_trains", &State::GetIncomingTrains, DOC(State, GetIncomingTrains), py::return_value_policy::reference)
		.def_property_readonly("outgoing_trains", &State::GetOutgoingTrains, DOC(State, GetOutgoingTrains), py::return_value_policy::reference)
		.def_property_readonly("shunting_units", &State::GetShuntingUnits, DOC(State, GetShuntingUnits), py::return_value_policy::reference)
		.def("peek_event", &State::PeekEvent, DOC(State, PeekEvent), py::return_value_policy::reference)
		.def("get_position", &State::GetPosition, DOC(State, GetPosition), py::arg("shunting_unit"), py::return_value_policy::reference)
		.def("get_previous", &State::GetPrevious, DOC(State, GetPrevious), py::arg("shunting_unit"), py::return_value_policy::reference)
		.def("get_occupations", &State::GetOccupations, DOC(State, GetOccupations), py::arg("track"), py::return_value_policy::reference)
		.def("get_position_on_track", &State::GetPositionOnTrack, DOC(State, GetPositionOnTrack), py::arg("shunting_unit"))
		.def("get_amount_on_track", &State::GetAmountOnTrack, DOC(State, GetAmountOnTrack), py::arg("track"))
		.def("get_reserved_tracks", &State::GetReservedTracks, DOC(State, GetReservedTracks), py::return_value_policy::move)
		.def("can_move_to_side", &State::CanMoveToSide, DOC(State, CanMoveToSide), py::arg("shunting_unit"), py::arg("track"))
		.def("get_train_by_id", &State::GetTrainByTrainID, DOC(State, GetTrainByTrainID), py::arg("train_id"), py::return_value_policy::reference)
		.def("get_shunting_unit_by_id", &State::GetShuntingUnitByTrainID, DOC(State, GetShuntingUnitByTrainID), py::arg("train_id"), py::return_value_policy::reference)
		.def("get_shunting_unit_by_ids", &State::GetShuntingUnitByTrainIDs, DOC(State, GetShuntingUnitByTrainIDs), py::arg("train_ids"), py::return_value_policy::reference)
		.def("is_moving", &State::IsMoving, DOC(State, IsMoving), py::arg("shunting_unit"))
		.def("is_waiting", &State::IsWaiting, DOC(State, IsWaiting), py::arg("shunting_unit"))
		.def("is_reserved", &State::IsReserved, DOC(State, IsReserved), py::arg("track"))
		.def("is_in_neutral", &State::IsInNeutral, DOC(State, IsInNeutral), py::arg("shunting_unit"))
		.def("has_active_action", &State::HasActiveAction, DOC(State, HasActiveAction), py::arg("shunting_unit"))
		.def("get_train_units_in_order", &State::GetTrainUnitsInOrder, DOC(State, GetTrainUnitsInOrder), py::arg("shunting_unit"), py::return_value_policy::move)
		.def("get_front_train", &State::GetFrontTrain, DOC(State, GetFrontTrain), py::arg("shunting_unit"), py::return_value_policy::reference)
		.def("get_active_actions", &State::GetActiveActions, DOC(State, GetActiveActions), py::arg("shunting_unit"), py::return_value_policy::reference)
		.def("get_tasks_for_train", &State::GetTasksForTrain, DOC(State, GetTasksForTrain), py::arg("train"), py::return_value_policy::reference)
		.def("print_state_info", &State::PrintStateInfo, DOC(State, PrintStateInfo),
			py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect>());

	////////////////////////////////////
	//// Action                     ////
	////////////////////////////////////
	py::class_<Action>(m, "Action", DOC(Action))
		.def_property_readonly("shunting_unit", &Action::GetShuntingUnit, DOC(Action, GetShuntingUnit), py::return_value_policy::reference)
		.def_property_readonly("reserved_tracks", &Action::GetReservedTracks, DOC(Action, GetReservedTracks), py::return_value_policy::reference)
		.def_property_readonly("duration", &Action::GetDuration, DOC(Action, GetDuration))
		.def_property_readonly("employees", &Action::GetEmployees, DOC(Action, GetEmployees), py::return_value_policy::reference)
		.def("__eq__ ", &Action::operator==, DOC(Action, operator_eq))
		.def("__ne__ ", &Action::operator!=, DOC(Action, operator_ne))
		.def("__str__", &Action::toString, DOC(Action, toString))
		.def("__repr__", &Action::toString, DOC(Action, toString));
	auto arriveAction = BIND_ACTION(ArriveAction);
	BIND_ACTION(BeginMoveAction);
	BIND_ACTION(EndMoveAction);
	auto moveAction = BIND_ACTION(MoveAction);
	auto exitAction = BIND_ACTION(ExitAction);
	auto combineAction = BIND_ACTION(CombineAction);
	auto splitAction = BIND_ACTION(SplitAction);
	auto serviceAction = BIND_ACTION(ServiceAction);
	auto setbackAction = BIND_ACTION(SetbackAction);
	BIND_ACTION(WaitAction);
	arriveAction.def_property_readonly("destination_track", &ArriveAction::GetDestinationTrack, DOC(ArriveAction, GetDestinationTrack), py::return_value_policy::reference)
		.def_property_readonly("incoming", &ArriveAction::GetIncoming, DOC(ArriveAction, GetIncoming), py::return_value_policy::reference);
	moveAction.def_property_readonly("destination_track", &MoveAction::GetDestinationTrack, DOC(MoveAction, GetDestinationTrack), py::return_value_policy::reference)
		.def_property_readonly("previous_track", &MoveAction::GetPreviousTrack, DOC(MoveAction, GetPreviousTrack), py::return_value_policy::reference)
		.def_property_readonly("tracks", &MoveAction::GetTracks, DOC(MoveAction, GetTracks), py::return_value_policy::reference);
	exitAction.def_property_readonly("destination_track", &ExitAction::GetDestinationTrack, DOC(ExitAction, GetDestinationTrack), py::return_value_policy::reference)
		.def_property_readonly("outgoing", &ExitAction::GetOutgoing, DOC(ExitAction, GetOutgoing), py::return_value_policy::reference);
	serviceAction.def_property_readonly("train", &ServiceAction::GetTrain, DOC(ServiceAction, GetTrain), py::return_value_policy::reference)
		.def_property_readonly("task", &ServiceAction::GetTask, DOC(ServiceAction, GetTask), py::return_value_policy::reference)
		.def_property_readonly("facility", &ServiceAction::GetFacility, DOC(ServiceAction, GetFacility), py::return_value_policy::reference);
	setbackAction.def_property_readonly("drivers", &SetbackAction::GetDrivers, DOC(SetbackAction, GetDrivers), py::return_value_policy::reference);
	splitAction.def_property_readonly("split_index", &SplitAction::GetSplitIndex, DOC(SplitAction, GetSplitIndex));
	combineAction.def_property_readonly("front_shunting_unit", &CombineAction::GetFrontShuntingUnit, DOC(CombineAction, GetFrontShuntingUnit), py::return_value_policy::reference)
		.def_property_readonly("rear_shunting_unit", &CombineAction::GetRearShuntingUnit, DOC(CombineAction, GetRearShuntingUnit), py::return_value_policy::reference);
	
	////////////////////////////////////
	//// Simple Action              ////
	////////////////////////////////////
	py::class_<SimpleAction>(m, "SimpleAction", DOC(SimpleAction))
		.def_property_readonly("train_ids", &SimpleAction::GetTrainIDs, DOC(SimpleAction, GetTrainIDs), py::return_value_policy::reference)
		.def("__str__", &SimpleAction::toString, DOC(SimpleAction, GetTrainIDs))
		.def("__repr__", &SimpleAction::toString, DOC(SimpleAction, GetTrainIDs));
	BIND_SIMPLE_ACTION(BeginMove);
	BIND_SIMPLE_ACTION(EndMove);
	BIND_SIMPLE_ACTION(Wait);
	BIND_SIMPLE_ACTION(Setback);
	py::class_<Service, SimpleAction>(m, "Service", DOC(Service))
		.def(py::init<const vector<int>&, const Task&, const Train&, int>(), DOC(Service, Service, 2))
		.def(py::init<const ShuntingUnit*, const Task&, const Train&, const Facility*>(), DOC(Service, Service, 4))
		.def(py::init<const vector<int>&, const Task&, const Train&, const Facility*>(), DOC(Service, Service, 3))
		.def_property_readonly("task", &Service::GetTask, DOC(Service, GetTask), py::return_value_policy::reference)
		.def_property_readonly("train", &Service::GetTrain, DOC(Service, GetTrain), py::return_value_policy::reference)
		.def_property_readonly("facility_id", &Service::GetFacilityID, DOC(Service, GetFacilityID))
		.def("__str__", &Service::toString, DOC(Service, toString))
		.def("__repr__", &Service::toString, DOC(Service, toString));
	py::class_<Arrive, SimpleAction>(m, "Arrive", DOC(Arrive))
		.def(py::init<const Incoming*>(), DOC(Arrive, Arrive, 2))
		.def_property_readonly("incoming_id", &Arrive::GetIncomingID, DOC(Arrive, GetIncomingID))
		.def("__str__", &Arrive::toString, DOC(Arrive, toString))
		.def("__repr__", &Arrive::toString, DOC(Arrive, toString));
	py::class_<Exit, SimpleAction>(m, "Exit", DOC(Exit))
		.def(py::init<const ShuntingUnit*, const Outgoing*>(), DOC(Exit, Exit, 3))
		.def(py::init<const vector<int>&, int>(), DOC(Exit, Exit, 2))
		.def_property_readonly("outgoing_id", &Exit::GetOutgoingID, DOC(Exit, GetOutgoingID))
		.def("__str__", &Exit::toString, DOC(Exit, toString))
		.def("__repr__", &Exit::toString, DOC(Exit, toString));
	py::class_<Move, SimpleAction>(m, "Move", DOC(Move))
		.def(py::init<const ShuntingUnit*, const Track*>(), DOC(Move, Move, 4))
		.def(py::init<const vector<int>&, const Track*>(), DOC(Move, Move, 3))
		.def(py::init<const vector<int>&, string>(), DOC(Move, Move, 2))
		.def_property_readonly("destination_id", &Move::GetDestinationID, DOC(Move, GetDestinationID))
		.def("__str__", &Move::toString, DOC(Move, toString))
		.def("__repr__", &Move::toString, DOC(Move, toString));
	py::class_<Split, SimpleAction>(m, "Split", DOC(Split))
		.def(py::init<const ShuntingUnit*, const int>(), DOC(Split, Split, 3))
		.def(py::init<const vector<int>&, const int>(), DOC(Split, Split, 2))
		.def_property_readonly("split_index", &Split::GetSplitIndex, DOC(Split, GetSplitIndex))
		.def("__str__", &Split::toString, DOC(Split, toString))
		.def("__repr__", &Split::toString, DOC(Split, toString));
	py::class_<Combine, SimpleAction>(m, "Combine", DOC(Combine))
		.def(py::init<const ShuntingUnit*, const ShuntingUnit*>(), DOC(Combine, Combine, 3))
		.def(py::init<const vector<int>&, const vector<int>&>(), DOC(Combine, Combine, 2))
		.def_property_readonly("second_train_ids", &Combine::GetSecondTrainIDs, DOC(Combine, GetSecondTrainIDs), py::return_value_policy::reference)
		.def("__str__", &Combine::toString, DOC(Combine, toString))
		.def("__repr__", &Combine::toString, DOC(Combine, toString));

	////////////////////////////////////
	//// Employee                   ////
	////////////////////////////////////
	py::class_<Employee>(m, "Employee", DOC(Employee))
		.def("__repr__", &Employee::toString, DOC(Employee, toString))
		.def("__str__", &Employee::toString, DOC(Employee, toString));

	////////////////////////////////////
	//// Location                   ////
	////////////////////////////////////
	py::class_<Location>(m, "Location", DOC(Location))
		.def_property_readonly("track_parts", &Location::GetTracks, DOC(Location, GetTracks), py::return_value_policy::reference)
		.def_property_readonly("facilities", &Location::GetFacilities, DOC(Location, GetFacilities), py::return_value_policy::reference)
		.def("get_track_by_id", &Location::GetTrackByID, py::arg("id"), DOC(Location, GetTrackByID), py::return_value_policy::reference)
		.def("calc_all_possible_paths", &Location::CalcAllPossiblePaths, DOC(Location, CalcAllPossiblePaths))
		.def("calc_shortest_paths", &Location::CalcShortestPaths, DOC(Location, CalcShortestPaths), py::arg("trainUnitType"))
		.def("get_distance", py::overload_cast<const vector<const Track*>&>(&Location::GetDistance, py::const_), DOC(Location, GetDistance), py::arg("tracks"))
		.def("get_shortest_path", 
			[](const Location& loc, const TrainUnitType* trainType, const Track* f1, const Track* f2, const Track* t1, const Track* t2) {
				return loc.GetShortestPath(trainType, {f1,f2}, {t1, t2});
			} , DOC(Location, GetShortestPath), py::arg("trainType"), py::arg("from_previous"), py::arg("from_track"), py::arg("to_previous"), py::arg("to_track"), py::return_value_policy::reference);
		

	py::enum_<TrackPartType>(m, "TrackPartType", DOC(TrackPartType))
		.value("RAILROAD", TrackPartType::Railroad, DOC(TrackPartType, Railroad))
		.value("SWITCH", TrackPartType::Switch, DOC(TrackPartType, Switch))
		.value("ENGLISH_SWITCH", TrackPartType::EnglishSwitch, DOC(TrackPartType, EnglishSwitch))
		.value("HALF_ENGLISH_SWITCH", TrackPartType::HalfEnglishSwitch, DOC(TrackPartType, HalfEnglishSwitch))
		.value("INTERSECTION", TrackPartType::InterSection, DOC(TrackPartType, InterSection))
		.value("BUMPER", TrackPartType::Bumper, DOC(TrackPartType, Bumper))
		.value("BUILDING", TrackPartType::Building, DOC(TrackPartType, Building))
		.export_values();

	py::class_<Facility>(m, "Facility", DOC(Facility))
		.def_property_readonly("id", &Facility::GetID, DOC(Facility, GetID))
		.def_property_readonly("type", &Facility::GetType, DOC(Facility, GetType))
		.def_property_readonly("capacity", &Facility::GetCapacity, DOC(Facility, GetCapacity))
		.def_property_readonly("tasks", &Facility::GetTasks, DOC(Facility, GetTasks), py::return_value_policy::reference)
		.def_property_readonly("tracks", &Facility::GetTracks, DOC(Facility, GetTracks), py::return_value_policy::reference)
		.def("is_available", &Facility::IsAvailable, DOC(Facility, IsAvailable), py::arg("start_time"), py::arg("duration"))
		.def("executes_task", &Facility::ExecutesTask, DOC(Facility, ExecutesTask), py::arg("task"))
		.def("__repr__", &Facility::toString, DOC(Facility, toString))
		.def("__str__", &Facility::toString, DOC(Facility, toString));

	////////////////////////////////////
	//// Track                      ////
	////////////////////////////////////
	py::class_<Track>(m, "Track", DOC(Track))
		.def_readonly("id", &Track::id, DOC(Track, id))
		.def_readonly("type", &Track::type, DOC(Track, type))
		.def_readonly("length", &Track::length, DOC(Track, length))
		.def_readonly("name", &Track::name, DOC(Track, name))
		.def_readonly("sawMovement_allowed", &Track::sawMovementAllowed, DOC(Track, sawMovementAllowed))
		.def_readonly("parking_allowed", &Track::parkingAllowed, DOC(Track, parkingAllowed))
		.def_readonly("is_electrified", &Track::isElectrified, DOC(Track, isElectrified))
		.def_property_readonly("facilities", &Track::GetFacilities, DOC(Track, GetFacilities), py::return_value_policy::reference)
		.def_property_readonly("neighbors", &Track::GetNeighbors, DOC(Track, GetNeighbors), py::return_value_policy::reference)
		.def_property_readonly("a_side_tracks", &Track::GetASideTracks, DOC(Track, GetASideTracks), py::return_value_policy::reference)
		.def_property_readonly("b_side_tracks", &Track::GetBSideTracks, DOC(Track, GetBSideTracks), py::return_value_policy::reference)
		.def("get_opposite_side", &Track::GetOppositeSide, DOC(Track, GetOppositeSide), py::arg("track"), py::return_value_policy::reference)
		.def("is_a_side", &Track::IsASide, DOC(Track, IsASide), py::arg("track"))
		.def("is_b_side", &Track::IsBSide, DOC(Track, IsBSide), py::arg("track"))
		.def("is_same_side", &Track::IsSameSide, DOC(Track, IsSameSide), py::arg("track_1"), py::arg("track_2"))
		.def("is_neighbor", &Track::IsNeighbor, DOC(Track, IsNeighbor), py::arg("track"))
		.def("get_next_track_parts", &Track::GetNextTrackParts, DOC(Track, GetNextTrackParts), py::arg("previous_track"), py::return_value_policy::reference)
		.def("__eq__", &Track::operator==, DOC(Track, operator_eq), py::arg("track"))
		.def("__ne__", &Track::operator!=, DOC(Track, operator_ne), py::arg("track"))
		.def("__repr__", &Track::toString, DOC(Track, toString))
		.def("__str__", &Track::toString, DOC(Track, toString));

	////////////////////////////////////
	//// Route                      ////
	////////////////////////////////////
	py::class_<Path>(m, "Path", DOC(Path))
		.def_readonly("length", &Path::length, DOC(Path, length))
		.def_readonly("route", &Path::route, DOC(Path, route), py::return_value_policy::reference)
		.def("__repr__", &Path::toString, DOC(Path, toString))
		.def("__str__", &Path::toString, DOC(Path, toString));

	////////////////////////////////////
	//// Scenario                   ////
	////////////////////////////////////
	py::class_<Scenario>(m, "Scenario", DOC(Scenario))
		.def(py::init<>())
		.def_property_readonly("start_time", &Scenario::GetStartTime, DOC(Scenario, GetStartTime))
		.def_property_readonly("end_time", &Scenario::GetEndTime, DOC(Scenario, GetEndTime))
		.def_property_readonly("number_of_trains", &Scenario::GetNumberOfTrains, DOC(Scenario, GetNumberOfTrains))
		.def_property_readonly("outgoing_trains", &Scenario::GetOutgoingTrains, DOC(Scenario, GetOutgoingTrains), py::return_value_policy::reference)
		.def_property_readonly("incoming_trains", &Scenario::GetIncomingTrains, DOC(Scenario, GetIncomingTrains), py::return_value_policy::reference)
		.def_property_readonly("employees", &Scenario::GetEmployees, DOC(Scenario, GetEmployees), py::return_value_policy::reference)
		.def_property_readonly("disturbances", &Scenario::GetDisturbances, DOC(Scenario, GetDisturbances), py::return_value_policy::reference)
		.def("set_start_time", &Scenario::SetStartTime, DOC(Scenario, SetStartTime), py::arg("start_time"))
		.def("set_end_time", &Scenario::SetEndTime, DOC(Scenario, SetEndTime), py::arg("end_time"))
		.def("set_outgoing_trains", &Scenario::SetOutgoingTrains, DOC(Scenario, SetOutgoingTrains), py::arg("incoming_trains"), py::keep_alive<1, 2>())
		.def("set_incoming_trains", &Scenario::SetIncomingTrains, DOC(Scenario, SetIncomingTrains), py::arg("outgoing_trains"), py::keep_alive<1, 2>())
		.def("add_outgoing_train", &Scenario::AddOutgoingTrain, DOC(Scenario, AddOutgoingTrain), py::arg("outgoing_train"), py::keep_alive<1, 2>())
		.def("add_incoming_train", &Scenario::AddIncomingTrain, DOC(Scenario, AddIncomingTrain), py::arg("incoming_train"), py::keep_alive<1, 2>())
		.def("set_employees", &Scenario::SetEmployees, DOC(Scenario, SetEmployees), py::arg("employees"), py::keep_alive<1, 2>())
		.def("add_employee", &Scenario::AddEmployee, DOC(Scenario, AddEmployee), py::arg("employee"), py::keep_alive<1, 2>())
		.def("get_disturbance_list", &Scenario::GetDisturbanceVector, DOC(Scenario, GetDisturbanceVector), py::return_value_policy::reference)
		.def("set_disturbances", &Scenario::SetDisturbances, DOC(Scenario, SetDisturbances), py::arg("disturbances"), py::keep_alive<1, 2>())
		.def("add_disturbance", &Scenario::AddDisturbance, DOC(Scenario, AddDisturbance), py::arg("disturbance"), py::keep_alive<1, 2>())
		.def("print_scenario_info", &Scenario::PrintScenarioInfo, DOC(Scenario, PrintScenarioInfo),
			py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect>())
		.def("get_copy", [](const Scenario& scenario) { return new Scenario(scenario); }, DOC(Scenario, Scenario, 4), py::return_value_policy::take_ownership);
	

	////////////////////////////////////
	//// Engine                     ////
	////////////////////////////////////
	py::class_<LocationEngine>(m, "Engine", DOC(LocationEngine))
		.def(py::init<const std::string&>(), DOC(LocationEngine, LocationEngine, 2))
		.def("step", &LocationEngine::Step, DOC(LocationEngine, Step), py::arg("state"),
			py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect>())
		.def("get_valid_actions", &LocationEngine::GetValidActions, DOC(LocationEngine, GetValidActions), py::arg("state"), py::return_value_policy::reference)
		.def("apply_action", py::overload_cast<State*, const SimpleAction&>(&LocationEngine::ApplyAction), DOC(LocationEngine, ApplyAction), py::arg("state"), py::arg("action"),
			py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect>())
		.def("apply_action", py::overload_cast<State*, const Action*>(&LocationEngine::ApplyAction), DOC(LocationEngine, ApplyAction), py::arg("state"), py::arg("action"),
			py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect>())
		.def("apply_action_and_step", py::overload_cast<State*, const SimpleAction&>(&LocationEngine::ApplyActionAndStep), DOC(LocationEngine, ApplyActionAndStep),
			py::arg("state"), py::arg("action"), py::return_value_policy::reference,
			py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect>())
		.def("apply_action_and_step", py::overload_cast<State*, const Action*>(&LocationEngine::ApplyActionAndStep), DOC(LocationEngine, ApplyActionAndStep),
			py::arg("state"), py::arg("action"), py::return_value_policy::reference,
			py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect>())
		.def("apply_wait_all_until", &LocationEngine::ApplyWaitAllUntil, DOC(LocationEngine, ApplyWaitAllUntil), py::arg("state"), py::arg("time"),
			py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect>())
		.def("generate_action", &LocationEngine::GenerateAction, DOC(LocationEngine, GenerateAction), py::arg("state"), py::arg("action"), py::return_value_policy::take_ownership,
			py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect>())
		.def("is_valid_action", py::overload_cast<const State*, const SimpleAction&>(&LocationEngine::IsValidAction, py::const_), DOC(LocationEngine, IsValidAction), py::arg("state"), py::arg("action"),
			py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect>())
		.def("is_valid_action", py::overload_cast<const State*, const Action*>(&LocationEngine::IsValidAction, py::const_), DOC(LocationEngine, IsValidAction), py::arg("state"), py::arg("action"),
			py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect>())
		.def("is_state_active", &LocationEngine::IsStateActive, DOC(LocationEngine, IsStateActive), py::arg("state"),
			py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect>())
		.def("start_session", &LocationEngine::StartSession, DOC(LocationEngine, StartSession), py::arg("scenario"), py::return_value_policy::reference)
		.def("end_session", &LocationEngine::EndSession, DOC(LocationEngine, EndSession), py::arg("state"))
		.def("get_location", &LocationEngine::GetLocation, DOC(LocationEngine, GetLocation), py::return_value_policy::reference)
		.def("get_scenario", &LocationEngine::GetScenario, DOC(LocationEngine, GetScenario), py::arg("file_path"), py::return_value_policy::reference)
		.def("get_result", &LocationEngine::GetResult, DOC(LocationEngine, GetResult), py::arg("state"), py::return_value_policy::copy)
		.def("get_path", &LocationEngine::GetPath, DOC(LocationEngine, GetPath), py::arg("state"), py::arg("move"), py::return_value_policy::take_ownership)
		.def("import_result", &LocationEngine::ImportResult, DOC(LocationEngine, ImportResult), py::arg("file_path"), py::return_value_policy::take_ownership)
		.def("calc_all_possible_paths", &LocationEngine::CalcAllPossiblePaths, DOC(LocationEngine, CalcAllPossiblePaths),
			py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect>())
		.def("calc_shortest_paths", &LocationEngine::CalcShortestPaths, DOC(LocationEngine, CalcShortestPaths),
			py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect>());

	////////////////////////////////////
	//// Event                      ////
	////////////////////////////////////
	py::enum_<EventType>(m, "EventType", DOC(EventType))
		.value("ActionFinish", EventType::ActionFinish, DOC(EventType, ActionFinish))
		.value("IncomingTrain", EventType::IncomingTrain, DOC(EventType, IncomingTrain))
		.value("OutgoingTrain", EventType::OutgoingTrain, DOC(EventType, OutgoingTrain))
		.value("DisturbanceBegin", EventType::DisturbanceBegin, DOC(EventType, DisturbanceBegin))
		.value("DisturbanceEnd", EventType::DisturbanceEnd, DOC(EventType, DisturbanceEnd))
		.value("Trigger", EventType::Trigger, DOC(EventType, Trigger))
		.value("MoveUpdate", EventType::MoveUpdate, DOC(EventType, MoveUpdate))
		.export_values();
	
	py::class_<Event>(m, "Event", DOC(Event))
		.def_property_readonly("time", &Event::GetTime, DOC(Event, GetTime))
		.def_property_readonly("type", &Event::GetType, DOC(Event, GetType));

	////////////////////////////////////
	//// POSPlan, RunResult         ////
	////////////////////////////////////
	py::class_<POSPlan>(m, "POSPlan", DOC(POSPlan))
		.def("get_actions", &POSPlan::GetActions, DOC(POSPlan, GetActions), py::return_value_policy::reference)
		.def("get_first_action", [](POSPlan& p) -> POSAction {return p.GetActions()[0]; })
		.def("serialize_to_file", &POSPlan::SerializeToFile, DOC(POSPlan, SerializeToFile), py::arg("engine"), py::arg("scenario"), py::arg("file_name"));

	py::class_<RunResult>(m, "RunResult", DOC(RunResult))
		.def_property_readonly("scenario", &RunResult::GetScenario, DOC(RunResult, GetScenario), py::return_value_policy::reference)
		.def_property_readonly("plan", &RunResult::GetPlan, DOC(RunResult, GetPlan), py::return_value_policy::reference)
		.def("get_actions", &RunResult::GetActions, DOC(RunResult, GetActions), py::return_value_policy::reference)
		.def("serialize_to_file", &RunResult::SerializeToFile, DOC(RunResult, SerializeToFile), py::arg("engine"), py::arg("file_name"));

	////////////////////////////////////
	//// POSAction                  ////
	////////////////////////////////////
	py::class_<POSAction>(m, "POSAction", DOC(POSAction))
		.def(py::init<int, int, int, const SimpleAction*>(), DOC(POSAction, POSAction, 2))
		.def_property_readonly("id", &POSAction::GetID, DOC(POSAction, GetID))
		.def_property_readonly("suggested_start", &POSAction::GetSuggestedStart, DOC(POSAction, GetSuggestedStart))
		.def_property_readonly("suggested_end", &POSAction::GetSuggestedEnd, DOC(POSAction, GetSuggestedEnd))
		.def_property_readonly("minimum_duration", &POSAction::GetMinimumDuration, DOC(POSAction, GetMinimumDuration))
		.def_property_readonly("action", &POSAction::GetAction, DOC(POSAction, GetAction), py::return_value_policy::reference);

	////////////////////////////////////
	//// Exceptions                 ////
	////////////////////////////////////
	py::register_exception<ScenarioFailedException>(m, "ScenarioFailedError");
	py::register_exception<InvalidActionException>(m, "InvalidActionError");
	py::register_exception<InvalidStateRequest>(m, "InvalidStateRequestError");
}

