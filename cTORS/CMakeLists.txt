cmake_minimum_required(VERSION 3.11)

# set the project name
project(cTORS VERSION 0.1)

# Verbose.
#set(CMAKE_VERBOSE_MAKEFILE on)

include(FetchContent)


# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set release build.
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
	message(STATUS "Build type not specified: Use Release by default")
endif ()

#Fetch the JSON library
FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Headers
set(PROJECT_SOURCE_DIR "src")
set(PROJECT_INCLUDE_DIR "include")

set(TORS_SOURCE_FILES
	${PROJECT_SOURCE_DIR}/action/Action.cpp
	${PROJECT_SOURCE_DIR}/action/ActionManager.cpp
	${PROJECT_SOURCE_DIR}/action/ActionValidator.cpp
	${PROJECT_SOURCE_DIR}/action/ArriveAction.cpp
	${PROJECT_SOURCE_DIR}/action/BeginMoveAction.cpp
	${PROJECT_SOURCE_DIR}/action/EndMoveAction.cpp
	${PROJECT_SOURCE_DIR}/action/ExitAction.cpp
	${PROJECT_SOURCE_DIR}/action/MoveAction.cpp
	${PROJECT_SOURCE_DIR}/action/ServiceAction.cpp
	${PROJECT_SOURCE_DIR}/action/SetbackAction.cpp
	${PROJECT_SOURCE_DIR}/action/SplitAction.cpp
	${PROJECT_SOURCE_DIR}/action/CombineAction.cpp
	${PROJECT_SOURCE_DIR}/action/WaitAction.cpp
	
	${PROJECT_SOURCE_DIR}/rules/arrival_departure/end_correct_order_on_track_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/arrival_departure/in_correct_time_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/arrival_departure/out_correct_order_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/arrival_departure/out_correct_time_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/arrival_departure/out_correct_track_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/combine_and_split/order_preserve_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/combine_and_split/park_combine_split_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/combine_and_split/setback_combine_split_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/facility/available_facility_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/facility/capacity_facility_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/facility/disabled_facility_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/parking/electric_track_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/parking/legal_on_parking_track_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/parking/legel_on_setback_track_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/service_tasks/correct_facility_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/service_tasks/mandatory_service_task_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/service_tasks/optional_service_task_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/service_tasks/understaffed_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/shunting/electric_move_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/shunting/setback_once_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/shunting/setback_track_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/track_occupation/blocked_first_track_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/track_occupation/blocked_track_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/track_occupation/length_track_rule.cpp
	${PROJECT_SOURCE_DIR}/rules/track_occupation/single_move_track_rule.cpp
	
	${PROJECT_SOURCE_DIR}/employees/Employee.cpp

	${PROJECT_SOURCE_DIR}/engine/Config.cpp
	${PROJECT_SOURCE_DIR}/engine/Engine.cpp

	${PROJECT_SOURCE_DIR}/location/Facility.cpp
	${PROJECT_SOURCE_DIR}/location/Location.cpp
	${PROJECT_SOURCE_DIR}/location/Track.cpp
	
	${PROJECT_SOURCE_DIR}/scenario/Disturbance.cpp
	${PROJECT_SOURCE_DIR}/scenario/Event.cpp
	${PROJECT_SOURCE_DIR}/scenario/EventQueue.cpp
	${PROJECT_SOURCE_DIR}/scenario/Scenario.cpp
	
	${PROJECT_SOURCE_DIR}/state/State.cpp
	
	${PROJECT_SOURCE_DIR}/vehicles/Incoming.cpp
	${PROJECT_SOURCE_DIR}/vehicles/Outgoing.cpp
	${PROJECT_SOURCE_DIR}/vehicles/ShuntingUnit.cpp
	${PROJECT_SOURCE_DIR}/vehicles/Train.cpp
	${PROJECT_SOURCE_DIR}/vehicles/TrainGoal.cpp
	
	${PROJECT_SOURCE_DIR}/main.cpp

	# Includes
	${PROJECT_INCLUDE_DIR}/Action.h
	${PROJECT_INCLUDE_DIR}/BusinessRules.h
	${PROJECT_INCLUDE_DIR}/Config.h
	${PROJECT_INCLUDE_DIR}/Disturbance.h
	${PROJECT_INCLUDE_DIR}/Employee.h
	${PROJECT_INCLUDE_DIR}/Engine.h
	${PROJECT_INCLUDE_DIR}/Event.h
	${PROJECT_INCLUDE_DIR}/EventQueue.h
	${PROJECT_INCLUDE_DIR}/Exceptions.h
	${PROJECT_INCLUDE_DIR}/Facility.h
	${PROJECT_INCLUDE_DIR}/Location.h
	${PROJECT_INCLUDE_DIR}/Scenario.h
	${PROJECT_INCLUDE_DIR}/ShuntingUnit.h
	${PROJECT_INCLUDE_DIR}/State.h
	${PROJECT_INCLUDE_DIR}/Track.h
	${PROJECT_INCLUDE_DIR}/TrainGoals.h
	${PROJECT_INCLUDE_DIR}/Train.h
	${PROJECT_INCLUDE_DIR}/Utils.h
)

add_library(cTORS SHARED ${TORS_SOURCE_FILES})

target_include_directories(cTORS PUBLIC ${PROJECT_INCLUDE_DIR})
target_link_libraries(cTORS PUBLIC nlohmann_json::nlohmann_json)



