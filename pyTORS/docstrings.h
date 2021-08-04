/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#define __EXPAND(x)                                      x
#define __COUNT(_1, _2, _3, _4, _5, _6, _7, COUNT, ...)  COUNT
#define __VA_SIZE(...)                                   __EXPAND(__COUNT(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1))
#define __CAT1(a, b)                                     a ## b
#define __CAT2(a, b)                                     __CAT1(a, b)
#define __DOC1(n1)                                       __doc_##n1
#define __DOC2(n1, n2)                                   __doc_##n1##_##n2
#define __DOC3(n1, n2, n3)                               __doc_##n1##_##n2##_##n3
#define __DOC4(n1, n2, n3, n4)                           __doc_##n1##_##n2##_##n3##_##n4
#define __DOC5(n1, n2, n3, n4, n5)                       __doc_##n1##_##n2##_##n3##_##n4##_##n5
#define __DOC6(n1, n2, n3, n4, n5, n6)                   __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6
#define __DOC7(n1, n2, n3, n4, n5, n6, n7)               __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7
#define DOC(...)                                         __EXPAND(__EXPAND(__CAT2(__DOC, __VA_SIZE(__VA_ARGS__)))(__VA_ARGS__))

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif


static const char *__doc_Action =
R"doc(The abstract base class for action descriptions. Action describes an
action more extensively than SimpleAction. It it therefore used to
check for validity and to execute.

SimpleActions can be transferred into Actions by using
Engine::GenerateAction or LocationEngine::GenerateAction)doc";

static const char *__doc_Action_2 = R"doc()doc";

static const char *__doc_ActionGenerator =
R"doc(An abstract base class for the ActionGenerators.

An ActionGenerator generates Actions based on a State.)doc";

static const char *__doc_ActionGenerator_ActionGenerator = R"doc()doc";

static const char *__doc_ActionGenerator_ActionGenerator_2 = R"doc()doc";

static const char *__doc_ActionGenerator_ActionGenerator_3 =
R"doc(Construct this ActionGenerator based on the parameters defined in the
json object */)doc";

static const char *__doc_ActionGenerator_Generate =
R"doc(Generate actions given the State and store the result in the out list
*/)doc";

static const char *__doc_ActionGenerator_Generate_2 = R"doc(Generate an Action object from a SimpleAction object given a State */)doc";

static const char *__doc_ActionGenerator_InitialCheck =
R"doc(Run an initial error check on the action and return the ShuntingUnit
from the State */)doc";

static const char *__doc_ActionGenerator_InitialCheck_2 =
R"doc(Run an initial error check on ShuntingUnit described the train IDs and
return the ShuntingUnit from the State */)doc";

static const char *__doc_ActionGenerator_location = R"doc(< a reference to the Location object */)doc";

static const char *__doc_ActionManager =
R"doc(An ActionManager contains all the ActionGenerators and
ActionValidators

The ActionManager is used to generate actions and to use the
configured business rules to filter valid actions.)doc";

static const char *__doc_ActionManager_ActionManager = R"doc()doc";

static const char *__doc_ActionManager_ActionManager_2 = R"doc()doc";

static const char *__doc_ActionManager_ActionManager_3 = R"doc(Construct the ActionManager from the given Config object */)doc";

static const char *__doc_ActionManager_AddGenerator = R"doc()doc";

static const char *__doc_ActionManager_AddGenerators = R"doc()doc";

static const char *__doc_ActionManager_AddValidators = R"doc()doc";

static const char *__doc_ActionManager_Generate =
R"doc(Generate valid Action%s given the State and store the result in the
out list */)doc";

static const char *__doc_ActionManager_GetGenerator = R"doc(Get the ActionGenerator based on its name */)doc";

static const char *__doc_ActionManager_IsValid =
R"doc(Check the validity of an Action in a certain State

Returns:
    a tuple (bool, string) describing 1. Whether the Action is valid
    2. If not valid, why)doc";

static const char *__doc_ActionManager_config = R"doc()doc";

static const char *__doc_ActionManager_generatorMap = R"doc()doc";

static const char *__doc_ActionManager_generators = R"doc()doc";

static const char *__doc_ActionManager_location = R"doc()doc";

static const char *__doc_ActionManager_validators = R"doc()doc";

static const char *__doc_Action_Action = R"doc()doc";

static const char *__doc_Action_Action_2 = R"doc(Construct an Action from the given parameters */)doc";

static const char *__doc_Action_Action_3 = R"doc(Construct an Action from the given parameters */)doc";

static const char *__doc_Action_Action_4 = R"doc(Construct an Action from the given parameters */)doc";

static const char *__doc_Action_Action_5 = R"doc(Construct an Action from the given parameters */)doc";

static const char *__doc_Action_Action_6 = R"doc(Default copy constructor */)doc";

static const char *__doc_Action_Clone = R"doc(Create a copy of this action using new)doc";

static const char *__doc_Action_CreateSimple = R"doc(Generate the SimpleAction equivalent of this action.)doc";

static const char *__doc_Action_Finish = R"doc(End the execution of the action and modify the state accordingly.)doc";

static const char *__doc_Action_GetDuration = R"doc(Get the duration of this action.)doc";

static const char *__doc_Action_GetEmployees =
R"doc(Get the employees that are involved with this action (employees is not
yet implemented).)doc";

static const char *__doc_Action_GetReservedTracks = R"doc(Get the Tracks that are reserved by this action.)doc";

static const char *__doc_Action_GetShuntingUnit = R"doc(Get the ShuntingUnit that this action operates on.)doc";

static const char *__doc_Action_IsEqual = R"doc(Check if two actions are equal by comparing their unique id.)doc";

static const char *__doc_Action_Start = R"doc(Start the execution action and modify the state accordingly.)doc";

static const char *__doc_Action_duration = R"doc(< The duration in seconds for this Action to complete */)doc";

static const char *__doc_Action_employees = R"doc(< The Employees involved in this Action (not yet implemented) */)doc";

static const char *__doc_Action_operator_eq = R"doc(Check if two actions are equal by comparing their unique id.)doc";

static const char *__doc_Action_operator_ne = R"doc(Check if two actions are not equal by comparing their unique id.)doc";

static const char *__doc_Action_reserved = R"doc(< A list of tracks that are reserved during this Action */)doc";

static const char *__doc_Action_su = R"doc(< The ShuntingUnit that this Action operates on */)doc";

static const char *__doc_Action_toString = R"doc(Generate a string representation of this action.)doc";

static const char *__doc_Action_uid = R"doc(< The unique id of this Action */)doc";

static const char *__doc_Arrive =
R"doc(The Arrive action lets a scheduled ShuntingUnit arrive on the shunting
yard.)doc";

static const char *__doc_ArriveAction =
R"doc(The ArriveAction lets a scheduled ShuntingUnit arrive on the shunting
yard.)doc";

static const char *__doc_ArriveActionGenerator = R"doc(The ArriveActionGenerator generates ArriveAction%s */)doc";

static const char *__doc_ArriveActionGenerator_ArriveActionGenerator = R"doc()doc";

static const char *__doc_ArriveActionGenerator_ArriveActionGenerator_2 = R"doc()doc";

static const char *__doc_ArriveActionGenerator_ArriveActionGenerator_3 = R"doc()doc";

static const char *__doc_ArriveActionGenerator_Generate = R"doc()doc";

static const char *__doc_ArriveActionGenerator_Generate_2 = R"doc()doc";

static const char *__doc_ArriveAction_ArriveAction = R"doc()doc";

static const char *__doc_ArriveAction_ArriveAction_2 = R"doc(Construct an ArriveAction from the given parameters */)doc";

static const char *__doc_ArriveAction_ArriveAction_3 = R"doc(Construct an ArriveAction from the given parameters */)doc";

static const char *__doc_ArriveAction_Clone = R"doc()doc";

static const char *__doc_ArriveAction_CreateSimple = R"doc()doc";

static const char *__doc_ArriveAction_Finish = R"doc()doc";

static const char *__doc_ArriveAction_GetDestinationTrack = R"doc(Get the destination Track for this Incoming ShuntingUnit */)doc";

static const char *__doc_ArriveAction_GetIncoming = R"doc(Get the Incoming event for this ArriveAction */)doc";

static const char *__doc_ArriveAction_Start = R"doc()doc";

static const char *__doc_ArriveAction_incoming = R"doc()doc";

static const char *__doc_ArriveAction_toString = R"doc()doc";

static const char *__doc_Arrive_Arrive = R"doc()doc";

static const char *__doc_Arrive_Arrive_2 = R"doc(Construct an Arrive action from the Incoming event */)doc";

static const char *__doc_Arrive_Arrive_3 = R"doc(Default copy constructor */)doc";

static const char *__doc_Arrive_Clone = R"doc()doc";

static const char *__doc_Arrive_GetGeneratorName = R"doc()doc";

static const char *__doc_Arrive_GetIncomingID = R"doc(Get the id of the Incoming event */)doc";

static const char *__doc_Arrive_incomingID = R"doc()doc";

static const char *__doc_Arrive_toString = R"doc()doc";

static const char *__doc_BeginMove =
R"doc(The BeginMove action changes the state of a ShuntingUnit to moving.
After this action the ShuntingUnit can be moved.)doc";

static const char *__doc_BeginMoveAction =
R"doc(The BeginMoveAction changes the state of a ShuntingUnit to moving.
After this action the ShuntingUnit can be moved.)doc";

static const char *__doc_BeginMoveAction_BeginMoveAction = R"doc()doc";

static const char *__doc_BeginMoveAction_BeginMoveAction_2 = R"doc(Construct a BeginMoveAction from the given parameters */)doc";

static const char *__doc_BeginMoveAction_Clone = R"doc()doc";

static const char *__doc_BeginMoveAction_CreateSimple = R"doc()doc";

static const char *__doc_BeginMoveAction_Finish = R"doc()doc";

static const char *__doc_BeginMoveAction_Start = R"doc()doc";

static const char *__doc_BeginMoveAction_toString = R"doc()doc";

static const char *__doc_BeginMove_BeginMove = R"doc()doc";

static const char *__doc_BeginMove_BeginMove_2 = R"doc()doc";

static const char *__doc_BeginMove_BeginMove_3 = R"doc()doc";

static const char *__doc_BeginMove_BeginMove_4 = R"doc()doc";

static const char *__doc_BeginMove_Clone = R"doc()doc";

static const char *__doc_BeginMove_GetGeneratorName = R"doc()doc";

static const char *__doc_BeginMove_toString = R"doc()doc";

static const char *__doc_BusinessRule = R"doc()doc";

static const char *__doc_BusinessRule_2 =
R"doc(Abstract base class for all the business rules. The ActionValidator
class uses these rules to validate actions)doc";

static const char *__doc_BusinessRule_BusinessRule = R"doc()doc";

static const char *__doc_BusinessRule_BusinessRule_2 = R"doc(Construct this BusinessRule using the given Config object */)doc";

static const char *__doc_BusinessRule_IsValid =
R"doc(Validate the action for the given state.

Returns:
    A pair describing 1) whether the action is valid, and 2) if not,
    why)doc";

static const char *__doc_BusinessRule_config = R"doc()doc";

static const char *__doc_Combine = R"doc(The Combine action combines two ShuntingUnits into one ShuntingUnit.)doc";

static const char *__doc_CombineAction = R"doc(The CombineAction combines two ShuntingUnits into one ShuntingUnit.)doc";

static const char *__doc_CombineActionGenerator = R"doc(The CombineActionGenerator generates CombineAction%s */)doc";

static const char *__doc_CombineActionGenerator_CombineActionGenerator = R"doc()doc";

static const char *__doc_CombineActionGenerator_CombineActionGenerator_2 = R"doc()doc";

static const char *__doc_CombineActionGenerator_CombineActionGenerator_3 = R"doc()doc";

static const char *__doc_CombineActionGenerator_Generate = R"doc()doc";

static const char *__doc_CombineActionGenerator_Generate_2 = R"doc()doc";

static const char *__doc_CombineAction_Clone = R"doc()doc";

static const char *__doc_CombineAction_CombineAction = R"doc()doc";

static const char *__doc_CombineAction_CombineAction_2 =
R"doc(Generate a CombineAction from the given parameters

The inNeutral parameter describes whether the resulting ShuntingUnit
will be in neutral direction. The position parameter is the index of
the resulting ShuntingUnit on the Track.)doc";

static const char *__doc_CombineAction_CreateSimple = R"doc()doc";

static const char *__doc_CombineAction_Finish = R"doc()doc";

static const char *__doc_CombineAction_GetCombinedShuntingUnit = R"doc(Get the combined ShuntingUnit */)doc";

static const char *__doc_CombineAction_GetFrontShuntingUnit = R"doc(Get the front ShuntingUnit to be combined */)doc";

static const char *__doc_CombineAction_GetRearShuntingUnit = R"doc(Get the rear ShuntingUnit to be combined */)doc";

static const char *__doc_CombineAction_Start = R"doc()doc";

static const char *__doc_CombineAction_combinedSU = R"doc()doc";

static const char *__doc_CombineAction_inNeutral = R"doc()doc";

static const char *__doc_CombineAction_position = R"doc()doc";

static const char *__doc_CombineAction_rearSU = R"doc()doc";

static const char *__doc_CombineAction_suString = R"doc()doc";

static const char *__doc_CombineAction_toString = R"doc()doc";

static const char *__doc_Combine_Clone = R"doc()doc";

static const char *__doc_Combine_Combine = R"doc()doc";

static const char *__doc_Combine_Combine_2 =
R"doc(Construct a Combine action for the two shunting units described by the
train ids. */)doc";

static const char *__doc_Combine_Combine_3 =
R"doc(Construct a Combine action for the two shunting units su and
secondShuntingUnit */)doc";

static const char *__doc_Combine_Combine_4 = R"doc(Default copy constructor */)doc";

static const char *__doc_Combine_GetGeneratorName = R"doc()doc";

static const char *__doc_Combine_GetSecondTrainIDs = R"doc(Get the IDs of the second ShuntingUnit */)doc";

static const char *__doc_Combine_GetSecondTrainsToString = R"doc(Get a string representation of the second ShuntingUnit */)doc";

static const char *__doc_Combine_secondTrainIDs = R"doc()doc";

static const char *__doc_Combine_toString = R"doc()doc";

static const char *__doc_Config = R"doc(The Config class describes the configuration for the Engine)doc";

static const char *__doc_Config_Config = R"doc(Construct a default Config object */)doc";

static const char *__doc_Config_Config_2 = R"doc(Construct a Config object based on the given json file */)doc";

static const char *__doc_Config_GetActionParameters = R"doc(Get the parameters for the ActionGenerator defined by the name */)doc";

static const char *__doc_Config_ImportActionRules = R"doc()doc";

static const char *__doc_Config_ImportBusinessRules = R"doc()doc";

static const char *__doc_Config_IsBusinessRuleActive = R"doc(Returns true iff the BusinessRule defined by the name is active */)doc";

static const char *__doc_Config_IsGeneratorActive = R"doc(Returns true iff the ActionGenerator defined by the name is active */)doc";

static const char *__doc_Config_actionParams = R"doc()doc";

static const char *__doc_Config_actionRules = R"doc()doc";

static const char *__doc_Config_businessRules = R"doc()doc";

static const char *__doc_ConvertPBTaskType = R"doc(//!\cond NO_DOC)doc";

static const char *__doc_ConvertPBTaskTypes = R"doc()doc";

static const char *__doc_ConvertPBTrackPartType = R"doc(Convert a protobuf track_part_type to a TrackPartType)doc";

static const char *__doc_Disturbance =
R"doc(The Disturbance class describes an (unexpected) distrubance that can
happen during a Scenario

(not yet implemented))doc";

static const char *__doc_Disturbance_Disturbance = R"doc(The default constructor */)doc";

static const char *__doc_Employee = R"doc(An Employee on the shunting yard (not yet implemented))doc";

static const char *__doc_Employee_AssignTracks = R"doc(Assign the start and (required) end location of this Employee */)doc";

static const char *__doc_Employee_Employee = R"doc()doc";

static const char *__doc_Employee_Employee_2 = R"doc(Construct an Employee object from the given parameters */)doc";

static const char *__doc_Employee_Employee_3 = R"doc(Construct an Employee object from the given protobuf object */)doc";

static const char *__doc_Employee_Employee_4 = R"doc(Default copy constructor */)doc";

static const char *__doc_Employee_breakDuration = R"doc()doc";

static const char *__doc_Employee_breakWindows = R"doc()doc";

static const char *__doc_Employee_canMoveTrains = R"doc()doc";

static const char *__doc_Employee_endLocation = R"doc()doc";

static const char *__doc_Employee_id = R"doc()doc";

static const char *__doc_Employee_name = R"doc()doc";

static const char *__doc_Employee_operator_eq = R"doc(Two Employee%s are equal if they have the same id */)doc";

static const char *__doc_Employee_operator_ne = R"doc(Two Employee%s are not equal if they do not have the same id */)doc";

static const char *__doc_Employee_shifts = R"doc()doc";

static const char *__doc_Employee_skills = R"doc()doc";

static const char *__doc_Employee_startLocation = R"doc()doc";

static const char *__doc_Employee_toString = R"doc(Get a string representation of this Employee */)doc";

static const char *__doc_Employee_type = R"doc()doc";

static const char *__doc_EndMove =
R"doc(The EndMove action changes the state of a ShuntingUnit to not moving.
After this action the ShuntingUnit can no longer be moved.)doc";

static const char *__doc_EndMoveAction =
R"doc(The EndMoveAction changes the state of a ShuntingUnit to not moving.
After this action the ShuntingUnit can no longer be moved.)doc";

static const char *__doc_EndMoveAction_Clone = R"doc()doc";

static const char *__doc_EndMoveAction_CreateSimple = R"doc()doc";

static const char *__doc_EndMoveAction_EndMoveAction = R"doc()doc";

static const char *__doc_EndMoveAction_EndMoveAction_2 = R"doc(Construct a EndMoveAction from the given parameters */)doc";

static const char *__doc_EndMoveAction_Finish = R"doc()doc";

static const char *__doc_EndMoveAction_Start = R"doc()doc";

static const char *__doc_EndMoveAction_toString = R"doc()doc";

static const char *__doc_EndMove_Clone = R"doc()doc";

static const char *__doc_EndMove_EndMove = R"doc()doc";

static const char *__doc_EndMove_EndMove_2 = R"doc()doc";

static const char *__doc_EndMove_EndMove_3 = R"doc()doc";

static const char *__doc_EndMove_EndMove_4 = R"doc()doc";

static const char *__doc_EndMove_GetGeneratorName = R"doc()doc";

static const char *__doc_EndMove_toString = R"doc()doc";

static const char *__doc_Engine = R"doc(The TORS Engine for several Location%s)doc";

static const char *__doc_Engine_2 = R"doc(The TORS Engine for several Location%s)doc";

static const char *__doc_Engine_ApplyAction = R"doc(Apply the given Action to the State */)doc";

static const char *__doc_Engine_ApplyAction_2 = R"doc(Apply the given SimpleAction to the State */)doc";

static const char *__doc_Engine_ApplyActionAndStep =
R"doc(Apply the Action to the State and go to the next step in the
simulation */)doc";

static const char *__doc_Engine_ApplyActionAndStep_2 =
R"doc(Apply the SimpleAction to the State and go to the next step in the
simulation */)doc";

static const char *__doc_Engine_CalcAllPossiblePaths =
R"doc(Calculate all the possible paths (run this once before requesting
possible paths) */)doc";

static const char *__doc_Engine_CalcShortestPaths =
R"doc(Calculate all the shortest paths (run this once before requesting
shortest paths) */)doc";

static const char *__doc_Engine_EndSession = R"doc(End the session that belongs to the given State */)doc";

static const char *__doc_Engine_Engine = R"doc(The default constructor */)doc";

static const char *__doc_Engine_Engine_2 = R"doc(Start the Engine with one Location as described by the given folder */)doc";

static const char *__doc_Engine_EvaluatePlan =
R"doc(Evaluate the given POSPlan for the given Scenario on the given
Location */)doc";

static const char *__doc_Engine_GenerateAction = R"doc(Generate an Action from the given SimpleAction */)doc";

static const char *__doc_Engine_GetLocation = R"doc(Get a reference to the Location of the given location string */)doc";

static const char *__doc_Engine_GetOrLoadLocationEngine = R"doc(Get or load the LocationEngine based on its file location */)doc";

static const char *__doc_Engine_GetPath = R"doc(Get a path for the Move */)doc";

static const char *__doc_Engine_GetResult = R"doc(Get the RunResult for the given State/session */)doc";

static const char *__doc_Engine_GetScenario =
R"doc(Get a reference to the Scenario given by the path and the given
location string */)doc";

static const char *__doc_Engine_GetValidActions = R"doc(Get the valid actions for the session with the given State */)doc";

static const char *__doc_Engine_ImportResult = R"doc(Import a RunResult from a protobuf file */)doc";

static const char *__doc_Engine_IsValidAction =
R"doc(Checks if the given SimpleAction is valid in the given State or not.
If not provides a reason why.)doc";

static const char *__doc_Engine_IsValidAction_2 =
R"doc(Checks if the given Action is valid in the given State or not. If not
provides a reason why.)doc";

static const char *__doc_Engine_StartSession =
R"doc(Start a session for the given Scenario and location and generate an
initial State */)doc";

static const char *__doc_Engine_Step = R"doc(Go to the next Step in the simulation and update the State */)doc";

static const char *__doc_Engine_engineMap = R"doc()doc";

static const char *__doc_Engine_engines = R"doc()doc";

static const char *__doc_Event = R"doc(A triggered Event)doc";

static const char *__doc_EventCompare = R"doc(//!\cond NO_DOC)doc";

static const char *__doc_EventCompare_operator_call = R"doc()doc";

static const char *__doc_EventQueue = R"doc(A priority queue of all the Event%s based on the time of the Event)doc";

static const char *__doc_EventType = R"doc(The EventType of an Event */)doc";

static const char *__doc_EventType_ActionFinish = R"doc(< When an Action is finished */)doc";

static const char *__doc_EventType_DisturbanceBegin = R"doc(< When a disturbance begins (not yet implemented) */)doc";

static const char *__doc_EventType_DisturbanceEnd = R"doc(< When a disturbance ends (not yet implemented) */)doc";

static const char *__doc_EventType_IncomingTrain = R"doc(< When an Incoming train arrives */)doc";

static const char *__doc_EventType_MoveUpdate = R"doc(< (not yet implemented) */)doc";

static const char *__doc_EventType_OutgoingTrain = R"doc(< When an Outgoing train should depart */)doc";

static const char *__doc_EventType_Trigger = R"doc(< (not yet implemented) */)doc";

static const char *__doc_Event_Event = R"doc(Construct an Event with type ActionFinish from the given Action */)doc";

static const char *__doc_Event_Event_2 =
R"doc(Construct an Event with type IncomingTrain from the given Incoming
object */)doc";

static const char *__doc_Event_Event_3 =
R"doc(Construct an Event with type OutgoingTrain from the given Outgoing
object */)doc";

static const char *__doc_Event_Event_4 = R"doc(Copy the Event */)doc";

static const char *__doc_Event_GetAction = R"doc(Get the Action that belongs to this Event */)doc";

static const char *__doc_Event_GetTime = R"doc(Get the time of this Event */)doc";

static const char *__doc_Event_GetType = R"doc(Get the EventType of this Event */)doc";

static const char *__doc_Event_action = R"doc()doc";

static const char *__doc_Event_time = R"doc()doc";

static const char *__doc_Event_toString = R"doc(Get a string represenation of the Event */)doc";

static const char *__doc_Event_type = R"doc()doc";

static const char *__doc_Exit =
R"doc(The Exit action lets a scheduled ShuntingUnit depart from the shunting
yard.)doc";

static const char *__doc_ExitAction =
R"doc(The ExitAction lets a scheduled ShuntingUnit depart from the shunting
yard.)doc";

static const char *__doc_ExitActionGenerator = R"doc(The ExitActionGenerator generates ExitAction%s */)doc";

static const char *__doc_ExitActionGenerator_ExitActionGenerator = R"doc()doc";

static const char *__doc_ExitActionGenerator_ExitActionGenerator_2 = R"doc()doc";

static const char *__doc_ExitActionGenerator_ExitActionGenerator_3 = R"doc()doc";

static const char *__doc_ExitActionGenerator_Generate = R"doc()doc";

static const char *__doc_ExitActionGenerator_Generate_2 = R"doc()doc";

static const char *__doc_ExitAction_Clone = R"doc()doc";

static const char *__doc_ExitAction_CreateSimple = R"doc()doc";

static const char *__doc_ExitAction_ExitAction = R"doc()doc";

static const char *__doc_ExitAction_ExitAction_2 =
R"doc(Construct an ExitAction from the given parameters

The ShuntingUnit attribute of the Outgoing event may refer to
unspecified trains, that is trains of some type, with no set id.
Therefore the ShuntingUnit in the shunting yard need to be specified
seperately.)doc";

static const char *__doc_ExitAction_ExitAction_3 =
R"doc(Construct an ExitAction from the given parameters

The ShuntingUnit attribute of the Outgoing event may refer to
unspecified trains, that is trains of some type, with no set id.
Therefore the ShuntingUnit in the shunting yard need to be specified
seperately.)doc";

static const char *__doc_ExitAction_Finish = R"doc()doc";

static const char *__doc_ExitAction_GetDestinationTrack = R"doc(Get the destination Track for this Outgoing ShuntingUnit */)doc";

static const char *__doc_ExitAction_GetOutgoing = R"doc(Get the Outgoing event for this ArriveAction */)doc";

static const char *__doc_ExitAction_Start = R"doc()doc";

static const char *__doc_ExitAction_outgoing = R"doc()doc";

static const char *__doc_ExitAction_toString = R"doc()doc";

static const char *__doc_Exit_Clone = R"doc()doc";

static const char *__doc_Exit_Exit = R"doc()doc";

static const char *__doc_Exit_Exit_2 =
R"doc(Construct an Exit action for the ShuntingUnit described by the train
ids and the id of the Outgoing event.

The ShuntingUnit attribute of the Outgoing event may refer to
unspecified trains, that is trains of some type, with no set id.
Therefore the train ids of the ShuntingUnit in the shunting yard need
to be specified seperately.)doc";

static const char *__doc_Exit_Exit_3 =
R"doc(Construct an Exit action for the ShuntingUnit and the Outgoing event.

The ShuntingUnit attribute of the Outgoing event may refer to
unspecified trains, that is trains of some type, with no set id.
Therefore the ShuntingUnit in the shunting yard need to be specified
seperately.)doc";

static const char *__doc_Exit_Exit_4 = R"doc(The default copy constructor */)doc";

static const char *__doc_Exit_GetGeneratorName = R"doc()doc";

static const char *__doc_Exit_GetOutgoingID = R"doc(Get the id of the Outgoing event */)doc";

static const char *__doc_Exit_outgoingID = R"doc()doc";

static const char *__doc_Exit_toString = R"doc()doc";

static const char *__doc_Facility = R"doc(A service Facility on the Location that can service certain Task%s)doc";

static const char *__doc_Facility_2 = R"doc()doc";

static const char *__doc_Facility_AssignTracks = R"doc(Assign the Track%s that for this Facility */)doc";

static const char *__doc_Facility_ExecutesTask = R"doc(Returns true iff this Facility services the given Task */)doc";

static const char *__doc_Facility_Facility = R"doc()doc";

static const char *__doc_Facility_Facility_2 = R"doc(Construct a Facility given the parameters */)doc";

static const char *__doc_Facility_Facility_3 = R"doc(Construct a Facility given the protobuf object */)doc";

static const char *__doc_Facility_GetCapacity =
R"doc(Get the capacity of this Facility (the number of Train%s it can serve
simultaneously) */)doc";

static const char *__doc_Facility_GetID = R"doc(Get the id of this Facility */)doc";

static const char *__doc_Facility_GetTasks = R"doc(Get a list of Task types that this Facility can service */)doc";

static const char *__doc_Facility_GetTracks = R"doc(Get the list of Track%s of this Facility */)doc";

static const char *__doc_Facility_GetType = R"doc(Get the type of this Facility */)doc";

static const char *__doc_Facility_IsAvailable =
R"doc(Returns true iff this Facility is active during the given time window
*/)doc";

static const char *__doc_Facility_id = R"doc()doc";

static const char *__doc_Facility_operator_eq = R"doc(Returns true iff the two Facilities have the same id */)doc";

static const char *__doc_Facility_operator_ne = R"doc(Returns true iff the two Facilities do not have the same id */)doc";

static const char *__doc_Facility_simultaneousUsageCount = R"doc()doc";

static const char *__doc_Facility_tEnd = R"doc()doc";

static const char *__doc_Facility_tStart = R"doc()doc";

static const char *__doc_Facility_tasks = R"doc()doc";

static const char *__doc_Facility_toString = R"doc(Get a string representation of this Facility */)doc";

static const char *__doc_Facility_tracks = R"doc()doc";

static const char *__doc_Facility_type = R"doc()doc";

static const char *__doc_Incoming = R"doc(Describes a future Incoming ShuntingUnit)doc";

static const char *__doc_Incoming_Incoming = R"doc()doc";

static const char *__doc_Incoming_Incoming_2 = R"doc(Construct an Incoming object from the given parameters */)doc";

static const char *__doc_Incoming_Incoming_3 = R"doc(Construct an Incoming object from the given parameters */)doc";

static const char *__doc_Incoming_Incoming_4 = R"doc(Construct an Incoming object from the given protobuf object */)doc";

static const char *__doc_Incoming_Incoming_5 = R"doc(Copy constructor */)doc";

static const char *__doc_Incoming_toString = R"doc()doc";

static const char *__doc_InvalidActionException = R"doc(Raise this exception when trying to apply an invalid Action */)doc";

static const char *__doc_InvalidActionException_InvalidActionException = R"doc()doc";

static const char *__doc_InvalidActionException_InvalidActionException_2 = R"doc()doc";

static const char *__doc_InvalidActionException_InvalidActionException_3 = R"doc()doc";

static const char *__doc_InvalidActionException_message = R"doc()doc";

static const char *__doc_InvalidActionException_what = R"doc()doc";

static const char *__doc_InvalidConfigException = R"doc(Raise this exception when the specified Config file is invalid */)doc";

static const char *__doc_InvalidConfigException_InvalidConfigException = R"doc()doc";

static const char *__doc_InvalidConfigException_InvalidConfigException_2 = R"doc()doc";

static const char *__doc_InvalidConfigException_InvalidConfigException_3 = R"doc()doc";

static const char *__doc_InvalidConfigException_message = R"doc()doc";

static const char *__doc_InvalidConfigException_what = R"doc()doc";

static const char *__doc_InvalidLocationException = R"doc(Raise this exception when the specified Location file is invalid */)doc";

static const char *__doc_InvalidLocationException_InvalidLocationException = R"doc()doc";

static const char *__doc_InvalidLocationException_InvalidLocationException_2 = R"doc()doc";

static const char *__doc_InvalidLocationException_InvalidLocationException_3 = R"doc()doc";

static const char *__doc_InvalidLocationException_message = R"doc()doc";

static const char *__doc_InvalidLocationException_what = R"doc()doc";

static const char *__doc_InvalidScenarioException = R"doc(Raise this exception when the specified Scenario file is invalid */)doc";

static const char *__doc_InvalidScenarioException_InvalidScenarioException = R"doc()doc";

static const char *__doc_InvalidScenarioException_InvalidScenarioException_2 = R"doc()doc";

static const char *__doc_InvalidScenarioException_InvalidScenarioException_3 = R"doc()doc";

static const char *__doc_InvalidScenarioException_message = R"doc()doc";

static const char *__doc_InvalidScenarioException_what = R"doc()doc";

static const char *__doc_InvalidStateRequest =
R"doc(Raise this exception when performing an invalid request on the state
object */)doc";

static const char *__doc_InvalidStateRequest_InvalidStateRequest = R"doc()doc";

static const char *__doc_InvalidStateRequest_InvalidStateRequest_2 = R"doc()doc";

static const char *__doc_InvalidStateRequest_InvalidStateRequest_3 = R"doc()doc";

static const char *__doc_InvalidStateRequest_message = R"doc()doc";

static const char *__doc_InvalidStateRequest_what = R"doc()doc";

static const char *__doc_Join = R"doc()doc";

static const char *__doc_Join_2 = R"doc()doc";

static const char *__doc_Join_3 = R"doc()doc";

static const char *__doc_Location =
R"doc(A Location describes a shunting yard.

It contains a list of Track%s and Facilities. It also has helper
functions to find (shortest) Path%s and distances)doc";

static const char *__doc_LocationEngine = R"doc(A TORS engine for a specific Location)doc";

static const char *__doc_LocationEngine_2 = R"doc(A TORS engine for a specific Location)doc";

static const char *__doc_LocationEngine_ApplyAction = R"doc(Apply the given Action to the State */)doc";

static const char *__doc_LocationEngine_ApplyAction_2 = R"doc(Apply the given SimpleAction to the State */)doc";

static const char *__doc_LocationEngine_ApplyActionAndStep =
R"doc(Apply the Action to the State and go to the next step in the
simulation */)doc";

static const char *__doc_LocationEngine_ApplyActionAndStep_2 =
R"doc(Apply the SimpleAction to the State and go to the next step in the
simulation */)doc";

static const char *__doc_LocationEngine_ApplyWaitAllUntil = R"doc(Apply Wait actions for all non-waiting trains until the given time */)doc";

static const char *__doc_LocationEngine_CalcAllPossiblePaths =
R"doc(Calculate all the possible paths (run this once before requesting
possible paths) */)doc";

static const char *__doc_LocationEngine_CalcShortestPaths =
R"doc(Calculate all the shortest paths (run this once before requesting
shortest paths) */)doc";

static const char *__doc_LocationEngine_EndSession = R"doc(End the session that belongs to the given State */)doc";

static const char *__doc_LocationEngine_EvaluatePlan = R"doc(Evaluate the given POSPlan for the given Scenario */)doc";

static const char *__doc_LocationEngine_ExecuteEvent = R"doc()doc";

static const char *__doc_LocationEngine_ExecuteImmediateEvents = R"doc()doc";

static const char *__doc_LocationEngine_GenerateAction = R"doc(Generate an Action from the given SimpleAction */)doc";

static const char *__doc_LocationEngine_GetLocation = R"doc(Get a reference to the Location of this Engine */)doc";

static const char *__doc_LocationEngine_GetPath = R"doc(Get a path for the Move */)doc";

static const char *__doc_LocationEngine_GetResult = R"doc(Get the RunResult for the given State/session */)doc";

static const char *__doc_LocationEngine_GetScenario = R"doc(Get the Scenario given in the file path */)doc";

static const char *__doc_LocationEngine_GetValidActions = R"doc(Get a list of valid Action%s for the given State */)doc";

static const char *__doc_LocationEngine_ImportResult = R"doc(Import a RunResult from a protobuf file */)doc";

static const char *__doc_LocationEngine_IsStateActive =
R"doc(Returns true iff the given state is still active (ie the scenario is
not ended or failed yet) */)doc";

static const char *__doc_LocationEngine_IsValidAction =
R"doc(Checks if the given SimpleAction is valid in the given State or not.
If not provides a reason why.)doc";

static const char *__doc_LocationEngine_IsValidAction_2 =
R"doc(Checks if the given Action is valid in the given State or not. If not
provides a reason why.)doc";

static const char *__doc_LocationEngine_LocationEngine = R"doc()doc";

static const char *__doc_LocationEngine_LocationEngine_2 =
R"doc(Construct a LocationEngine based on the configuration files found in
the given folder */)doc";

static const char *__doc_LocationEngine_StartSession =
R"doc(Start a session for the given Scenario and generate an initial State
*/)doc";

static const char *__doc_LocationEngine_Step = R"doc(Go to the next Step in the simulation and update the State */)doc";

static const char *__doc_LocationEngine_actionManager = R"doc()doc";

static const char *__doc_LocationEngine_config = R"doc()doc";

static const char *__doc_LocationEngine_location = R"doc()doc";

static const char *__doc_LocationEngine_path = R"doc()doc";

static const char *__doc_LocationEngine_results = R"doc()doc";

static const char *__doc_LocationEngine_scenarios = R"doc()doc";

static const char *__doc_LocationEngine_stateActionMap = R"doc()doc";

static const char *__doc_Location_CalcAllPossiblePaths =
R"doc(Calculate all the possible paths.

Calling this method once is required if you want to use
GetPossiblePaths.)doc";

static const char *__doc_Location_CalcNeighboringPaths =
R"doc(Calculate all the neighboring paths.

Calling this method once is required if you want to use
GetNeighborPath.)doc";

static const char *__doc_Location_CalcShortestPaths =
R"doc(Calculate all the shortest paths.

Calling this method once (per TrainUnitType) is required if you want
to use GetShortestPath. The parameter type is used because different
train types have different Setback times.)doc";

static const char *__doc_Location_GetDistance =
R"doc(Get the distance from one Track to another based on the provided
distance matrix

Note that this distance is only used when configured appropriatly)doc";

static const char *__doc_Location_GetDistance_2 = R"doc(Get the duration for the route described by the Track%s)doc";

static const char *__doc_Location_GetDistance_3 = R"doc(Get the duration for the route described by the Track%s)doc";

static const char *__doc_Location_GetDurationByType =
R"doc(Get the duration for crossing the given track based on its type
(Railroad or Switch))doc";

static const char *__doc_Location_GetFacilities = R"doc(Get all the facilities */)doc";

static const char *__doc_Location_GetFacilityByID = R"doc(Get a reference to a Facility by its id */)doc";

static const char *__doc_Location_GetLocationFilePath = R"doc(Get the path of the protobuf file */)doc";

static const char *__doc_Location_GetNeighborPath =
R"doc(Get the neighboring path from a certain position to a certain
destination

A neighboring path is a path to a neighboring Railroad track. Every
track has immediate neighbors as defined by the A-side and B-side
attributes. This method returns a path to a (non-immediate) Railroad
neighbor that can be reached by traversing zero or more non-Railroad
tracks.

Call CalcNeighboringPaths (once) to calculate all the neighboring
paths)doc";

static const char *__doc_Location_GetNeighboringPaths =
R"doc(Get all the neighboring paths from a certain position

A neighboring path is a path to a neighboring Railroad track. Every
track has immediate neighbors as defined by the A-side and B-side
attributes. This method returns all (non-immediate) Railroad neighbors
that can be reached by traversing zero or more non-Railroad tracks.

Call CalcNeighboringPaths (once) to calculate all the neighboring
paths)doc";

static const char *__doc_Location_GetPossiblePaths =
R"doc(Get all the possible Path%s from Position from to Position to (without
setbacks)

Call CalcAllPossiblePaths (once) to callculate all possible paths)doc";

static const char *__doc_Location_GetPossiblePaths_2 =
R"doc(Get all the possible Path%s from Position from to any other Position
(without setbacks)

Call CalcAllPossiblePaths (once) to callculate all possible paths)doc";

static const char *__doc_Location_GetShortestPath =
R"doc(Get the shortest path from a certain position to a destination.

Both from and to are Position%s, a tuple describing the ShuntingUnit's
current position and its previous position. The current position must
always be a track of type Railroad Call CalcShortestPaths (once) to
calculate all the shortest paths)doc";

static const char *__doc_Location_GetTrackByID = R"doc(Get a reference to the Track by its id */)doc";

static const char *__doc_Location_GetTracks = R"doc(Get all the Track%s */)doc";

static const char *__doc_Location_ImportDistanceMatrix = R"doc()doc";

static const char *__doc_Location_ImportFacilities = R"doc()doc";

static const char *__doc_Location_ImportTracks = R"doc()doc";

static const char *__doc_Location_Location = R"doc()doc";

static const char *__doc_Location_Location_2 =
R"doc(Construct a Location from a protobuf file

The parameter byType determines if distances are calculated by
TrackPartType or by using the distance matrix)doc";

static const char *__doc_Location_Location_3 = R"doc(Default copy constructor */)doc";

static const char *__doc_Location_byType = R"doc()doc";

static const char *__doc_Location_distanceMatrix = R"doc()doc";

static const char *__doc_Location_facilities = R"doc()doc";

static const char *__doc_Location_moveDuration = R"doc()doc";

static const char *__doc_Location_movementConstant = R"doc()doc";

static const char *__doc_Location_neighborPaths = R"doc(Path from a Railroad position to all neighboring RailRoad positions */)doc";

static const char *__doc_Location_path = R"doc()doc";

static const char *__doc_Location_possibleMovements =
R"doc(A list of all possible paths starting from a Position, without
setbacks */)doc";

static const char *__doc_Location_possiblePaths =
R"doc(A list of all possible paths. The map is from (start_position,
end_position), without setbacks */)doc";

static const char *__doc_Location_shortestPath =
R"doc(A shortest path map for each setbackTime. The map is from
(start_position, end_position) -> path */)doc";

static const char *__doc_Location_trackIndex = R"doc(All the tracks indexed by their string id */)doc";

static const char *__doc_Location_tracks = R"doc()doc";

static const char *__doc_Move =
R"doc(The Move action moves a ShuntingUnit from one Track to a neighboring
Railroad Track.)doc";

static const char *__doc_MoveAction =
R"doc(The MoveAction moves a ShuntingUnit from one Track to a neighboring
Railroad Track.)doc";

static const char *__doc_MoveActionGenerator = R"doc(The MoveActionGenerator generates MoveAction%s */)doc";

static const char *__doc_MoveActionGenerator_Generate = R"doc()doc";

static const char *__doc_MoveActionGenerator_Generate_2 = R"doc()doc";

static const char *__doc_MoveActionGenerator_GenerateMovesFrom = R"doc()doc";

static const char *__doc_MoveActionGenerator_GeneratePath = R"doc(Generate a Path for the given Move action */)doc";

static const char *__doc_MoveActionGenerator_MoveActionGenerator =
R"doc(Construct this MoveActionGenerator based on the parameters defined in
the json object */)doc";

static const char *__doc_MoveActionGenerator_MoveActionGenerator_2 = R"doc()doc";

static const char *__doc_MoveActionGenerator_MoveActionGenerator_3 = R"doc()doc";

static const char *__doc_MoveActionGenerator_constantTime = R"doc()doc";

static const char *__doc_MoveActionGenerator_defaultTime = R"doc()doc";

static const char *__doc_MoveActionGenerator_noRoutingDuration = R"doc()doc";

static const char *__doc_MoveActionGenerator_normTime = R"doc()doc";

static const char *__doc_MoveActionGenerator_walkTime = R"doc()doc";

static const char *__doc_MoveAction_Clone = R"doc()doc";

static const char *__doc_MoveAction_CreateSimple = R"doc()doc";

static const char *__doc_MoveAction_Finish = R"doc()doc";

static const char *__doc_MoveAction_GetDestinationTrack = R"doc(Get the destination Track of this MoveAction */)doc";

static const char *__doc_MoveAction_GetPreviousTrack =
R"doc(Get the previous Track of the moving ShuntingUnit when it has arrived
on its destination Track */)doc";

static const char *__doc_MoveAction_GetTracks = R"doc(Get all the tracks that are used for this MoveAction */)doc";

static const char *__doc_MoveAction_IsStepMove = R"doc(True if this MoveAction is a step-by-step move, otherwise false */)doc";

static const char *__doc_MoveAction_MoveAction = R"doc()doc";

static const char *__doc_MoveAction_MoveAction_2 =
R"doc(Construct a MoveAction from the given parameters

The tracks are all the tracks that the ShuntingUnit passes in reaching
its destination. The reserved tracks are all those tracks except the
first track, the current track of the ShuntingUnit.)doc";

static const char *__doc_MoveAction_Start = R"doc()doc";

static const char *__doc_MoveAction_stepMove = R"doc()doc";

static const char *__doc_MoveAction_toString = R"doc()doc";

static const char *__doc_MoveAction_tracks = R"doc()doc";

static const char *__doc_MoveHelperGenerator =
R"doc(The MoveHelperGenerator generates BeginMoveAction%s, EndMoveAction%s
and single step MoveAction%s */)doc";

static const char *__doc_MoveHelperGenerator_Generate = R"doc()doc";

static const char *__doc_MoveHelperGenerator_Generate_2 = R"doc()doc";

static const char *__doc_MoveHelperGenerator_MoveHelperGenerator = R"doc()doc";

static const char *__doc_MoveHelperGenerator_MoveHelperGenerator_2 = R"doc()doc";

static const char *__doc_MoveHelperGenerator_MoveHelperGenerator_3 = R"doc()doc";

static const char *__doc_Move_Clone = R"doc()doc";

static const char *__doc_Move_GetDestinationID = R"doc(Get the id of the destination track */)doc";

static const char *__doc_Move_GetGeneratorName = R"doc()doc";

static const char *__doc_Move_Move = R"doc()doc";

static const char *__doc_Move_Move_2 =
R"doc(Construct a Move action for the ShuntingUnit described by the train
ids to the Track with id destinationID */)doc";

static const char *__doc_Move_Move_3 =
R"doc(Construct a Move action for the ShuntingUnit described by the train
ids to the destination Track */)doc";

static const char *__doc_Move_Move_4 =
R"doc(Construct a Move action for the ShuntingUnit su to the destination
Track */)doc";

static const char *__doc_Move_Move_5 = R"doc(Default copy constructor */)doc";

static const char *__doc_Move_destinationID = R"doc()doc";

static const char *__doc_Move_toString = R"doc()doc";

static const char *__doc_MultiMove =
R"doc(The MultiMove action moves a ShuntingUnit from one Track to another
Railroad Track.)doc";

static const char *__doc_MultiMove_Clone = R"doc()doc";

static const char *__doc_MultiMove_GetDestinationID = R"doc(Get the id of the destination track */)doc";

static const char *__doc_MultiMove_GetGeneratorName = R"doc()doc";

static const char *__doc_MultiMove_GetTrackIDs = R"doc(Get the Track IDs of the route */)doc";

static const char *__doc_MultiMove_MultiMove = R"doc()doc";

static const char *__doc_MultiMove_MultiMove_2 =
R"doc(Construct a MultiMove action for the ShuntingUnit described by the
train ids over the given list of Track IDs */)doc";

static const char *__doc_MultiMove_MultiMove_3 =
R"doc(Construct a MultiMove action for the ShuntingUnit described by the
train ids over the given vector of Track%s */)doc";

static const char *__doc_MultiMove_MultiMove_4 =
R"doc(Construct a MultiMove action for the ShuntingUnit su over the given
vector of Track%s */)doc";

static const char *__doc_MultiMove_MultiMove_5 =
R"doc(Construct a MultiMove action for the ShuntingUnit described by the
train ids over the given list of Track%s */)doc";

static const char *__doc_MultiMove_MultiMove_6 =
R"doc(Construct a MultiMove action for the ShuntingUnit su over the given
list of Track%s */)doc";

static const char *__doc_MultiMove_MultiMove_7 = R"doc(Default copy constructor */)doc";

static const char *__doc_MultiMove_toString = R"doc()doc";

static const char *__doc_MultiMove_trackIDs = R"doc()doc";

static const char *__doc_NonExistingPathException =
R"doc(Raise this exception when trying to find a non-existing Path in the
shunting yard */)doc";

static const char *__doc_NonExistingPathException_NonExistingPathException = R"doc()doc";

static const char *__doc_NonExistingPathException_NonExistingPathException_2 = R"doc()doc";

static const char *__doc_NonExistingPathException_NonExistingPathException_3 = R"doc()doc";

static const char *__doc_NonExistingPathException_message = R"doc()doc";

static const char *__doc_NonExistingPathException_what = R"doc()doc";

static const char *__doc_Outgoing =
R"doc(Describes a future Outgoing ShuntingUnit

Note that the ids of the Train%s in the ShuntingUnit may be set to -1,
in case the precise ids of the outgoing trains do not matter.)doc";

static const char *__doc_Outgoing_Outgoing = R"doc()doc";

static const char *__doc_Outgoing_Outgoing_2 = R"doc(Construct an Outgoing object from the given parameters */)doc";

static const char *__doc_Outgoing_Outgoing_3 = R"doc(Construct an Outgoing object from the given parameters */)doc";

static const char *__doc_Outgoing_Outgoing_4 = R"doc(Construct an Outgoing object from the given protobuf object */)doc";

static const char *__doc_Outgoing_Outgoing_5 = R"doc(Copy constructor */)doc";

static const char *__doc_Outgoing_toString = R"doc()doc";

static const char *__doc_PBToStringList = R"doc()doc";

static const char *__doc_PBToStringVector = R"doc()doc";

static const char *__doc_POSAction =
R"doc(A POSAction is an action in a Partial Order Schedule

This class is used for serializing actions to protobuf)doc";

static const char *__doc_POSAction_CreatePOSAction = R"doc(Construct a POSAction from a protobuf action */)doc";

static const char *__doc_POSAction_GetAction = R"doc(Get the SimpleAction of this POSAction */)doc";

static const char *__doc_POSAction_GetID = R"doc(Get the unique id of the action */)doc";

static const char *__doc_POSAction_GetMinimumDuration = R"doc(Get the suggested minimum duration time of the action */)doc";

static const char *__doc_POSAction_GetSuggestedEnd = R"doc(Get the suggested end time of the action */)doc";

static const char *__doc_POSAction_GetSuggestedStart = R"doc(Get the suggested start time of the action */)doc";

static const char *__doc_POSAction_POSAction = R"doc()doc";

static const char *__doc_POSAction_POSAction_2 = R"doc(Construct a POSAction from the given parameters */)doc";

static const char *__doc_POSAction_POSAction_3 = R"doc(Copy constructor */)doc";

static const char *__doc_POSAction_Serialize = R"doc(Serialize this POSAction to a protobuf object */)doc";

static const char *__doc_POSAction_action = R"doc()doc";

static const char *__doc_POSAction_id = R"doc()doc";

static const char *__doc_POSAction_minDuration = R"doc()doc";

static const char *__doc_POSAction_operator_assign = R"doc(Assignment operator */)doc";

static const char *__doc_POSAction_suggestedEnd = R"doc()doc";

static const char *__doc_POSAction_suggestedStart = R"doc()doc";

static const char *__doc_POSMatch =
R"doc(A POSMatch describes a match between Outgoing trains and Train%s (not
yet implemented))doc";

static const char *__doc_POSMatch_out = R"doc()doc";

static const char *__doc_POSMatch_position = R"doc()doc";

static const char *__doc_POSMatch_train = R"doc()doc";

static const char *__doc_POSPlan =
R"doc(A POPlan describes a Partial Order Schedule

A POSPlan consists of a list of POSAction%s, POSMatch%es and
POSPrecedenceConstraint%s)doc";

static const char *__doc_POSPlan_2 =
R"doc(A POPlan describes a Partial Order Schedule

A POSPlan consists of a list of POSAction%s, POSMatch%es and
POSPrecedenceConstraint%s)doc";

static const char *__doc_POSPlan_AddAction = R"doc(Add a POSAction to the list of POSAction%s */)doc";

static const char *__doc_POSPlan_CreatePOSPlan = R"doc(Construct a POSPlan from a protobuf object */)doc";

static const char *__doc_POSPlan_GetActions = R"doc(Get the list of POSAction%s */)doc";

static const char *__doc_POSPlan_POSPlan = R"doc(Construct an empty POSPlan */)doc";

static const char *__doc_POSPlan_POSPlan_2 = R"doc(Construct a POSPlan based on the list of POSAction%s */)doc";

static const char *__doc_POSPlan_Serialize = R"doc(Serialize this plan to a protobuf object */)doc";

static const char *__doc_POSPlan_SerializeToFile = R"doc(Serialize this plan to a protobuf file */)doc";

static const char *__doc_POSPlan_actions = R"doc()doc";

static const char *__doc_POSPlan_feasible = R"doc()doc";

static const char *__doc_POSPlan_graph = R"doc()doc";

static const char *__doc_POSPlan_matching = R"doc()doc";

static const char *__doc_POSPrecedenceConstraint =
R"doc(A POSPrecedenceConstraint describes a precedence constraint between
two POSAction%s (not yet implemented))doc";

static const char *__doc_POSPrecedenceConstraint_firstAction = R"doc()doc";

static const char *__doc_POSPrecedenceConstraint_minimumTimeLag = R"doc()doc";

static const char *__doc_POSPrecedenceConstraint_secondAction = R"doc()doc";

static const char *__doc_Path =
R"doc(A Path describes a route through the shunting yard.

Note that the length of the path may depend on a Train's type)doc";

static const char *__doc_Path_Append = R"doc(Append a path to the Path */)doc";

static const char *__doc_Path_GetDestination = R"doc(Get the destination track of this path */)doc";

static const char *__doc_Path_GetNumberOfTracks = R"doc(Get the number of Track%s in this Path */)doc";

static const char *__doc_Path_GetStart = R"doc(Get the starting track of this path */)doc";

static const char *__doc_Path_Path = R"doc(Initialize an empty path with infinite length */)doc";

static const char *__doc_Path_Path_2 = R"doc(Initialize a path given the parameters */)doc";

static const char *__doc_Path_length =
R"doc(< The length of this route in seconds, possibly different for
different train types. */)doc";

static const char *__doc_Path_route =
R"doc(< All the tracks that are part of this path, ordered from start
position to end position */)doc";

static const char *__doc_Path_toString = R"doc(Get a string representation of this path */)doc";

static const char *__doc_RunResult =
R"doc(A RunResult describes a TORS session

A TORS session is run at a Location, given a certain Scenario. In this
context a POSPlan will be, or was run)doc";

static const char *__doc_RunResult_2 =
R"doc(A RunResult describes a TORS session

A TORS session is run at a Location, given a certain Scenario. In this
context a POSPlan will be, or was run)doc";

static const char *__doc_RunResult_AddAction = R"doc(Add a POSAction to the plan */)doc";

static const char *__doc_RunResult_CreateRunResult =
R"doc(Construct a RunResult from a protobuf object by using the provided
Engine */)doc";

static const char *__doc_RunResult_CreateRunResult_2 =
R"doc(Construct a RunResult from a protobuf object by using the provided
Location */)doc";

static const char *__doc_RunResult_GetActions = R"doc(Get the actions in the plan */)doc";

static const char *__doc_RunResult_GetLocation = R"doc(Get the location string for this run */)doc";

static const char *__doc_RunResult_GetPlan = R"doc(Get the POSPlan for this run */)doc";

static const char *__doc_RunResult_GetScenario = R"doc(Get the Scenario for this run */)doc";

static const char *__doc_RunResult_RunResult = R"doc()doc";

static const char *__doc_RunResult_RunResult_2 =
R"doc(Construct a RunResult for a location and a Scenario with an empty plan
*/)doc";

static const char *__doc_RunResult_RunResult_3 = R"doc(Construct a RunResult for a location and a Scenario and a plan */)doc";

static const char *__doc_RunResult_RunResult_4 = R"doc(Default copy constructor */)doc";

static const char *__doc_RunResult_Serialize = R"doc(Serialize this object to a protobuf object */)doc";

static const char *__doc_RunResult_SerializeToFile = R"doc(Serialize this object to a protobuf file */)doc";

static const char *__doc_RunResult_feasible = R"doc()doc";

static const char *__doc_RunResult_location = R"doc()doc";

static const char *__doc_RunResult_plan = R"doc()doc";

static const char *__doc_RunResult_scenario = R"doc()doc";

static const char *__doc_Scenario =
R"doc(The Scenario class describes a session by its Incoming trains,
Outgoing trains Employee%s and Disturbance%s. An initial State is
generated from a Scenario object.)doc";

static const char *__doc_ScenarioFailedException =
R"doc(Raise this exception when a TORS session ends in an invalid, or
finished but unsolved State */)doc";

static const char *__doc_ScenarioFailedException_ScenarioFailedException = R"doc()doc";

static const char *__doc_ScenarioFailedException_ScenarioFailedException_2 = R"doc()doc";

static const char *__doc_ScenarioFailedException_ScenarioFailedException_3 = R"doc()doc";

static const char *__doc_ScenarioFailedException_message = R"doc()doc";

static const char *__doc_ScenarioFailedException_what = R"doc()doc";

static const char *__doc_Scenario_AddDisturbance = R"doc(Add a Disturbance to this Scenario (not yet implemented) */)doc";

static const char *__doc_Scenario_AddEmployee = R"doc(Add an Employee to this Scenario (not yet implemented) */)doc";

static const char *__doc_Scenario_AddIncomingTrain = R"doc(Add an Incoming train to this Scenario */)doc";

static const char *__doc_Scenario_AddOutgoingTrain = R"doc(Add an Outgoing train to this Scenario */)doc";

static const char *__doc_Scenario_GetDisturbanceVector = R"doc(Get the disturbances in this Scenario as a vector*/)doc";

static const char *__doc_Scenario_GetDisturbances = R"doc(Get the disturbances in this Scenario as an EventQueue */)doc";

static const char *__doc_Scenario_GetEmployees = R"doc(Get all the employees (not yet implemented) */)doc";

static const char *__doc_Scenario_GetEndTime = R"doc(Get the end time of this Scenario */)doc";

static const char *__doc_Scenario_GetIncomingBySU = R"doc(Get an Incoming train by its ShuntingUnit */)doc";

static const char *__doc_Scenario_GetIncomingByTrain = R"doc(Get an Incoming train by one of its Train%s */)doc";

static const char *__doc_Scenario_GetIncomingByTrainID = R"doc(Get an Incoming train by one of its Train ids */)doc";

static const char *__doc_Scenario_GetIncomingByTrainIDs = R"doc(Get an Incoming train by its Train ids */)doc";

static const char *__doc_Scenario_GetIncomingTrains = R"doc(Get all the Incoming trains */)doc";

static const char *__doc_Scenario_GetNumberOfTrains = R"doc(Get the number of Train%s in this Scenario */)doc";

static const char *__doc_Scenario_GetOutgoingBySU = R"doc(Get an Outgoing train by its ShuntingUnit */)doc";

static const char *__doc_Scenario_GetOutgoingByTrain = R"doc(Get an Outgoing train by one of its Train%s */)doc";

static const char *__doc_Scenario_GetOutgoingByTrainID = R"doc(Get an Outgoing train by one of its Train ids */)doc";

static const char *__doc_Scenario_GetOutgoingByTrainIDs = R"doc(Get an Outgoing train by its Train ids */)doc";

static const char *__doc_Scenario_GetOutgoingTrains = R"doc(Get all the Outgoing trains */)doc";

static const char *__doc_Scenario_GetStartTime = R"doc(Get the start time of this Scenario */)doc";

static const char *__doc_Scenario_GetTasksForTrain = R"doc(Get all the Task%s for a Train in this Scenario */)doc";

static const char *__doc_Scenario_GetTrainByID = R"doc(Get a Train by its id from this Scenario */)doc";

static const char *__doc_Scenario_ImportEmployees = R"doc()doc";

static const char *__doc_Scenario_ImportShuntingUnits = R"doc()doc";

static const char *__doc_Scenario_Init = R"doc()doc";

static const char *__doc_Scenario_PrintScenarioInfo = R"doc(Print the Scenario Info */)doc";

static const char *__doc_Scenario_Scenario = R"doc(Generate an empty scenario */)doc";

static const char *__doc_Scenario_Scenario_2 =
R"doc(Generate a Scenario according to the protobuf file on the given path
for the provided Location */)doc";

static const char *__doc_Scenario_Scenario_3 =
R"doc(Generate a Scenario according to the protobuf object for the provided
Location */)doc";

static const char *__doc_Scenario_Scenario_4 = R"doc(Copy constructor */)doc";

static const char *__doc_Scenario_Serialize = R"doc(Serialize this object to a protobuf object */)doc";

static const char *__doc_Scenario_SetDisturbances =
R"doc(Set the queue of Disturbance%s in this Scenario (not yet implemented)
*/)doc";

static const char *__doc_Scenario_SetEmployees = R"doc(Set the list of Employee%s in this Scenario (not yet implemented) */)doc";

static const char *__doc_Scenario_SetEndTime = R"doc(Set the end time of this scenario */)doc";

static const char *__doc_Scenario_SetIncomingTrains = R"doc(Set the list of Incoming trains in this Scenario */)doc";

static const char *__doc_Scenario_SetOutgoingTrains = R"doc(Set the list of Outgoing trains in this Scenario */)doc";

static const char *__doc_Scenario_SetStartTime = R"doc(Set the start time of this scenario */)doc";

static const char *__doc_Scenario_disturbanceList = R"doc()doc";

static const char *__doc_Scenario_disturbances = R"doc()doc";

static const char *__doc_Scenario_employees = R"doc()doc";

static const char *__doc_Scenario_endTime = R"doc()doc";

static const char *__doc_Scenario_incomingTrains = R"doc()doc";

static const char *__doc_Scenario_outgoingTrains = R"doc()doc";

static const char *__doc_Scenario_startTime = R"doc()doc";

static const char *__doc_Service =
R"doc(The Service action executes a task on the specified train.

Note that service tasks are executed on trains, not on shunting units.
The reason is that trains are persistent and atomic. Shunting units
can change.)doc";

static const char *__doc_ServiceAction = R"doc(The ServiceAction executes a task on the specified train.)doc";

static const char *__doc_ServiceActionGenerator = R"doc(The ServiceActionGenerator generates ServiceAction%s */)doc";

static const char *__doc_ServiceActionGenerator_Generate = R"doc()doc";

static const char *__doc_ServiceActionGenerator_Generate_2 = R"doc()doc";

static const char *__doc_ServiceActionGenerator_ServiceActionGenerator = R"doc()doc";

static const char *__doc_ServiceActionGenerator_ServiceActionGenerator_2 = R"doc()doc";

static const char *__doc_ServiceActionGenerator_ServiceActionGenerator_3 = R"doc()doc";

static const char *__doc_ServiceAction_Clone = R"doc()doc";

static const char *__doc_ServiceAction_CreateSimple = R"doc()doc";

static const char *__doc_ServiceAction_Finish = R"doc()doc";

static const char *__doc_ServiceAction_GetFacility = R"doc(Get the Facility at which the Task will be executed */)doc";

static const char *__doc_ServiceAction_GetTask = R"doc(Get the Task that will be executed */)doc";

static const char *__doc_ServiceAction_GetTrain = R"doc(Get the Train that this service Task is operated on */)doc";

static const char *__doc_ServiceAction_ServiceAction = R"doc()doc";

static const char *__doc_ServiceAction_ServiceAction_2 =
R"doc(Construct a ServiceAction for the given parameters

Note that service tasks are executed on trains, not on shunting units.
The reason is that trains are persistent and atomic. Shunting units
can change.)doc";

static const char *__doc_ServiceAction_Start = R"doc()doc";

static const char *__doc_ServiceAction_facility = R"doc()doc";

static const char *__doc_ServiceAction_task = R"doc()doc";

static const char *__doc_ServiceAction_toString = R"doc()doc";

static const char *__doc_ServiceAction_train = R"doc()doc";

static const char *__doc_Service_Clone = R"doc()doc";

static const char *__doc_Service_GetFacilityID = R"doc(Get the id of the Facility where the Task is operated */)doc";

static const char *__doc_Service_GetGeneratorName = R"doc()doc";

static const char *__doc_Service_GetTask = R"doc(Get the service Task */)doc";

static const char *__doc_Service_GetTrain = R"doc(Get the Train that the Task should be operated on */)doc";

static const char *__doc_Service_Service = R"doc()doc";

static const char *__doc_Service_Service_2 = R"doc(Construct a Service action from the given parameters */)doc";

static const char *__doc_Service_Service_3 = R"doc(Construct a Service action from the given parameters */)doc";

static const char *__doc_Service_Service_4 = R"doc(Construct a Service action from the given parameters */)doc";

static const char *__doc_Service_Service_5 = R"doc(Default copy constructor */)doc";

static const char *__doc_Service_facilityID = R"doc()doc";

static const char *__doc_Service_task = R"doc()doc";

static const char *__doc_Service_toString = R"doc()doc";

static const char *__doc_Service_train = R"doc()doc";

static const char *__doc_Setback = R"doc(The Setback action changes the direction of the ShuntingUnit)doc";

static const char *__doc_SetbackAction = R"doc(The SetbackAction changes the direction of the ShuntingUnit)doc";

static const char *__doc_SetbackActionGenerator = R"doc(The SetbackActionGenerator generates SetbackAction%s */)doc";

static const char *__doc_SetbackActionGenerator_Generate = R"doc()doc";

static const char *__doc_SetbackActionGenerator_Generate_2 = R"doc()doc";

static const char *__doc_SetbackActionGenerator_GetDuration = R"doc()doc";

static const char *__doc_SetbackActionGenerator_SetbackActionGenerator =
R"doc(Construct this SetbackActionGenerator based on the parameters defined
in the json object */)doc";

static const char *__doc_SetbackActionGenerator_SetbackActionGenerator_2 = R"doc()doc";

static const char *__doc_SetbackActionGenerator_SetbackActionGenerator_3 = R"doc()doc";

static const char *__doc_SetbackActionGenerator_constantTime = R"doc()doc";

static const char *__doc_SetbackActionGenerator_defaultTime = R"doc()doc";

static const char *__doc_SetbackActionGenerator_normTime = R"doc()doc";

static const char *__doc_SetbackActionGenerator_walkTime = R"doc()doc";

static const char *__doc_SetbackAction_Clone = R"doc()doc";

static const char *__doc_SetbackAction_CreateSimple = R"doc()doc";

static const char *__doc_SetbackAction_Finish = R"doc()doc";

static const char *__doc_SetbackAction_GetDrivers = R"doc(Get the drivers for this action (not yet implemented) */)doc";

static const char *__doc_SetbackAction_SetbackAction = R"doc()doc";

static const char *__doc_SetbackAction_SetbackAction_2 = R"doc(Construct a SetbackAction from the given parameters */)doc";

static const char *__doc_SetbackAction_Start = R"doc()doc";

static const char *__doc_SetbackAction_toString = R"doc()doc";

static const char *__doc_Setback_Clone = R"doc()doc";

static const char *__doc_Setback_GetGeneratorName = R"doc()doc";

static const char *__doc_Setback_Setback = R"doc()doc";

static const char *__doc_Setback_Setback_2 = R"doc()doc";

static const char *__doc_Setback_Setback_3 = R"doc()doc";

static const char *__doc_Setback_Setback_4 = R"doc()doc";

static const char *__doc_Setback_toString = R"doc()doc";

static const char *__doc_ShuntingUnit =
R"doc(A ShuntingUnit describes connected Train%s that are shunted as one
unit)doc";

static const char *__doc_ShuntingUnit_2 =
R"doc(A ShuntingUnit describes connected Train%s that are shunted as one
unit)doc";

static const char *__doc_ShuntingUnitEquals = R"doc()doc";

static const char *__doc_ShuntingUnitEquals_operator_call = R"doc()doc";

static const char *__doc_ShuntingUnitHash = R"doc(//!\cond NO_DOC)doc";

static const char *__doc_ShuntingUnitHash_operator_call = R"doc()doc";

static const char *__doc_ShuntingUnitState =
R"doc(The ShuntingUnitState struct describes the state of a ShuntingUnit

Note: The current implementation of the direction of the ShuntingUnit
is not correct yet and will be changed.)doc";

static const char *__doc_ShuntingUnitState_HasActiveAction = R"doc(Returns true iff the ShuntingUnit has active Action%s */)doc";

static const char *__doc_ShuntingUnitState_ShuntingUnitState = R"doc()doc";

static const char *__doc_ShuntingUnitState_ShuntingUnitState_2 = R"doc(Copy constructor */)doc";

static const char *__doc_ShuntingUnitState_ShuntingUnitState_3 = R"doc(Iniitialize the ShuntingUnitState based on the given parameters */)doc";

static const char *__doc_ShuntingUnitState_activeActions = R"doc(< The list of active Action%s for this ShuntingUnit */)doc";

static const char *__doc_ShuntingUnitState_beginMoving = R"doc(< True iff the ShuntingUnit has just started movign */)doc";

static const char *__doc_ShuntingUnitState_frontTrain = R"doc(< A reference to the current front Train of the ShuntingUnit */)doc";

static const char *__doc_ShuntingUnitState_inNeutral = R"doc(< True iff the ShuntingUnit is currently in neutral position */)doc";

static const char *__doc_ShuntingUnitState_moving = R"doc(< True iff the ShuntingUnit is currently moving */)doc";

static const char *__doc_ShuntingUnitState_position = R"doc(< The position of the ShuntingUnit */)doc";

static const char *__doc_ShuntingUnitState_previous = R"doc(< The previous position of the ShuntingUnit */)doc";

static const char *__doc_ShuntingUnitState_waiting = R"doc(< True iff the ShuntingUnit is curently waiting */)doc";

static const char *__doc_ShuntingUnit_GetID = R"doc(Get the ID of the ShuntingUnit */)doc";

static const char *__doc_ShuntingUnit_GetLength = R"doc(Get the total length of the ShuntingUnit */)doc";

static const char *__doc_ShuntingUnit_GetNumberOfTrains = R"doc(Get the number of Train%s in this ShuntingUnit */)doc";

static const char *__doc_ShuntingUnit_GetSetbackTime =
R"doc(Get the Setback time for this ShuntingUnit, based on the current
front-train and the given parameters */)doc";

static const char *__doc_ShuntingUnit_GetSetbackTime_2 =
R"doc(Get the Setback time for this ShuntingUnit, based on the current
front-train and the given parameters */)doc";

static const char *__doc_ShuntingUnit_GetStartUpTime =
R"doc(Get the start-up time for this ShuntingUnit, based on the current
front-train */)doc";

static const char *__doc_ShuntingUnit_GetTrainByID = R"doc(Get a Train reference of a Train in this ShuntingUnit by its id */)doc";

static const char *__doc_ShuntingUnit_GetTrainIDs = R"doc(Get all the ids of the ShuntingUnit's Train%s */)doc";

static const char *__doc_ShuntingUnit_GetTrainIndexByID = R"doc(Get the index of a Train in this ShuntingUnit by its id */)doc";

static const char *__doc_ShuntingUnit_GetTrainString = R"doc(Get a string representation of the ShuntingUnit's Train%s */)doc";

static const char *__doc_ShuntingUnit_GetTrains = R"doc(Get the Train%s in this ShuntingUnit */)doc";

static const char *__doc_ShuntingUnit_HasAnyTrainByIDs = R"doc(Checks if this ShuntingUnit has a Train with any of the given ids */)doc";

static const char *__doc_ShuntingUnit_HasTrainByID = R"doc(Checks if this ShuntingUnit has a Train with the given id */)doc";

static const char *__doc_ShuntingUnit_MatchesShuntingUnit =
R"doc(Checks if two ShuntingUnit%s match (based on id or type)

Returns:
    true if the two ShuntingUnits match. They match iff 1. All Trains
    have the same ID. 2. OR the ID of the Trains are -1 and the types
    of the Train match.

The matching does consider order, but does not consider direction,
thus 1. 1-2-3 and 3-2-1 match. 2. 1-3-2 and 1-2-3 do not match.)doc";

static const char *__doc_ShuntingUnit_MatchesTrainIDs =
R"doc(Checks if this ShuntingUnit matches a list of given train ids and
train types.

Returns:
    true if this ShuntingUnit and ShuntingUnit described by the vector
    of ids and types match. They match iff 1. All Trains have the same
    ID. 2. OR the ID of the Trains are -1 and the types of the Train
    match.

The matching does consider order, but does not consider direction,
thus 1. 1-2-3 and 3-2-1 match. 2. 1-3-2 and 1-2-3 do not match.)doc";

static const char *__doc_ShuntingUnit_NeedsElectricity =
R"doc(Returns true iff any of this ShuntingUnit's Train%s needs electricity
*/)doc";

static const char *__doc_ShuntingUnit_SetTrains = R"doc(Set the Train%s of this ShuntingUnit */)doc";

static const char *__doc_ShuntingUnit_ShuntingUnit = R"doc()doc";

static const char *__doc_ShuntingUnit_ShuntingUnit_2 = R"doc(Construct a ShuntingUnit with the given id and the given Train%s */)doc";

static const char *__doc_ShuntingUnit_ShuntingUnit_3 = R"doc(Construct a ShuntingUnit from the given protobuf object */)doc";

static const char *__doc_ShuntingUnit_ShuntingUnit_4 = R"doc(The default copy constructor */)doc";

static const char *__doc_ShuntingUnit_UpdateValues = R"doc()doc";

static const char *__doc_ShuntingUnit_id = R"doc()doc";

static const char *__doc_ShuntingUnit_length = R"doc()doc";

static const char *__doc_ShuntingUnit_needsElectricity = R"doc()doc";

static const char *__doc_ShuntingUnit_operator_eq =
R"doc(Returns:
    true iff the two ShuntingUnit%s have the same id */)doc";

static const char *__doc_ShuntingUnit_operator_ne =
R"doc(Returns:
    true iff the two ShuntingUnit%s do not have the same id */)doc";

static const char *__doc_ShuntingUnit_toString = R"doc(Get a string representation of the ShuntingUnit */)doc";

static const char *__doc_ShuntingUnit_trainIDs = R"doc()doc";

static const char *__doc_ShuntingUnit_trainString = R"doc()doc";

static const char *__doc_ShuntingUnit_trains = R"doc()doc";

static const char *__doc_SimpleAction =
R"doc(The abstract base class for simple action descriptions. SimpleAction
describes an action without using pointers and is therefore useful for
serializing and for inputting actions outside of the current runtime.

SimpleActions can be transferred into Actions by using
Engine::GenerateAction or LocationEngine::GenerateAction)doc";

static const char *__doc_SimpleAction_Clone = R"doc(Create a clone of this instance using new)doc";

static const char *__doc_SimpleAction_GetGeneratorName =
R"doc(Get the name of the generator that can turn this SimpleAction into an
Action)doc";

static const char *__doc_SimpleAction_GetTrainIDs = R"doc(Get the train ids of the ShuntingUnit)doc";

static const char *__doc_SimpleAction_GetTrainsToString =
R"doc(Return a string representation of the ShuntingUnit that this action is
acting on.)doc";

static const char *__doc_SimpleAction_SimpleAction = R"doc()doc";

static const char *__doc_SimpleAction_SimpleAction_2 =
R"doc(Construct a SimpleAction for the given ShuntingUnit. The Train ids are
stored.)doc";

static const char *__doc_SimpleAction_SimpleAction_3 = R"doc(Construct a SimpleAction for a ShuntingUnit with the given train ids.)doc";

static const char *__doc_SimpleAction_SimpleAction_4 = R"doc(Default copy constructor)doc";

static const char *__doc_SimpleAction_toString = R"doc(Get a string representation of this action)doc";

static const char *__doc_SimpleAction_trainIDs = R"doc()doc";

static const char *__doc_Split =
R"doc(The Split action splits a ShuntingUnit into two separate ShuntingUnits
based on the splitIndex.)doc";

static const char *__doc_SplitAction = R"doc(The SplitAction splits a ShuntingUnit into two separate ShuntingUnits.)doc";

static const char *__doc_SplitActionGenerator = R"doc(The SplitActionGenerator generates SplitAction%s */)doc";

static const char *__doc_SplitActionGenerator_Generate = R"doc()doc";

static const char *__doc_SplitActionGenerator_Generate_2 = R"doc()doc";

static const char *__doc_SplitActionGenerator_SplitActionGenerator = R"doc()doc";

static const char *__doc_SplitActionGenerator_SplitActionGenerator_2 = R"doc()doc";

static const char *__doc_SplitActionGenerator_SplitActionGenerator_3 = R"doc()doc";

static const char *__doc_SplitAction_Clone = R"doc()doc";

static const char *__doc_SplitAction_CreateSimple = R"doc()doc";

static const char *__doc_SplitAction_Finish = R"doc()doc";

static const char *__doc_SplitAction_GetASideShuntingUnit =
R"doc(Get the resulting ShuntingUnit that will apear on the A-side of the
track. */)doc";

static const char *__doc_SplitAction_GetBSideShuntingUnit =
R"doc(Get the resulting ShuntingUnit that will apear on the B-side of the
track. */)doc";

static const char *__doc_SplitAction_GetSplitIndex =
R"doc(Get the split index of the split.

A ShuntingUnit consisting of Trains A-B-C-D will split as follow with
split index: 1. SU1: A, SU2: B-C-D 2. SU1: A-B, SU2: C-D 3. SU1:
A-B-C, SU2: D)doc";

static const char *__doc_SplitAction_SplitAction = R"doc()doc";

static const char *__doc_SplitAction_SplitAction_2 = R"doc(Construct a SplitAction from the given parameters */)doc";

static const char *__doc_SplitAction_Start = R"doc()doc";

static const char *__doc_SplitAction_suA = R"doc()doc";

static const char *__doc_SplitAction_suB = R"doc()doc";

static const char *__doc_SplitAction_suString = R"doc()doc";

static const char *__doc_SplitAction_toString = R"doc()doc";

static const char *__doc_Split_Clone = R"doc()doc";

static const char *__doc_Split_GetGeneratorName = R"doc()doc";

static const char *__doc_Split_GetSplitIndex =
R"doc(Get the index for the split operation.

A ShuntingUnit consisting of Trains A-B-C-D will split as follow with
split index: 1. SU1: A, SU2: B-C-D 2. SU1: A-B, SU2: C-D 3. SU1:
A-B-C, SU2: D)doc";

static const char *__doc_Split_Split = R"doc()doc";

static const char *__doc_Split_Split_2 =
R"doc(Construct a Split action for the ShuntingUnit described by the train
ids. Split the ShuntingUnit at the specified index */)doc";

static const char *__doc_Split_Split_3 =
R"doc(Construct a Split action for the ShuntingUnit su. Split the
ShuntingUnit at the specified index */)doc";

static const char *__doc_Split_Split_4 = R"doc(The default copy constructor */)doc";

static const char *__doc_Split_splitIndex = R"doc()doc";

static const char *__doc_Split_toString = R"doc()doc";

static const char *__doc_State = R"doc()doc";

static const char *__doc_State_2 =
R"doc(The State class describes the current state of the a session.

All mutable information is stored in the state.)doc";

static const char *__doc_State_AddActiveAction = R"doc(Add an active Action to a ShuntingUnit */)doc";

static const char *__doc_State_AddActiveTaskToTrain = R"doc(Add an active Task to a Train */)doc";

static const char *__doc_State_AddEvent = R"doc(Add an Incoming Event to the EventQueue */)doc";

static const char *__doc_State_AddEvent_2 = R"doc(Add an Outgoing Event to the EventQueue */)doc";

static const char *__doc_State_AddEvent_3 = R"doc(Add an Action finish Event to the EventQueue */)doc";

static const char *__doc_State_AddShuntingUnit = R"doc(Add a ShuntingUnit to the State */)doc";

static const char *__doc_State_AddShuntingUnit_2 = R"doc(Add a ShuntingUnit to the State */)doc";

static const char *__doc_State_AddShuntingUnitOnPosition = R"doc(Add a ShuntingUnit to the State on the given position */)doc";

static const char *__doc_State_AddShuntingUnitToState = R"doc(Add a ShuntingUnitState to the State */)doc";

static const char *__doc_State_AddTaskToTrain = R"doc(Add a Task to a Train */)doc";

static const char *__doc_State_AddTasksToTrains = R"doc(Add tasks a train as given by the map Train -> vector<Task> */)doc";

static const char *__doc_State_CanMoveToSide = R"doc(Return true if the given ShuntingUnit can move to the given Track */)doc";

static const char *__doc_State_FinishAction = R"doc(Execute the finish of the given Action */)doc";

static const char *__doc_State_FreeTrack = R"doc(Remove the Track reservation for the given Track */)doc";

static const char *__doc_State_FreeTracks = R"doc(Remove the Track reservation for the given Track%s */)doc";

static const char *__doc_State_FreeTracks_2 = R"doc(Remove the Track reservation for the given Track%s */)doc";

static const char *__doc_State_GetActiveActions = R"doc(Returns all the current active Actions for the given ShuntingUnit */)doc";

static const char *__doc_State_GetActiveTasksForTrain =
R"doc(Get a list of all the Task%s for the given Train that are currently
being executed */)doc";

static const char *__doc_State_GetAmountOnTrack = R"doc(Get the number of ShuntingUnit%s on the given Track */)doc";

static const char *__doc_State_GetEndTime = R"doc(Get the end time of this Scenario */)doc";

static const char *__doc_State_GetFrontTrain = R"doc(Get the front Train for the given ShuntingUnit */)doc";

static const char *__doc_State_GetIncomingByID = R"doc(Get the Incoming event by the given id */)doc";

static const char *__doc_State_GetIncomingTrains = R"doc(Get the Incoming trains */)doc";

static const char *__doc_State_GetMatchingShuntingUnit =
R"doc(Returns a matching ShuntingUnit on the shunting yard. See
ShuntingUnit::MatchesShuntingUnit */)doc";

static const char *__doc_State_GetNumberOfEvents = R"doc(Get the number of Event%s in the EventQueue */)doc";

static const char *__doc_State_GetOccupations = R"doc(Get all the ShuntingUnit%s at Track track */)doc";

static const char *__doc_State_GetOutgoingByID = R"doc(Get the Outgoing event by the given id */)doc";

static const char *__doc_State_GetOutgoingTrains = R"doc(Get the Outgoing trains */)doc";

static const char *__doc_State_GetPosition = R"doc(Get the position of the ShuntingUnit su */)doc";

static const char *__doc_State_GetPositionOnTrack =
R"doc(Get the position of a ShuntingUnit on a Track, with 0 refering to the
ShuntingUnit at the A-side */)doc";

static const char *__doc_State_GetPrevious = R"doc(Get the previous position of the ShuntingUnit su */)doc";

static const char *__doc_State_GetReservedTracks = R"doc(Get all the reserved Track%s */)doc";

static const char *__doc_State_GetShuntingUnitByID = R"doc(Get the ShuntingUnit by ShuntingUnit id */)doc";

static const char *__doc_State_GetShuntingUnitByTrainID =
R"doc(Get the ShuntingUnit that currently contains the Train with the given
id */)doc";

static const char *__doc_State_GetShuntingUnitByTrainIDs =
R"doc(Get the ShuntingUnit that currently contains the Train%s with the
given ids, or null if not found*/)doc";

static const char *__doc_State_GetShuntingUnitState = R"doc(Get the ShuntingUnitState for the given ShuntingUnit */)doc";

static const char *__doc_State_GetShuntingUnitStates =
R"doc(Get the ShuntingUnitState%s for all the ShuntingUnit%s in the shunting
yard */)doc";

static const char *__doc_State_GetShuntingUnits = R"doc(Return all ShuntingUnit%s on the shunting yard */)doc";

static const char *__doc_State_GetStartTime = R"doc(Get the start time of this Scenario */)doc";

static const char *__doc_State_GetTasksForTrain =
R"doc(Get a vector of all the Task%s for the given Train that have not been
executed yet */)doc";

static const char *__doc_State_GetTime = R"doc(Get the State's time */)doc";

static const char *__doc_State_GetTrainByTrainID = R"doc(Get the Train with the given id */)doc";

static const char *__doc_State_GetTrainUnitsInOrder =
R"doc(Get the Train%s of the given ShuntingUnit in order, based on its
direction */)doc";

static const char *__doc_State_HasActiveAction =
R"doc(Returns true if the given ShuntingUnit currently has an active Action
*/)doc";

static const char *__doc_State_HasShuntingUnit = R"doc(Return true iff the given ShuntingUnit is on the Shunting yard */)doc";

static const char *__doc_State_HasShuntingUnitByID = R"doc(Returns true if a ShuntingUnit with this id exists */)doc";

static const char *__doc_State_InsertOnTrack =
R"doc(Insert the given ShuntingUnit on a specific position on the given
track described by the tuple (previous, park) */)doc";

static const char *__doc_State_IsActionRequired =
R"doc(Returns true if any ShuntingUnit is inactive or if an Incoming train
is available */)doc";

static const char *__doc_State_IsActive =
R"doc(Returns true if any of the ShuntingUnit%s is currently active or
waiting */)doc";

static const char *__doc_State_IsAnyInactive =
R"doc(Returns true if any of the ShuntingUnit%s is currently not active or
waiting */)doc";

static const char *__doc_State_IsBeginMoving =
R"doc(Return true if the given ShuntingUnit is currently has just started
moving */)doc";

static const char *__doc_State_IsChanged =
R"doc(Returns true if this state has changed since the last time it was set
to unchanged */)doc";

static const char *__doc_State_IsInNeutral =
R"doc(Return true if the given ShuntingUnit is currently in neutral position
(to be updated) */)doc";

static const char *__doc_State_IsMoving = R"doc(Return true if the given ShuntingUnit is currently moving */)doc";

static const char *__doc_State_IsReserved = R"doc(Return true if the given Track is currently reserved */)doc";

static const char *__doc_State_IsWaiting = R"doc(Return true if the given ShuntingUnit is currently waiting */)doc";

static const char *__doc_State_MoveShuntingUnit =
R"doc(Change the position of the ShuntingUnit to the new position described
by the tuple (previous, to) */)doc";

static const char *__doc_State_OccupyTrack =
R"doc(Occupy the given position describes by the tuple (previous, park) with
the given ShuntingUnit */)doc";

static const char *__doc_State_PeekEvent = R"doc(Get the first Event in the EventQueue */)doc";

static const char *__doc_State_PopEvent = R"doc(Get and remove the first Event from the EventQueue */)doc";

static const char *__doc_State_PrintStateInfo = R"doc(Print the state info */)doc";

static const char *__doc_State_RemoveActiveAction = R"doc(Rmove the Action from the list of active Action%s */)doc";

static const char *__doc_State_RemoveActiveTaskFromTrain = R"doc(Remove the active Task%s for a given Train from the state */)doc";

static const char *__doc_State_RemoveIncoming = R"doc(Remove the Incoming event from the list of incoming trains */)doc";

static const char *__doc_State_RemoveOccupation = R"doc(Remove a ShuntingUnit from its current Track occupation */)doc";

static const char *__doc_State_RemoveOutgoing = R"doc(Remove the Outgoing event from the list of outgoing trains */)doc";

static const char *__doc_State_RemoveShuntingUnit = R"doc(Rmove the ShuntingUnit from the shunting yard */)doc";

static const char *__doc_State_RemoveTaskFromTrain = R"doc(Remove the Task%s for a given Train from the state */)doc";

static const char *__doc_State_ReserveTrack = R"doc(Reserve the Track */)doc";

static const char *__doc_State_ReserveTracks = R"doc(Reserve the Track%s */)doc";

static const char *__doc_State_ReserveTracks_2 = R"doc(Reserve the Track%s */)doc";

static const char *__doc_State_SetBeginMoving = R"doc(Set the ShuntingUnit's begin moving state */)doc";

static const char *__doc_State_SetFrontTrain = R"doc(Set the front Train of the ShuntingUnit */)doc";

static const char *__doc_State_SetInNeutral = R"doc(Set the ShuntingUnit's neutral state (to be updated) */)doc";

static const char *__doc_State_SetMoving = R"doc(Set the ShuntingUnit's moving state */)doc";

static const char *__doc_State_SetPosition = R"doc(Set the ShuntingUnit's current position */)doc";

static const char *__doc_State_SetPrevious = R"doc(Set the ShuntingUnit's previous position */)doc";

static const char *__doc_State_SetTime = R"doc(Set the State's time */)doc";

static const char *__doc_State_SetUnchanged = R"doc(Set this state to unchanged */)doc";

static const char *__doc_State_SetWaiting = R"doc(Set the ShuntingUnit's waiting state */)doc";

static const char *__doc_State_StartAction = R"doc(Execute the start of the given Action */)doc";

static const char *__doc_State_State = R"doc()doc";

static const char *__doc_State_State_2 =
R"doc(Construct an initial State object from a Scenario object and a vector
of Track%s in a Location */)doc";

static const char *__doc_State_State_3 = R"doc(Default copy constructor */)doc";

static const char *__doc_State_SwitchFrontTrain = R"doc(Switch the front Train of the ShuntingUnit */)doc";

static const char *__doc_State_changed = R"doc()doc";

static const char *__doc_State_employees = R"doc()doc";

static const char *__doc_State_endTime = R"doc()doc";

static const char *__doc_State_events = R"doc()doc";

static const char *__doc_State_incomingTrains = R"doc()doc";

static const char *__doc_State_outgoingTrains = R"doc()doc";

static const char *__doc_State_shuntingUnitStates = R"doc()doc";

static const char *__doc_State_shuntingUnits = R"doc()doc";

static const char *__doc_State_startTime = R"doc()doc";

static const char *__doc_State_time = R"doc()doc";

static const char *__doc_State_trackStates = R"doc()doc";

static const char *__doc_State_trainIDToShuntingUnit = R"doc()doc";

static const char *__doc_State_trainIDToTrain = R"doc()doc";

static const char *__doc_State_trainStates = R"doc()doc";

static const char *__doc_Task = R"doc(A Task struct describing a task for a Train)doc";

static const char *__doc_Task_Serialize = R"doc(Serialize this Task to a protobuf object */)doc";

static const char *__doc_Task_Task = R"doc()doc";

static const char *__doc_Task_Task_2 = R"doc(Construct a Task object given the parameters */)doc";

static const char *__doc_Task_Task_3 = R"doc(Construct a Task object given the protobuf object */)doc";

static const char *__doc_Task_duration = R"doc(< the duration of this Task in seconds */)doc";

static const char *__doc_Task_operator_eq =
R"doc(Two Task%s are equal if they have the same task type, priority and
duration */)doc";

static const char *__doc_Task_operator_ne =
R"doc(Two Task%s are different if they ahve differnt task type, or priority
or duration */)doc";

static const char *__doc_Task_priority = R"doc(< the priority of this Task (0 = mandatory, 1 = optional) */)doc";

static const char *__doc_Task_skills = R"doc(< the skills required to execute this task (not yet implemented) */)doc";

static const char *__doc_Task_taskType = R"doc(< the type of this Task */)doc";

static const char *__doc_Task_toString = R"doc(Get a string representation of the Task */)doc";

static const char *__doc_TimeShift =
R"doc(A TimeShift describes a time window with a start and end time in
seconds */)doc";

static const char *__doc_TimeShift_end = R"doc(< This TimeShift's end time in seconds */)doc";

static const char *__doc_TimeShift_start = R"doc(< This TimeShift's start time in seconds */)doc";

static const char *__doc_Track = R"doc(A Track describes a track part and its connections.)doc";

static const char *__doc_Track_2 = R"doc(A Track describes a track part and its connections.)doc";

static const char *__doc_TrackPartType =
R"doc(TrackPartType describes the type of a track part.

From a graph perspective, the Railroad tracks are 'nodes', the other
types (except Building) represent connections.)doc";

static const char *__doc_TrackPartType_Building = R"doc(< A Railroad with a building on/over it */)doc";

static const char *__doc_TrackPartType_Bumper =
R"doc(< A piece of track which stops at some point. Here, a_side contains
the piece of track and b_side does not exist, as the track doesn't
continue.)doc";

static const char *__doc_TrackPartType_EnglishSwitch =
R"doc(< A piece of track with two pieces of track on both ends. Here, both
a_sides tracks are connected to both b_sides tracks, but not a_side to
a_side or b_side to b_side.)doc";

static const char *__doc_TrackPartType_HalfEnglishSwitch =
R"doc(< A piece of track with two pieces of track on both ends.Here,
a_sides[0] is connected to both b_sides tracks, while a_sides[1] is
only connected to b_sides[1].)doc";

static const char *__doc_TrackPartType_InterSection =
R"doc(< A piece of track with two pieces of track on both ends. Here,
a_sides[0] connects to b_sides[1] and a_sides[1] connects to
b_sides[0].)doc";

static const char *__doc_TrackPartType_Railroad = R"doc(< A piece of track with one piece of track on both ends. */)doc";

static const char *__doc_TrackPartType_Switch =
R"doc(< A piece of track with one piece of track on one end and two pieces
of track on the other end)doc";

static const char *__doc_TrackState = R"doc(The TrackState struct describes the state of a Track)doc";

static const char *__doc_TrackState_TrackState = R"doc(Default constructor */)doc";

static const char *__doc_TrackState_occupations =
R"doc(< A list of all the ShuntingUnit%s currently on this Track, ordered
from A-side to B-side */)doc";

static const char *__doc_TrackState_reserved = R"doc(< True iff this Track is currently reserved */)doc";

static const char *__doc_Track_AddFacility = R"doc(Add a Facility to this track)doc";

static const char *__doc_Track_AssignNeighbors = R"doc(Assign the neighbors for this track)doc";

static const char *__doc_Track_GetASideTracks = R"doc(Get the A-side tracks)doc";

static const char *__doc_Track_GetBSideTracks = R"doc(Get the B-side tracks)doc";

static const char *__doc_Track_GetFacilities = R"doc(Get all the facilities that are located at this track)doc";

static const char *__doc_Track_GetID = R"doc(Get the id of this track)doc";

static const char *__doc_Track_GetLength = R"doc(Get the length of this track)doc";

static const char *__doc_Track_GetNeighbors = R"doc(Get all the neighbors of this track, both A-side and B-side.)doc";

static const char *__doc_Track_GetNextTrackParts =
R"doc(Get the next track parts from this track, if the previous track is
previous

For example: 1. If the type of this track is Railroad, and the A-side
track is given as previous, the B-side track is return 2. If the type
of this track is Intersection and aSides[0] is given, bSides[1] is
returned 3. If the type of this track is Switch and the A-side track
is given, both B-side tracks are returned.)doc";

static const char *__doc_Track_GetOppositeSide = R"doc(Get a neighbor of this track at the opposite side to previous)doc";

static const char *__doc_Track_GetType = R"doc(Get the TrackPartType of this track)doc";

static const char *__doc_Track_IsASide =
R"doc(Returns:
    true iff track t is part of this track's A-side)doc";

static const char *__doc_Track_IsBSide =
R"doc(Returns:
    true iff track t is part of this track's B-side)doc";

static const char *__doc_Track_IsNeighbor =
R"doc(Returns:
    true iff track t is a neighbor of this track, that is if it is
    either at the A-side or B-side of this track.)doc";

static const char *__doc_Track_IsSameSide =
R"doc(Returns:
    true iff track t1 and t2 are both at the same side for this track,
    that is, either both at the A-side or both at the B-side.)doc";

static const char *__doc_Track_Track = R"doc()doc";

static const char *__doc_Track_Track_2 = R"doc(Construct a track from the given parameters)doc";

static const char *__doc_Track_Track_3 = R"doc(Construct a track from the protobuf object)doc";

static const char *__doc_Track_Track_4 = R"doc(Get a copy of the track)doc";

static const char *__doc_Track_aSides = R"doc(< the A-side tracks of this track */)doc";

static const char *__doc_Track_bSides = R"doc(< the B-side tracks of this track */)doc";

static const char *__doc_Track_facilities = R"doc(< the facilities located at this track */)doc";

static const char *__doc_Track_id = R"doc(< The id of this track */)doc";

static const char *__doc_Track_isElectrified = R"doc(< Whether this track is electrified or not */)doc";

static const char *__doc_Track_length = R"doc(< The length of thist track */)doc";

static const char *__doc_Track_name = R"doc(< The name of this track */)doc";

static const char *__doc_Track_next =
R"doc(a map connecting neighboring tracks to reachable tracks if the key
track is the previous track */)doc";

static const char *__doc_Track_operator_eq =
R"doc(Returns:
    true iff the id of this track equals the id of the other track)doc";

static const char *__doc_Track_operator_ne =
R"doc(Returns:
    true iff the id of this track does not equal the id of the other
    track)doc";

static const char *__doc_Track_parkingAllowed = R"doc(< Whether parking is allowed on this track */)doc";

static const char *__doc_Track_sawMovementAllowed = R"doc(< Whether saw movements are allowed on this track */)doc";

static const char *__doc_Track_toString = R"doc(Get a string representation of this track)doc";

static const char *__doc_Track_type = R"doc(< The type of this track */)doc";

static const char *__doc_Train =
R"doc(A description of a Train unit.

A Train unit is an atomic Train in the shunting yard. It can consist
of several carriages, but those carriages cannot be split. Trains can
be combined with other Trains to form ShuntingUnit%s)doc";

static const char *__doc_TrainEquals = R"doc()doc";

static const char *__doc_TrainEquals_operator_call = R"doc()doc";

static const char *__doc_TrainGoal = R"doc(The abstract base class for the Incoming and Outgoing classes)doc";

static const char *__doc_TrainGoal_GetID = R"doc(Get the id of this TrainGoal */)doc";

static const char *__doc_TrainGoal_GetParkingTrack = R"doc(Get the parking Track for this TrainGoal */)doc";

static const char *__doc_TrainGoal_GetShuntingUnit = R"doc(Return the ShuntingUnit */)doc";

static const char *__doc_TrainGoal_GetSideTrack =
R"doc(Get the side Track for this TrainGoal.

If Incoming, the Track from which the ShuntingUnit arrives If
Outgoing, the Track from which the ShuntingUnit departs)doc";

static const char *__doc_TrainGoal_GetStandingIndex =
R"doc(If instanding, return the position index on the Track of this
ShuntingUnit */)doc";

static const char *__doc_TrainGoal_GetTasks =
R"doc(Get all the tasks, defined per Train, for this ShuntingUnit (only
Incoming) */)doc";

static const char *__doc_TrainGoal_GetTime = R"doc(Return the time of arrival (Incoming) or departure (Outgoing) */)doc";

static const char *__doc_TrainGoal_IsInstanding = R"doc(Return true iff this ShuntingUnit is instanding */)doc";

static const char *__doc_TrainGoal_Serialize = R"doc(Serialize this object to a protobuf object */)doc";

static const char *__doc_TrainGoal_TrainGoal = R"doc()doc";

static const char *__doc_TrainGoal_TrainGoal_2 = R"doc(Construct this TrainGoal from the provided parameters */)doc";

static const char *__doc_TrainGoal_TrainGoal_3 = R"doc(Construct this TrainGoal from the provided parameters */)doc";

static const char *__doc_TrainGoal_TrainGoal_4 = R"doc(Construct this TrainGoal from the provided parameters */)doc";

static const char *__doc_TrainGoal_TrainGoal_5 = R"doc(Copy this TrainGoal */)doc";

static const char *__doc_TrainGoal_assignTracks = R"doc(Assign the Track%s for this TrainGoal */)doc";

static const char *__doc_TrainGoal_id = R"doc(< The id of this TrainGoal */)doc";

static const char *__doc_TrainGoal_isInstanding =
R"doc(< True if this ShuntingUnit is standing on the shunting yard before
the Scenario starts (Incoming) or after it ends (Outgoing))doc";

static const char *__doc_TrainGoal_operator_eq =
R"doc(Return true iff both are the same type (Incoming vs Outgoing) and have
the same id */)doc";

static const char *__doc_TrainGoal_operator_ne =
R"doc(Return false iff both are the same type (Incoming vs Outgoing) or have
the same id */)doc";

static const char *__doc_TrainGoal_parkingTrack = R"doc(< The parking track for the ShuntingUnit */)doc";

static const char *__doc_TrainGoal_shuntingUnit = R"doc(< The arriving/departing ShuntingUnit */)doc";

static const char *__doc_TrainGoal_sideTrack =
R"doc(< The side track for teh ShuntingUnit (Bumper track) If Incoming, the
Track from which the ShuntingUnit arrives If Outgoing, the Track from
which the ShuntingUnit departs)doc";

static const char *__doc_TrainGoal_standingIndex =
R"doc(< If instanding, the position index on the Track of this ShuntingUnit
*/)doc";

static const char *__doc_TrainGoal_tasks = R"doc(All Task%s for the Train%s in this ShuntingUnit */)doc";

static const char *__doc_TrainGoal_time = R"doc(< The time of arriving/departing */)doc";

static const char *__doc_TrainGoal_toString = R"doc(Get a string representation of this object */)doc";

static const char *__doc_TrainHash = R"doc(//!\cond NO_DOC)doc";

static const char *__doc_TrainHash_operator_call = R"doc()doc";

static const char *__doc_TrainState = R"doc(The TrainState struct describes the state of a Train)doc";

static const char *__doc_TrainState_activeTasks =
R"doc(< A vector of all the Task%s for this Train that are currently being
executed */)doc";

static const char *__doc_TrainState_tasks =
R"doc(< A vector containing all the Task%s for this Train that have not been
executed yet */)doc";

static const char *__doc_TrainUnitType = R"doc(The TrainUnitType describes the type of the Train)doc";

static const char *__doc_TrainUnitType_Serialize = R"doc(Serialize this TrainUnitType to a protobuf object */)doc";

static const char *__doc_TrainUnitType_TrainUnitType = R"doc()doc";

static const char *__doc_TrainUnitType_TrainUnitType_2 =
R"doc(Construct a TrainUnitType from the given parameters. The setback time
is equal to the number of carriages of this type times the back
addition time)doc";

static const char *__doc_TrainUnitType_TrainUnitType_3 = R"doc(Construct a TrainUnitType from the given protobuf object */)doc";

static const char *__doc_TrainUnitType_backAdditionTime = R"doc(< The back addition time */)doc";

static const char *__doc_TrainUnitType_backNormTime = R"doc(< the back norm time */)doc";

static const char *__doc_TrainUnitType_carriages = R"doc(< The number of carriages */)doc";

static const char *__doc_TrainUnitType_combineDuration = R"doc(< The time it takes to combine this type of train unit with another */)doc";

static const char *__doc_TrainUnitType_displayName = R"doc(< The name of the train unit type */)doc";

static const char *__doc_TrainUnitType_isLoco = R"doc(< Whether or not this type of train unit is a locomotive */)doc";

static const char *__doc_TrainUnitType_length = R"doc(< The length of this train unit, in meters */)doc";

static const char *__doc_TrainUnitType_needsElectricity =
R"doc(< Whether or not this type of train unit can only drive over
electrified tracks */)doc";

static const char *__doc_TrainUnitType_needsLoco = R"doc(< Whether or not this type of train unit needs a locomotive to move */)doc";

static const char *__doc_TrainUnitType_operator_eq = R"doc(Returns true iff the two TrainUnitType%s have the same name */)doc";

static const char *__doc_TrainUnitType_operator_ne = R"doc(Returns true iff the two TrainUnitType%s do not have the same name */)doc";

static const char *__doc_TrainUnitType_setbackTime =
R"doc(< The time needed to setback this type of train unit, calculated as
#carriages * #backAdditionTime */)doc";

static const char *__doc_TrainUnitType_splitDuration = R"doc(< The time it takes to split this type of train unit */)doc";

static const char *__doc_TrainUnitType_startUpTime =
R"doc(< The time it takes for this type of train unit to start up / shut
down */)doc";

static const char *__doc_TrainUnitType_toString = R"doc(Get a string representation of this TrainUnitType */)doc";

static const char *__doc_TrainUnitType_travelSpeed = R"doc(< The travel speed of the train */)doc";

static const char *__doc_TrainUnitType_typePrefix =
R"doc(< The prefix for this type of train unit, for example "SLT" or "VIRM"
*/)doc";

static const char *__doc_Train_GetID = R"doc(Get the id of the Train */)doc";

static const char *__doc_Train_GetType = R"doc(Get the type of the train */)doc";

static const char *__doc_Train_Serialize = R"doc(Serialize this Train to a protobuf object */)doc";

static const char *__doc_Train_SetID = R"doc(Set the id (if the id is yet not set (-1)) */)doc";

static const char *__doc_Train_Train = R"doc()doc";

static const char *__doc_Train_Train_2 = R"doc(Construct a Train from the given parameters */)doc";

static const char *__doc_Train_Train_3 = R"doc(Construct a Train from a protobuf object */)doc";

static const char *__doc_Train_Train_4 = R"doc(The default copy constructor */)doc";

static const char *__doc_Train_id = R"doc()doc";

static const char *__doc_Train_operator_eq =
R"doc(Two trains are equal if they have the same id (provided it is not -1)
or if they are the same object */)doc";

static const char *__doc_Train_operator_ne =
R"doc(Two trains are different if they have a different id or if one id is
-1, and they are not the same object */)doc";

static const char *__doc_Train_toString = R"doc(Get a string representation of the Train */)doc";

static const char *__doc_Train_type = R"doc()doc";

static const char *__doc_Wait =
R"doc(The Wait action instructs this ShuntingUnit to wait until the next
Event.)doc";

static const char *__doc_WaitAction =
R"doc(The WaitAction instructs this ShuntingUnit to wait until the next
Event.)doc";

static const char *__doc_WaitActionGenerator = R"doc(The WaitActionGenerator generates WaitAction%s */)doc";

static const char *__doc_WaitActionGenerator_Generate = R"doc()doc";

static const char *__doc_WaitActionGenerator_Generate_2 = R"doc()doc";

static const char *__doc_WaitActionGenerator_WaitActionGenerator = R"doc()doc";

static const char *__doc_WaitActionGenerator_WaitActionGenerator_2 = R"doc()doc";

static const char *__doc_WaitActionGenerator_WaitActionGenerator_3 = R"doc()doc";

static const char *__doc_WaitAction_Clone = R"doc()doc";

static const char *__doc_WaitAction_CreateSimple = R"doc()doc";

static const char *__doc_WaitAction_Finish = R"doc()doc";

static const char *__doc_WaitAction_Start = R"doc()doc";

static const char *__doc_WaitAction_WaitAction = R"doc()doc";

static const char *__doc_WaitAction_WaitAction_2 = R"doc(Construct a WaitAction with duration in seconds */)doc";

static const char *__doc_WaitAction_toString = R"doc()doc";

static const char *__doc_Wait_Clone = R"doc()doc";

static const char *__doc_Wait_GetGeneratorName = R"doc()doc";

static const char *__doc_Wait_Wait = R"doc()doc";

static const char *__doc_Wait_Wait_2 = R"doc()doc";

static const char *__doc_Wait_Wait_3 = R"doc()doc";

static const char *__doc_Wait_Wait_4 = R"doc()doc";

static const char *__doc_Wait_toString = R"doc()doc";

static const char *__doc_available_facility_rule =
R"doc(Rule that verifies that tasks assigned to a facility are only executed
when that facility is available.)doc";

static const char *__doc_available_facility_rule_IsValid = R"doc()doc";

static const char *__doc_available_facility_rule_available_facility_rule = R"doc()doc";

static const char *__doc_available_facility_rule_available_facility_rule_2 = R"doc()doc";

static const char *__doc_blocked_first_track_rule =
R"doc(Rule that verifies that shunting units, upon starting a movement, are
not blocked on exit by other shunting units on their current track.)doc";

static const char *__doc_blocked_first_track_rule_IsValid = R"doc()doc";

static const char *__doc_blocked_first_track_rule_blocked_first_track_rule = R"doc()doc";

static const char *__doc_blocked_first_track_rule_blocked_first_track_rule_2 = R"doc()doc";

static const char *__doc_blocked_track_rule =
R"doc(Rule that verifies that moving shunting units are not blocked by other
shunting units.)doc";

static const char *__doc_blocked_track_rule_IsValid = R"doc()doc";

static const char *__doc_blocked_track_rule_blocked_track_rule = R"doc()doc";

static const char *__doc_blocked_track_rule_blocked_track_rule_2 = R"doc()doc";

static const char *__doc_capacity_facility_rule =
R"doc(Rule that verifies that no more tasks are executed at a facility than
the facility can handle.)doc";

static const char *__doc_capacity_facility_rule_IsValid = R"doc()doc";

static const char *__doc_capacity_facility_rule_capacity_facility_rule = R"doc()doc";

static const char *__doc_capacity_facility_rule_capacity_facility_rule_2 = R"doc()doc";

static const char *__doc_copy_of = R"doc()doc";

static const char *__doc_correct_facility_rule =
R"doc(Rule that verifies that service tasks are executed at the correct
facility.)doc";

static const char *__doc_correct_facility_rule_IsValid = R"doc()doc";

static const char *__doc_correct_facility_rule_correct_facility_rule = R"doc()doc";

static const char *__doc_correct_facility_rule_correct_facility_rule_2 = R"doc()doc";

static const char *__doc_disabled_facility_rule =
R"doc(Rule that verifies that no tasks are assigned to facilities which are
disabled by a disturbance.)doc";

static const char *__doc_disabled_facility_rule_IsValid = R"doc()doc";

static const char *__doc_disabled_facility_rule_disabled_facility_rule = R"doc()doc";

static const char *__doc_disabled_facility_rule_disabled_facility_rule_2 = R"doc()doc";

static const char *__doc_electric_move_rule =
R"doc(Rule that verifies that shunting units which need electricity park
only on electrified tracks.)doc";

static const char *__doc_electric_move_rule_IsValid = R"doc()doc";

static const char *__doc_electric_move_rule_electric_move_rule = R"doc()doc";

static const char *__doc_electric_move_rule_electric_move_rule_2 = R"doc()doc";

static const char *__doc_electric_track_rule =
R"doc(Rule that verifies that shunting units which need electricity park
only on electrified tracks.)doc";

static const char *__doc_electric_track_rule_IsValid = R"doc()doc";

static const char *__doc_electric_track_rule_electric_track_rule = R"doc()doc";

static const char *__doc_electric_track_rule_electric_track_rule_2 = R"doc()doc";

static const char *__doc_end_correct_order_on_track_rule =
R"doc(Rule that verifies that shunting units which stay in the shunting yard
after the scheduling period will be located in the right order on
their track.)doc";

static const char *__doc_end_correct_order_on_track_rule_IsValid = R"doc()doc";

static const char *__doc_end_correct_order_on_track_rule_end_correct_order_on_track_rule = R"doc()doc";

static const char *__doc_end_correct_order_on_track_rule_end_correct_order_on_track_rule_2 = R"doc()doc";

static const char *__doc_hash_combine = R"doc()doc";

static const char *__doc_in_correct_time_rule =
R"doc(Rule that verifies that shunting units that are arriving, arrive at
the correct time. Note: shunting units will never arrive too early, so
this rule only checks if a shunting unit arrives too late.)doc";

static const char *__doc_in_correct_time_rule_IsValid = R"doc()doc";

static const char *__doc_in_correct_time_rule_in_correct_time_rule = R"doc()doc";

static const char *__doc_in_correct_time_rule_in_correct_time_rule_2 = R"doc()doc";

static const char *__doc_instanceof = R"doc()doc";

static const char *__doc_legal_on_parking_track_rule =
R"doc(Rule that verifies that parked shunting units are on a track where
parking is allowed.)doc";

static const char *__doc_legal_on_parking_track_rule_IsValid = R"doc()doc";

static const char *__doc_legal_on_parking_track_rule_legal_on_parking_track_rule = R"doc()doc";

static const char *__doc_legal_on_parking_track_rule_legal_on_parking_track_rule_2 = R"doc()doc";

static const char *__doc_legal_on_setback_track_rule =
R"doc(Rule that verifies if a shunting unit is parked on a track where
setback is allowed.)doc";

static const char *__doc_legal_on_setback_track_rule_IsValid = R"doc()doc";

static const char *__doc_legal_on_setback_track_rule_legal_on_setback_track_rule = R"doc()doc";

static const char *__doc_legal_on_setback_track_rule_legal_on_setback_track_rule_2 = R"doc()doc";

static const char *__doc_length_track_rule =
R"doc(Rule that verifies that shunting units on a single track do not take
up more space than available on that track.)doc";

static const char *__doc_length_track_rule_IsValid = R"doc()doc";

static const char *__doc_length_track_rule_length_track_rule = R"doc()doc";

static const char *__doc_length_track_rule_length_track_rule_2 = R"doc()doc";

static const char *__doc_mandatory_service_task_rule =
R"doc(Rule that verifies that all required service tasks are performed
before a shunting unit leaves the shunting yard.)doc";

static const char *__doc_mandatory_service_task_rule_IsValid = R"doc()doc";

static const char *__doc_mandatory_service_task_rule_mandatory_service_task_rule = R"doc()doc";

static const char *__doc_mandatory_service_task_rule_mandatory_service_task_rule_2 = R"doc()doc";

static const char *__doc_operator_lshift = R"doc()doc";

static const char *__doc_operator_lshift_2 = R"doc()doc";

static const char *__doc_operator_lshift_3 = R"doc()doc";

static const char *__doc_operator_lshift_4 = R"doc()doc";

static const char *__doc_operator_lshift_5 = R"doc()doc";

static const char *__doc_operator_lshift_6 = R"doc()doc";

static const char *__doc_operator_lshift_7 = R"doc()doc";

static const char *__doc_operator_lshift_8 = R"doc()doc";

static const char *__doc_operator_lshift_9 = R"doc()doc";

static const char *__doc_operator_lshift_10 = R"doc()doc";

static const char *__doc_operator_lshift_11 = R"doc()doc";

static const char *__doc_operator_lshift_12 = R"doc()doc";

static const char *__doc_operator_lshift_13 = R"doc()doc";

static const char *__doc_operator_lshift_14 = R"doc()doc";

static const char *__doc_operator_lshift_15 = R"doc()doc";

static const char *__doc_operator_lshift_16 = R"doc()doc";

static const char *__doc_operator_lshift_17 = R"doc()doc";

static const char *__doc_operator_lshift_18 = R"doc()doc";

static const char *__doc_optional_service_task_rule =
R"doc(Rule that verifies that all optional service tasks are performed
before a shunting unit leaves the shunting yard.)doc";

static const char *__doc_optional_service_task_rule_IsValid = R"doc()doc";

static const char *__doc_optional_service_task_rule_optional_service_task_rule = R"doc()doc";

static const char *__doc_optional_service_task_rule_optional_service_task_rule_2 = R"doc()doc";

static const char *__doc_order_preserve_rule =
R"doc(Rule that verifies that combining or splitting shunting units does not
change the order of train units on a track.)doc";

static const char *__doc_order_preserve_rule_IsValid = R"doc()doc";

static const char *__doc_order_preserve_rule_order_preserve_rule = R"doc()doc";

static const char *__doc_order_preserve_rule_order_preserve_rule_2 = R"doc()doc";

static const char *__doc_out_correct_order_rule =
R"doc(Rule that verifies that leaving shunting units have their train units
in the correct order when they leave the shunting yard.)doc";

static const char *__doc_out_correct_order_rule_IsValid = R"doc()doc";

static const char *__doc_out_correct_order_rule_out_correct_order_rule = R"doc()doc";

static const char *__doc_out_correct_order_rule_out_correct_order_rule_2 = R"doc()doc";

static const char *__doc_out_correct_time_rule =
R"doc(Rule that verifies that leaving shunting units leave at the correct
time.)doc";

static const char *__doc_out_correct_time_rule_IsValid = R"doc()doc";

static const char *__doc_out_correct_time_rule_out_correct_time_rule = R"doc()doc";

static const char *__doc_out_correct_time_rule_out_correct_time_rule_2 = R"doc()doc";

static const char *__doc_out_correct_track_rule =
R"doc(Rule that verifies that leaving shunting units leave over the correct
tracks.)doc";

static const char *__doc_out_correct_track_rule_IsValid = R"doc()doc";

static const char *__doc_out_correct_track_rule_out_correct_track_rule = R"doc()doc";

static const char *__doc_out_correct_track_rule_out_correct_track_rule_2 = R"doc()doc";

static const char *__doc_park_combine_split_rule =
R"doc(Rule that verifies that combine and split actions on shunting units
are only performed on tracks where parking is allowed.)doc";

static const char *__doc_park_combine_split_rule_IsValid = R"doc()doc";

static const char *__doc_park_combine_split_rule_park_combine_split_rule = R"doc()doc";

static const char *__doc_park_combine_split_rule_park_combine_split_rule_2 = R"doc()doc";

static const char *__doc_parse_json_to_pb = R"doc()doc";

static const char *__doc_parse_json_to_pb_2 = R"doc()doc";

static const char *__doc_parse_pb_to_json = R"doc()doc";

static const char *__doc_parse_pb_to_json_2 = R"doc()doc";

static const char *__doc_setback_combine_split_rule =
R"doc(Rule that verifies that combine and split actions on shunting units
are only performed on tracks where setback is allowed.)doc";

static const char *__doc_setback_combine_split_rule_IsValid = R"doc()doc";

static const char *__doc_setback_combine_split_rule_setback_combine_split_rule = R"doc()doc";

static const char *__doc_setback_combine_split_rule_setback_combine_split_rule_2 = R"doc()doc";

static const char *__doc_setback_once_rule =
R"doc(Rule that verifies that a setback action is not performed on a
shunting unit which is already in a neutral state. A shunting unit is
in a neutral state if a setback or service action is performed.)doc";

static const char *__doc_setback_once_rule_IsValid = R"doc()doc";

static const char *__doc_setback_once_rule_setback_once_rule = R"doc()doc";

static const char *__doc_setback_once_rule_setback_once_rule_2 = R"doc()doc";

static const char *__doc_setback_track_rule =
R"doc(Rule that verifies that performing a setback action on a shunting unit
is allowed on the track where the shunting unit is at.)doc";

static const char *__doc_setback_track_rule_IsValid = R"doc()doc";

static const char *__doc_setback_track_rule_setback_track_rule = R"doc()doc";

static const char *__doc_setback_track_rule_setback_track_rule_2 = R"doc()doc";

static const char *__doc_single_move_track_rule =
R"doc(Rule that verifies that at most one shunting unit can use a piece of
track at a given time.)doc";

static const char *__doc_single_move_track_rule_IsValid = R"doc()doc";

static const char *__doc_single_move_track_rule_single_move_track_rule = R"doc()doc";

static const char *__doc_single_move_track_rule_single_move_track_rule_2 = R"doc()doc";

static const char *__doc_understaffed_rule =
R"doc(Rule that verifies that all tasks have enough employees assigned, with
the right skills, such that the task will have all of its required
skills available.)doc";

static const char *__doc_understaffed_rule_IsValid = R"doc()doc";

static const char *__doc_understaffed_rule_understaffed_rule = R"doc()doc";

static const char *__doc_understaffed_rule_understaffed_rule_2 = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

