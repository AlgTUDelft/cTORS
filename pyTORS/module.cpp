#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Python.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "Engine.h"

#ifndef BIND_ACTION
#define BIND_ACTION(name) \
	py::class_<name, Action>(m, #name) \
	.def("__str__", &name::toString) \
	.def("__repr__", &name::toString);
#endif

#ifndef BIND_SIMPLE_ACTION
#define BIND_SIMPLE_ACTION(name) \
	py::class_<name, SimpleAction>(m, #name) \
	.def(py::init<const ShuntingUnit*>()) \
	.def(py::init<const vector<int>&>()) \
	.def("__str__", &name::toString) \
	.def("__repr__", &name::toString);
#endif


namespace py = pybind11;
using namespace std;



PYBIND11_MODULE(pyTORS, m) {
    m.doc() = "TORS implemented in C++"; 

	////////////////////////////////////
	//// ShuntingUnit               ////
	////////////////////////////////////
	py::class_<ShuntingUnit>(m, "ShuntingUnit")
		.def(py::init<int, const vector<Train>&>())
		.def_property_readonly("id", &ShuntingUnit::GetID)
		.def_property("trains", &ShuntingUnit::GetTrains, &ShuntingUnit::SetTrains, py::return_value_policy::reference, py::keep_alive<1, 2>())
		.def_property_readonly("length", &ShuntingUnit::GetLength)
		.def_property_readonly("needs_electricity", &ShuntingUnit::NeedsElectricity)
		.def_property_readonly("number_of_trains", &ShuntingUnit::GetNumberOfTrains)
		.def("get_copy", [](const ShuntingUnit& su) { return new ShuntingUnit(su); }, py::return_value_policy::take_ownership)
		.def("matches_shunting_unit", &ShuntingUnit::MatchesShuntingUnit, py::arg("su"))
		.def("__str__", &ShuntingUnit::toString)
		.def("__repr__", &ShuntingUnit::toString)
		.def("__eq__", &ShuntingUnit::operator==, py::arg("other"))
		.def("__ne__", &ShuntingUnit::operator!=, py::arg("other"));
		

	////////////////////////////////////
	//// Train                      ////
	////////////////////////////////////
	py::class_<Train>(m, "Train")
		.def(py::init<int, TrainUnitType*>())
		.def_property_readonly("id", &Train::GetID)
		.def_property_readonly("type", &Train::GetType, py::return_value_policy::reference)
		.def("get_type", &Train::GetType, py::return_value_policy::reference)
		.def("get_id", &Train::GetID)
		.def("get_copy", [](const Train& tu) { return new Train(tu); }, py::return_value_policy::take_ownership)
		.def("__str__", &Train::toString)
		.def("__repr__", &Train::toString)
		.def("__eq__", &Train::operator==, py::arg("other"))
		.def("__ne__", &Train::operator!=, py::arg("other"))
		.def("__hash__", [](const Train* train) { return TrainHash()(train); });
		

	////////////////////////////////////
	//// TrainUnitType              ////
	////////////////////////////////////
	py::class_<TrainUnitType>(m, "TrainUnitType")
		.def(py::init<const string&, int, double, int, int, int,
			int, int, int, const string&, bool, bool, bool>())
		.def_readonly("display_name", &TrainUnitType::displayName)
		.def_readonly("carriages", &TrainUnitType::carriages)
		.def_readonly("length", &TrainUnitType::length)
		.def_readonly("combine_duration", &TrainUnitType::combineDuration)
		.def_readonly("split_duration", &TrainUnitType::splitDuration)
		.def_readonly("back_norm_time", &TrainUnitType::backNormTime)
		.def_readonly("back_addition_time", &TrainUnitType::backAdditionTime)
		.def_readonly("travel_speed", &TrainUnitType::travelSpeed)
		.def_readonly("startup_time", &TrainUnitType::startUpTime)
		.def_readonly("type_prefix", &TrainUnitType::typePrefix)
		.def_readonly("needs_loco", &TrainUnitType::needsLoco)
		.def_readonly("is_loco", &TrainUnitType::isLoco)
		.def_readonly("needs_electricity", &TrainUnitType::needsElectricity)
		.def("__str__", &TrainUnitType::toString)
		.def("__repr__", &TrainUnitType::toString)
		.def("__eq__", &TrainUnitType::operator==)
		.def("__ne__", &TrainUnitType::operator!=);

	////////////////////////////////////
	//// Task                       ////
	////////////////////////////////////
	py::class_<Task>(m, "Task")
		.def(py::init<const string&, int, int, list<string>>())
		.def_readonly("task_type", &Task::taskType)
		.def_readonly("priority", &Task::priority)
		.def_readonly("duration", &Task::duration)
		.def_readonly("skills", &Task::skills, py::return_value_policy::copy)
		.def("__str__", &Task::toString)
		.def("__repr__", &Task::toString)
		.def("__eq__", &Task::operator==)
		.def("__ne__", &Task::operator!=);

	////////////////////////////////////
	//// Incoming, Outgoing         ////
	////////////////////////////////////
	py::class_<TrainGoal>(m, "TrainGoal")
		.def_property_readonly("id", &TrainGoal::GetID)
		.def_property_readonly("shunting_unit", &TrainGoal::GetShuntingUnit, py::return_value_policy::reference)
		.def_property_readonly("time", &TrainGoal::GetTime)
		.def_property_readonly("instanding", &TrainGoal::IsInstanding)
		.def_property_readonly("standing_index", &TrainGoal::GetStandingIndex)
		.def_property_readonly("side_track", &TrainGoal::GetSideTrack, py::return_value_policy::reference)
		.def_property_readonly("parking_track", &TrainGoal::GetParkingTrack, py::return_value_policy::reference);
	py::class_<Incoming, TrainGoal>(m, "Incoming")
		.def(py::init<int, const ShuntingUnit*, const Track*, const Track*, int, bool, int, unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals>>())
		.def("__str__", &Incoming::toString)
		.def("__repr__", &Incoming::toString);
	py::class_<Outgoing, TrainGoal>(m, "Outgoing")
		.def(py::init<int, const ShuntingUnit*, const Track*, const Track*, int, bool, int>())
		.def("__str__", &Outgoing::toString)
		.def("__repr__", &Outgoing::toString);

	////////////////////////////////////
	//// State                      ////
	////////////////////////////////////
	py::class_<State>(m, "State")
		.def_property_readonly("time", &State::GetTime)
		.def_property_readonly("start_time", &State::GetStartTime)
		.def_property_readonly("end_time", &State::GetEndTime)
		.def_property_readonly("incoming_trains", &State::GetIncomingTrains, py::return_value_policy::reference)
		.def_property_readonly("outgoing_trains", &State::GetOutgoingTrains, py::return_value_policy::reference)
		.def_property_readonly("shunting_units", &State::GetShuntingUnits, py::return_value_policy::reference)
		.def("peek_event", &State::PeekEvent, py::return_value_policy::reference)
		.def("get_position", &State::GetPosition, py::arg("shunting_unit"), py::return_value_policy::reference)
		.def("get_previous", &State::GetPrevious, py::arg("shunting_unit"), py::return_value_policy::reference)
		.def("get_occupations", &State::GetOccupations, py::arg("track"), py::return_value_policy::reference)
		.def("get_position_on_track", &State::GetPositionOnTrack, py::arg("shunting_unit"))
		.def("get_amount_on_track", &State::GetAmountOnTrack, py::arg("track"))
		.def("get_reserved_tracks", &State::GetReservedTracks, py::return_value_policy::move)
		.def("can_move_to_side", &State::CanMoveToSide, py::arg("shunting_unit"), py::arg("track"))
		.def("get_train_by_id", &State::GetTrainByTrainID, py::arg("train_id"), py::return_value_policy::reference)
		.def("get_shunting_unit_by_id", &State::GetShuntingUnitByTrainID, py::arg("train_id"), py::return_value_policy::reference)
		.def("get_shunting_unit_by_ids", &State::GetShuntingUnitByTrainIDs, py::arg("train_ids"), py::return_value_policy::reference)
		.def("is_moving", &State::IsMoving, py::arg("shunting_unit"))
		.def("is_waiting", &State::IsWaiting, py::arg("shunting_unit"))
		.def("is_reserved", &State::IsReserved, py::arg("track"))
		.def("is_in_neutral", &State::IsInNeutral, py::arg("shunting_unit"))
		.def("has_active_action", &State::HasActiveAction, py::arg("shunting_unit"))
		.def("get_train_units_in_order", &State::GetTrainUnitsInOrder, py::arg("shunting_unit"), py::return_value_policy::move)
		.def("get_front_train", &State::GetFrontTrain, py::arg("shunting_unit"), py::return_value_policy::reference)
		.def("get_active_actions", &State::GetActiveActions, py::return_value_policy::reference)
		.def("get_tasks_for_train", &State::GetTasksForTrain, py::arg("train"), py::return_value_policy::reference);

	////////////////////////////////////
	//// Action                     ////
	////////////////////////////////////
	py::class_<Action>(m, "Action")
		.def_property_readonly("shunting_unit", &Action::GetShuntingUnit, py::return_value_policy::reference)
		.def_property_readonly("reserved_tracks", &Action::GetReservedTracks, py::return_value_policy::reference)
		.def_property_readonly("duration", &Action::GetDuration)
		.def_property_readonly("employees", &Action::GetEmployees, py::return_value_policy::reference)
		.def("__eq__ ", &Action::operator==)
		.def("__ne__ ", &Action::operator!=)
		.def("__str__", &Action::toString)
		.def("__repr__", &Action::toString);
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
	arriveAction.def_property_readonly("destination_track", &ArriveAction::GetDestinationTrack, py::return_value_policy::reference)
		.def_property_readonly("incoming", &ArriveAction::GetIncoming, py::return_value_policy::reference);
	moveAction.def_property_readonly("destination_track", &MoveAction::GetDestinationTrack, py::return_value_policy::reference)
		.def_property_readonly("previous_track", &MoveAction::GetPreviousTrack, py::return_value_policy::reference)
		.def_property_readonly("tracks", &MoveAction::GetTracks, py::return_value_policy::reference);
	exitAction.def_property_readonly("destination_track", &ExitAction::GetDestinationTrack, py::return_value_policy::reference)
		.def_property_readonly("outgoing", &ExitAction::GetOutgoing, py::return_value_policy::reference);
	serviceAction.def_property_readonly("train", &ServiceAction::GetTrain, py::return_value_policy::reference)
		.def_property_readonly("task", &ServiceAction::GetTask, py::return_value_policy::reference)
		.def_property_readonly("facility", &ServiceAction::GetFacility, py::return_value_policy::reference);
	setbackAction.def_property_readonly("drivers", &SetbackAction::GetDrivers, py::return_value_policy::reference);
	splitAction.def_property_readonly("split_index", &SplitAction::GetSplitIndex);
	combineAction.def_property_readonly("front_shunting_unit", &CombineAction::GetFrontShuntingUnit, py::return_value_policy::reference)
		.def_property_readonly("rear_shunting_unit", &CombineAction::GetRearShuntingUnit, py::return_value_policy::reference);
	
	////////////////////////////////////
	//// Simple Action              ////
	////////////////////////////////////
	py::class_<SimpleAction>(m, "SimpleAction")
		.def_property_readonly("train_ids", &SimpleAction::GetTrainIDs, py::return_value_policy::reference)
		.def("__str__", &SimpleAction::toString)
		.def("__repr__", &SimpleAction::toString);
	BIND_SIMPLE_ACTION(BeginMove);
	BIND_SIMPLE_ACTION(EndMove);
	BIND_SIMPLE_ACTION(Wait);
	BIND_SIMPLE_ACTION(Setback);
	py::class_<Service, SimpleAction>(m, "Service")
		.def(py::init<const vector<int>&, const Task&, const Train&, int>())
		.def(py::init<const ShuntingUnit*, const Task&, const Train&, const Facility*>())
		.def(py::init<const vector<int>&, const Task&, const Train&, const Facility*>())
		.def_property_readonly("task", &Service::GetTask, py::return_value_policy::reference)
		.def_property_readonly("train", &Service::GetTrain, py::return_value_policy::reference)
		.def_property_readonly("facility_id", &Service::GetFacilityID)
		.def("__str__", &Service::toString)
		.def("__repr__", &Service::toString);
	py::class_<Arrive, SimpleAction>(m, "Arrive")
		.def(py::init<const Incoming*>())
		.def_property_readonly("incoming_id", &Arrive::GetIncomingID)
		.def("__str__", &Arrive::toString)
		.def("__repr__", &Arrive::toString);
	py::class_<Exit, SimpleAction>(m, "Exit")
		.def(py::init<const ShuntingUnit*, const Outgoing*>())
		.def(py::init<const vector<int>&, int>())
		.def_property_readonly("outgoing_id", &Exit::GetOutgoingID)
		.def("__str__", &Exit::toString)
		.def("__repr__", &Exit::toString);
	py::class_<Move, SimpleAction>(m, "Move")
		.def(py::init<const ShuntingUnit*, const Track*>())
		.def(py::init<const vector<int>&, const Track*>())
		.def(py::init<const vector<int>&, string>())
		.def_property_readonly("destination_id", &Move::GetDestinationID)
		.def("__str__", &Move::toString)
		.def("__repr__", &Move::toString);
	py::class_<Split, SimpleAction>(m, "Split")
		.def(py::init<const ShuntingUnit*, const int>())
		.def(py::init<const vector<int>&, const int>())
		.def_property_readonly("split_index", &Split::GetSplitIndex)
		.def("__str__", &Split::toString)
		.def("__repr__", &Split::toString);
	py::class_<Combine, SimpleAction>(m, "Combine")
		.def(py::init<const ShuntingUnit*, const ShuntingUnit*>())
		.def(py::init<const vector<int>&, const vector<int>&>())
		.def_property_readonly("second_train_ids", &Combine::GetSecondTrainIDs, py::return_value_policy::reference)
		.def("__str__", &Combine::toString)
		.def("__repr__", &Combine::toString);

	////////////////////////////////////
	//// Employee                   ////
	////////////////////////////////////
	py::class_<Employee>(m, "Employee")
		.def("__repr__", &Employee::toString)
		.def("__str__", &Employee::toString);

	////////////////////////////////////
	//// Location                   ////
	////////////////////////////////////
	py::class_<Location>(m, "Location")
		.def_property_readonly("track_parts", &Location::GetTracks, py::return_value_policy::reference)
		.def_property_readonly("facilities", &Location::GetFacilities, py::return_value_policy::reference)
		.def("get_track_by_id", &Location::GetTrackByID, py::arg("id"), py::return_value_policy::reference)
		.def("calc_shortest_paths", &Location::CalcShortestPaths, py::arg("byTrackType"), py::arg("trainUnitType"))
		.def("get_shortest_path", 
			[](const Location& loc, const TrainUnitType* trainType, const Track* f1, const Track* f2, const Track* t1, const Track* t2) {
				return loc.GetShortestPath(trainType, {f1,f2}, {t1, t2});
			} , py::arg("trainType"), py::arg("from_previous"), py::arg("from_track"), py::arg("to_previous"), py::arg("to_track"), py::return_value_policy::reference);
		

	py::enum_<TrackPartType>(m, "TrackPartType")
		.value("RAILROAD", TrackPartType::Railroad)
		.value("SWITCH", TrackPartType::Switch)
		.value("ENGLISH_SWITCH", TrackPartType::EnglishSwitch)
		.value("HALF_ENGLISH_SWITCH", TrackPartType::HalfEnglishSwitch)
		.value("INTERSECTION", TrackPartType::InterSection)
		.value("BUMPER", TrackPartType::Bumper)
		.value("BUILDING", TrackPartType::Building)
		.export_values();

	py::class_<Facility>(m, "Facility")
		.def_property_readonly("id", &Facility::GetID)
		.def_property_readonly("type", &Facility::GetType)
		.def_property_readonly("capacity", &Facility::GetCapacity)
		.def_property_readonly("tasks", &Facility::GetTasks, py::return_value_policy::reference)
		.def_property_readonly("tracks", &Facility::GetTracks, py::return_value_policy::reference)
		.def("is_available", &Facility::IsAvailable, py::arg("start_time"), py::arg("duration"))
		.def("executes_task", &Facility::ExecutesTask, py::arg("task"))
		.def("__repr__", &Facility::toString)
		.def("__str__", &Facility::toString);

	////////////////////////////////////
	//// Track                      ////
	////////////////////////////////////
	py::class_<Track>(m, "Track")
		.def_readonly("id", &Track::id)
		.def_readonly("type", &Track::type)
		.def_readonly("length", &Track::length)
		.def_readonly("name", &Track::name)
		.def_readonly("sawMovement_allowed", &Track::sawMovementAllowed)
		.def_readonly("parking_allowed", &Track::parkingAllowed)
		.def_readonly("is_electrified", &Track::isElectrified)
		.def_readonly("standing_allowed", &Track::standingAllowed)
		.def_property_readonly("facilities", &Track::GetFacilities, py::return_value_policy::reference)
		.def_property_readonly("neighbors", &Track::GetNeighbors, py::return_value_policy::reference)
		.def("is_a_side", &Track::IsASide, py::arg("track"))
		.def("is_b_side", &Track::IsBSide, py::arg("track"))
		.def("get_next_track_parts", &Track::GetNextTrackParts, py::arg("previous_track"), py::return_value_policy::reference)
		.def("__eq__", &Track::operator==, py::arg("track"))
		.def("__ne__", &Track::operator!=, py::arg("track"))
		.def("__repr__", &Track::toString)
		.def("__str__", &Track::toString);

	////////////////////////////////////
	//// Route                      ////
	////////////////////////////////////
	py::class_<Path>(m, "Path")
		.def_readonly("length", &Path::length)
		.def_readonly("route", &Path::route, py::return_value_policy::reference)
		.def("__repr__", &Path::toString)
		.def("__str__", &Path::toString);

	////////////////////////////////////
	//// Scenario                   ////
	////////////////////////////////////
	py::class_<Scenario>(m, "Scenario")
		.def(py::init<>())
		.def_property_readonly("start_time", &Scenario::GetStartTime)
		.def_property_readonly("end_time", &Scenario::GetEndTime)
		.def_property_readonly("number_of_trains", &Scenario::GetNumberOfTrains)
		.def_property_readonly("outgoing_trains", &Scenario::GetOutgoingTrains, py::return_value_policy::reference)
		.def_property_readonly("incoming_trains", &Scenario::GetIncomingTrains, py::return_value_policy::reference)
		.def_property_readonly("employees", &Scenario::GetEmployees, py::return_value_policy::reference)
		.def_property_readonly("disturbances", &Scenario::GetDisturbances, py::return_value_policy::reference)
		.def("set_start_time", &Scenario::SetStartTime, py::arg("start_time"))
		.def("set_end_time", &Scenario::SetEndTime, py::arg("end_time"))
		.def("set_outgoing_trains", &Scenario::SetOutgoingTrains, py::arg("incoming_trains"), py::keep_alive<1, 2>())
		.def("set_incoming_trains", &Scenario::SetIncomingTrains, py::arg("outgoing_trains"), py::keep_alive<1, 2>())
		.def("add_outgoing_train", &Scenario::AddOutgoingTrain, py::arg("outgoing_train"), py::keep_alive<1, 2>())
		.def("add_incoming_train", &Scenario::AddIncomingTrain, py::arg("incoming_train"), py::keep_alive<1, 2>())
		.def("set_employees", &Scenario::SetEmployees, py::arg("employees"), py::keep_alive<1, 2>())
		.def("add_employee", &Scenario::AddEmployee, py::arg("employee"), py::keep_alive<1, 2>())
		.def("get_disturbance_list", &Scenario::GetDisturbanceVector, py::return_value_policy::reference)
		.def("set_disturbances", &Scenario::SetDisturbances, py::arg("disturbances"), py::keep_alive<1, 2>())
		.def("add_disturbance", &Scenario::AddDisturbance, py::arg("disturbance"), py::keep_alive<1, 2>())
		.def("get_copy", [](const Scenario& scenario) { return new Scenario(scenario); }, py::return_value_policy::take_ownership);
	

	////////////////////////////////////
	//// Engine                     ////
	////////////////////////////////////
	py::class_<LocationEngine>(m, "Engine")
		.def(py::init<const std::string&>())
		.def("step", &LocationEngine::Step, py::arg("state"), py::arg("scenario") = nullptr, py::return_value_policy::reference)
		.def("get_valid_actions", &LocationEngine::GetValidActions, py::arg("state"), py::return_value_policy::reference)
		.def("apply_action", static_cast<void (LocationEngine::*)(State*, const SimpleAction&)>(&LocationEngine::ApplyAction), py::arg("state"), py::arg("action"))
		.def("apply_action", static_cast<void (LocationEngine::*)(State*, const Action*)>(&LocationEngine::ApplyAction), py::arg("state"), py::arg("action"))
		.def("generate_action", &LocationEngine::GenerateAction, py::arg("state"), py::arg("action"), py::return_value_policy::take_ownership)
		.def("start_session", 
			[](LocationEngine& e, Scenario *scenario) -> State* { 
				if (scenario == nullptr) return e.StartSession();
				else return e.StartSession(*scenario); 
		}, py::arg("scenario") = nullptr, py::return_value_policy::reference)
		.def("end_session", &LocationEngine::EndSession, py::arg("state"))
		.def("get_location", &LocationEngine::GetLocation, py::return_value_policy::reference)
		.def("get_scenario", &LocationEngine::GetScenario, py::return_value_policy::reference)
		.def("get_result", &LocationEngine::GetResult, py::arg("state"), py::return_value_policy::copy)
		.def("get_path", &LocationEngine::GetPath, py::arg("state"), py::arg("move"), py::return_value_policy::take_ownership)
		.def("import_result", &LocationEngine::ImportResult, py::arg("file_path"), py::return_value_policy::take_ownership)
		.def("calc_shortest_paths", &LocationEngine::CalcShortestPaths);

	////////////////////////////////////
	//// Event                      ////
	////////////////////////////////////
	py::enum_<EventType>(m, "EventType")
		.value("ActionFinish", EventType::ActionFinish)
		.value("IncomingTrain", EventType::IncomingTrain)
		.value("OutgoingTrain", EventType::OutgoingTrain)
		.value("DisturbanceBegin", EventType::DisturbanceBegin)
		.value("DisturbanceEnd", EventType::DisturbanceEnd)
		.value("Trigger", EventType::Trigger)
		.value("MoveUpdate", EventType::MoveUpdate)
		.export_values();
	
	py::class_<Event>(m, "Event")
		.def_property_readonly("time", &Event::GetTime)
		.def_property_readonly("type", &Event::GetType);

	////////////////////////////////////
	//// POSPlan, RunResult         ////
	////////////////////////////////////
	py::class_<POSPlan>(m, "POSPlan")
		.def("get_actions", &POSPlan::GetActions, py::return_value_policy::reference)
		.def("get_first_action", [](POSPlan& p) -> POSAction {return p.GetActions()[0]; })
		.def("serialize_to_file", &POSPlan::SerializeToFile, py::arg("engine"), py::arg("scenario"), py::arg("file_name"));

	py::class_<RunResult>(m, "RunResult")
		.def_property_readonly("scenario", &RunResult::GetScenario, py::return_value_policy::reference)
		.def_property_readonly("plan", &RunResult::GetPlan, py::return_value_policy::reference)
		.def("get_actions", &RunResult::GetActions, py::return_value_policy::reference)
		.def("serialize_to_file", &RunResult::SerializeToFile, py::arg("engine"), py::arg("file_name"));

	////////////////////////////////////
	//// POSAction                  ////
	////////////////////////////////////
	py::class_<POSAction>(m, "POSAction")
		.def(py::init<int, int, int, const SimpleAction*>())
		.def_property_readonly("id", &POSAction::GetID)
		.def_property_readonly("suggested_start", &POSAction::GetSuggestedStart)
		.def_property_readonly("suggested_end", &POSAction::GetSuggestedEnd)
		.def_property_readonly("minimum_duration", &POSAction::GetMinimumDuration)
		.def_property_readonly("action", &POSAction::GetAction, py::return_value_policy::reference);

	////////////////////////////////////
	//// Exceptions                 ////
	////////////////////////////////////
	py::register_exception<ScenarioFailedException>(m, "ScenarioFailedError");
	py::register_exception<InvalidActionException>(m, "InvalidActionError");
}

