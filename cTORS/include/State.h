/** \file State.h
 * Describes the State class
 */
#pragma once
#ifndef STATE_H
#define STATE_H
#include "Scenario.h"
using namespace std;

/** 
 * The ShuntingUnitState struct describes the state of a ShuntingUnit
 * 
 * Note: The current implementation of the direction of the ShuntingUnit is not correct yet and will be changed.
 */
struct ShuntingUnitState {
	const Track* position; 				/**< The position of the ShuntingUnit */
	const Track* previous;				/**< The previous position of the ShuntingUnit */
	list<const Action*> activeActions;	/**< The list of active Action%s for this ShuntingUnit */
	bool moving,						/**< True iff the ShuntingUnit is currently moving */
		waiting,						/**< True iff the ShuntingUnit is curently waiting */
		inNeutral,						/**< True iff the ShuntingUnit is currently in neutral position */
		beginMoving;					/**< True iff the ShuntingUnit has just started movign */
	const Train* frontTrain;			/**< A reference to the current front Train of the ShuntingUnit */
	//The shunting unit's direction is described by previous and frontTrain.


	ShuntingUnitState() = delete;
	/** Copy constructor */
	ShuntingUnitState(const ShuntingUnitState& suState) : position(suState.position), previous(suState.previous),
		moving(suState.moving), waiting(suState.waiting), inNeutral(suState.inNeutral), beginMoving(suState.beginMoving), frontTrain(suState.frontTrain) {
			for(auto action: suState.activeActions) activeActions.emplace_back(action->Clone());
		}
	/** Iniitialize the ShuntingUnitState based on the given parameters */
	ShuntingUnitState(const Track* position, const Track* previous, const Train* frontTrain) 
		: position(position), previous(previous), moving(false), waiting(false), inNeutral(false), beginMoving(false), frontTrain(frontTrain) {}
	/** Destroy the ShuntingUnitState and its stored active actions */
	~ShuntingUnitState() {
		DELETE_LIST(activeActions);
	}
	/** Returns true iff the ShuntingUnit has active Action%s */
	bool HasActiveAction() const { return activeActions.size() > 0; }
};

/**
 * The TrackState struct describes the state of a Track
 */
struct TrackState {
	list<const ShuntingUnit*> occupations;	/**< A list of all the ShuntingUnit%s currently on this Track, ordered from A-side to B-side */
	bool reserved;							/**< True iff this Track is currently reserved */
	/** Default constructor */
	TrackState() : reserved(false) {};
};

/** 
 * The TrainState struct describes the state of a Train 
 */
struct TrainState {
	vector<Task> tasks;			/**< A vector containing all the Task%s for this Train that have not been executed yet */
	vector<Task> activeTasks;	/**< A vector of all the Task%s for this Train that are currently being executed */
};

/**
 * The State class describes the current state of the a session.
 * 
 * All mutable information is stored in the state.
 */
class State
{
private:
	EventQueue events;
	int time, startTime, endTime;
	vector<const Incoming*> incomingTrains;
	vector<const Outgoing*> outgoingTrains;
	vector<const Employee*> employees;
	vector<const ShuntingUnit*> shuntingUnits;
	
	unordered_map<const ShuntingUnit*, ShuntingUnitState, ShuntingUnitHash, ShuntingUnitEquals> shuntingUnitStates;
	unordered_map<const Track*, TrackState> trackStates;
	unordered_map<const Train*, TrainState, TrainHash, TrainEquals> trainStates;
	map<int, const ShuntingUnit*> trainIDToShuntingUnit;
	map<int, const Train*> trainIDToTrain;
	bool changed;
public:
	State() = delete;
	/** Construct an initial State object from a Scenario object and a vector of Track%s in a Location */
	State(const Scenario& scenario, const vector<Track*>& tracks);
	/** Default copy constructor */
	State(const State& state) = default;
	/** Destroy the State */
	~State();

	//Events
	/** Get the number of Event%s in the EventQueue */
	inline size_t GetNumberOfEvents() const { return events.size(); }
	/** Get the first Event in the EventQueue */
	const Event* PeekEvent() const;
	/** Get and remove the first Event from the EventQueue */
	const Event* PopEvent();
	/** Add an Incoming Event to the EventQueue */
	void AddEvent(const Incoming* in);
	/** Add an Outgoing Event to the EventQueue */
	void AddEvent(const Outgoing* out);
	/** Add an Action finish Event to the EventQueue */
	void AddEvent(const Action* action);

	//Time
	/** Set the State's time */
	void SetTime(int time);
	/** Get the State's time */
	inline int GetTime() const { return time; };
	/** Get the end time of this Scenario */
	inline int GetEndTime() const { return endTime; }
	/** Get the start time of this Scenario */
	inline int GetStartTime() const { return startTime; }
	
	//Changed
	/** Returns true if this state has changed since the last time it was set to unchanged */
	inline bool IsChanged() const { return changed; }
	/** Set this state to unchanged */
	inline void SetUnchanged() { changed = false; }
	
	//Apply action
	/** Execute the start of the given Action */
	void StartAction(const Action* action);
	/** Execute the finish of the given Action */
	void FinishAction(const Action* action);

	//Getters
	/** Get the Incoming trains */
	inline const vector<const Incoming*>& GetIncomingTrains() const { return incomingTrains; }
	/** Get the Outgoing trains */
	inline const vector<const Outgoing*>& GetOutgoingTrains() const { return outgoingTrains; }
	/** Get the position of the ShuntingUnit su */
	inline const Track* GetPosition(const ShuntingUnit* su) const { return_ce(shuntingUnitStates.at(su).position); }
	/** Get the previous position of the ShuntingUnit su */
	inline const Track* GetPrevious(const ShuntingUnit* su) const { return_ce(shuntingUnitStates.at(su).previous); }
	/** Get all the ShuntingUnit%s at Track track */
	inline const list<const ShuntingUnit*>& GetOccupations(const Track* track) const { return_ce(trackStates.at(track).occupations); }
	/** Get all the reserved Track%s */
	const vector<const Track*> GetReservedTracks() const;
	/** Get the position of a ShuntingUnit on a Track, with 0 refering to the ShuntingUnit at the A-side */
	int GetPositionOnTrack(const ShuntingUnit* su) const;
	/** Get the number of ShuntingUnit%s on the given Track */
	inline size_t GetAmountOnTrack(const Track* track) const { return GetOccupations(track).size(); }
	/** Get the Train%s of the given ShuntingUnit in order, based on its direction */
	const vector<Train> GetTrainUnitsInOrder(const ShuntingUnit* su) const;
	/** Return true if the given ShuntingUnit can move to the given Track */
	bool CanMoveToSide(const ShuntingUnit* su, const Track* side) const;
	/** Return true if the given ShuntingUnit is currently moving */
	inline bool IsMoving(const ShuntingUnit* su) const { return_ce(shuntingUnitStates.at(su).moving); }
	/** Return true if the given Track is currently reserved */
	inline bool IsReserved(const Track* track) const { return_ce(trackStates.at(track).reserved); }
	/** Return true if the given ShuntingUnit is currently waiting */
	inline bool IsWaiting(const ShuntingUnit* su) const { return_ce(shuntingUnitStates.at(su).waiting); }
	/** Return true if the given ShuntingUnit is currently in neutral position (to be updated) */
	inline bool IsInNeutral(const ShuntingUnit* su) const { return_ce(shuntingUnitStates.at(su).inNeutral); }
	/** Return true if the given ShuntingUnit is currently has just started moving */
	inline bool IsBeginMoving(const ShuntingUnit* su) const { return_ce(shuntingUnitStates.at(su).beginMoving); }
	/** Return all ShuntingUnit%s on the shunting yard */
	inline const vector<const ShuntingUnit*> GetShuntingUnits() const { return shuntingUnits; }
	/** Return true iff the given ShuntingUnit is on the Shunting yard */
	bool HasShuntingUnit(const ShuntingUnit* su) const;
	/** Returns a matching ShuntingUnit on the shunting yard. See ShuntingUnit::MatchesShuntingUnit */
	const ShuntingUnit* GetMatchingShuntingUnit(const ShuntingUnit* su) const;
	/** Returns true if the given ShuntingUnit currently has an active Action */
	inline bool HasActiveAction(const ShuntingUnit* su) const { return_ce(GetActiveActions(su).size() > 0); }
	/** Returns all the current active Actions for the given ShuntingUnit */
	inline const list<const Action*> &GetActiveActions(const ShuntingUnit* su) const { return_ce(shuntingUnitStates.at(su).activeActions); }
	/** Returns true if any of the ShuntingUnit%s is currently active or waiting */
	bool IsActive() const;
	/** Returns true if any of the ShuntingUnit%s is currently not active or waiting */
	bool IsAnyInactive() const;
	/**Returns true if any ShuntingUnit is inactive or if an Incoming train is available */
	bool IsActionRequired() const;
	/** Get the front Train for the given ShuntingUnit */
	inline const Train* GetFrontTrain(const ShuntingUnit* su) const { return_ce(shuntingUnitStates.at(su).frontTrain); }
	/** Get a vector of all the Task%s for the given Train that have not been executed yet */
	inline const vector<Task>& GetTasksForTrain(const Train* tu) const { return_ce(trainStates.at(tu).tasks); }
	/** Get a list of all the Task%s for the given Train that are currently being executed */
	inline const vector<Task>& GetActiveTasksForTrain(const Train* tu) const { return_ce(trainStates.at(tu).activeTasks); }
	/** Get the ShuntingUnitState for the given ShuntingUnit */
	inline const ShuntingUnitState& GetShuntingUnitState(const ShuntingUnit* su) const { return_ce(shuntingUnitStates.at(su)); }
	/** Get the ShuntingUnitState%s for all the ShuntingUnit%s in the shunting yard */
	inline const unordered_map<const ShuntingUnit*, ShuntingUnitState, ShuntingUnitHash, ShuntingUnitEquals>& GetShuntingUnitStates() const { return shuntingUnitStates; }
	/** Get the ShuntingUnit by ShuntingUnit id */
	const ShuntingUnit* GetShuntingUnitByID(int id) const;
	/** Returns true if a ShuntingUnit with this id exists */
	inline const bool HasShuntingUnitByID(int id) const { return GetShuntingUnitByID(id) != nullptr; }
	/** Get the ShuntingUnit that currently contains the Train with the given id */
	inline const ShuntingUnit* GetShuntingUnitByTrainID(int id) const { return_ce(trainIDToShuntingUnit.at(id)); }
	/** Get the Train with the given id */
	inline const Train* GetTrainByTrainID(int id) const { return_ce(trainIDToTrain.at(id)); }
	/** Get the ShuntingUnit that currently contains the Train%s with the given ids, or null if not found*/
	const ShuntingUnit* GetShuntingUnitByTrainIDs(const vector<int>& ids) const;
	/** Get the Incoming event by the given id */
	const Incoming* GetIncomingByID(int id) const;
	/** Get the Outgoing event by the given id */
	const Outgoing* GetOutgoingByID(int id) const;

	//Setters and Adders
	/** Set the ShuntingUnit's moving state */
	inline void SetMoving(const ShuntingUnit* su, bool b) { ce(shuntingUnitStates.at(su).moving = b); }
	/** Set the ShuntingUnit's waiting state */
	inline void SetWaiting(const ShuntingUnit* su, bool b) { ce(shuntingUnitStates.at(su).waiting = b); }
	/** Set the ShuntingUnit's neutral state (to be updated) */
	inline void SetInNeutral(const ShuntingUnit* su, bool b) { ce(shuntingUnitStates.at(su).inNeutral = b); }
	/** Set the ShuntingUnit's begin moving state */
	inline void SetBeginMoving(const ShuntingUnit* su, bool b) { ce(shuntingUnitStates.at(su).beginMoving = b); }
	/** Set the ShuntingUnit's current position */
	inline void SetPosition(const ShuntingUnit* su, const Track* track) { ce(shuntingUnitStates.at(su).position = track); }
	/** Set the ShuntingUnit's previous position */
	inline void SetPrevious(const ShuntingUnit* su, const Track* track) { ce(shuntingUnitStates.at(su).previous = track); }

	/** Add tasks a train as given by the map Train -> vector<Task>  */
	void AddTasksToTrains(const unordered_map<const Train*, vector<Task>, TrainHash, TrainEquals>& tasks);
	/** Add a Task to a Train */
	inline void AddTaskToTrain(const Train* tu, const Task& task) { ce(trainStates.at(tu).tasks.push_back(task)); }
	/** Add an active Task to a Train */
	inline void AddActiveTaskToTrain(const Train* tu, const Task& task) { ce(trainStates.at(tu).activeTasks.push_back(task)); }
	/** Add an active Action to a ShuntingUnit */
	inline void AddActiveAction(const ShuntingUnit* su, const Action* action) { ce(shuntingUnitStates.at(su).activeActions.push_back(action->Clone())); }
	/** Add a ShuntingUnitState to the State */
	const ShuntingUnit* AddShuntingUnitToState(const ShuntingUnit* su, const Track* track, const Track* previous, const Train* frontTrain);
	/** Add a ShuntingUnit to the State */
	void AddShuntingUnit(const ShuntingUnit* su, const Track* track, const Track* previous, const Train* frontTrain);
	/** Add a ShuntingUnit to the State */
	inline void AddShuntingUnit(const ShuntingUnit* su, const Track* track, const Track* previous) { AddShuntingUnit(su, track, previous, &su->GetTrains().front()); }
	/** Add a ShuntingUnit to the State on the given position */
	void AddShuntingUnitOnPosition(const ShuntingUnit* su, const Track* track, const Track* previous, const Train* frontTrain, int positionOnTrack);
	/** Set the front Train of the ShuntingUnit */
	inline void SetFrontTrain(const ShuntingUnit* su, const Train* frontTrain) { ce(shuntingUnitStates.at(su).frontTrain = frontTrain); }
	/** Switch the front Train of the ShuntingUnit */
	void SwitchFrontTrain(const ShuntingUnit* su);
	
	//Track Reservation
	/** Reserve the Track%s */
	void ReserveTracks(const vector<const Track*>& tracks);
	/** Reserve the Track%s */
	void ReserveTracks(const list<const Track*>& tracks);
	/** Reserve the Track */
	inline void ReserveTrack(const Track* track) { ce(trackStates.at(track).reserved = true); };
	/** Remove the Track reservation for the given Track%s */
	void FreeTracks(const vector<const Track*>& tracks);
	/** Remove the Track reservation for the given Track%s */
	void FreeTracks(const list<const Track*>& tracks);
	/** Remove the Track reservation for the given Track */
	inline void FreeTrack(const Track* track) { ce(trackStates.at(track).reserved = false); };

	//Moving
	/** Change the position of the ShuntingUnit to the new position described by the tuple (previous, to) */
	void MoveShuntingUnit(const ShuntingUnit* su, const Track* to, const Track* previous);
	/** Occupy the given position describes by the tuple (previous, park) with the given ShuntingUnit */
	void OccupyTrack(const ShuntingUnit* su, const Track* park, const Track* previous);
	/** Insert the given ShuntingUnit on a specific position on the given track described by the tuple (previous, park) */
	void InsertOnTrack(const ShuntingUnit* su, const Track* park, const Track* previous, int positionOnTrack);
	
	//Remove
	/** Remove the Incoming event from the list of incoming trains */
	void RemoveIncoming(const Incoming* incoming);
	/** Remove the Outgoing event from the list of outgoing trains */
	void RemoveOutgoing(const Outgoing* outgoing);
	/** Rmove the ShuntingUnit from the shunting yard */
	void RemoveShuntingUnit(const ShuntingUnit* su);
	/** Rmove the Action from the list of active Action%s */
	void RemoveActiveAction(const ShuntingUnit* su, const Action* action);
	/** Remove a ShuntingUnit from its current Track occupation */
	void RemoveOccupation(const ShuntingUnit* su);
	/** Remove the Task%s for a given Train from the state */
	void RemoveTaskFromTrain(const Train* tu, const Task& task);
	/** Remove the active Task%s for a given Train from the state */
	void RemoveActiveTaskFromTrain(const Train* tu, const Task& task);

	/** Print the state info */
	void PrintStateInfo() const;
};

#endif