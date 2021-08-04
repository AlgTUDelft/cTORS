/** \file Action.h
 * Describes the Action, SimpleAction classes with their derivatives
 * and the ActionGenerator with its derivates, the ActionValidator and the ActionManager
 */
#pragma once
#ifndef ACTION_H
#define ACTION_H
#include "TrainGoals.h"
#include "Employee.h"
#include "Config.h"
#include "Location.h"

using namespace std;

class ShuntingUnit;
class Track;
class BusinessRule;
class State;


/////////////////////////////////////
////// Simple Action Classes  ///////
/////////////////////////////////////

/**
 * The abstract base class for simple action descriptions.
 * SimpleAction describes an action without using pointers and is therefore useful for serializing
 * and for inputting actions outside of the current runtime.
 * 
 * SimpleActions can be transferred into Actions by using Engine::GenerateAction or LocationEngine::GenerateAction
 */
class SimpleAction {
private:
	vector<int> trainIDs;
protected:
	/**
	 * Return a string representation of the ShuntingUnit that this action is acting on.
	 */
	const string GetTrainsToString() const;

public:
	SimpleAction() = delete;

	/**
	 * Construct a SimpleAction for the given ShuntingUnit. The Train ids are stored.
	 */
	SimpleAction(const ShuntingUnit* su) : trainIDs(su->GetTrainIDs()) {}
	
	/**
	 * Construct a SimpleAction for a ShuntingUnit with the given train ids.
	 */
	SimpleAction(const vector<int>& trainIDs) : trainIDs(trainIDs) {}

	/**
	 * Default copy constructor
	 */
	SimpleAction(const SimpleAction& sa) = default;
	
	/**
	 * Get the train ids of the ShuntingUnit
	 */
	inline const vector<int>& GetTrainIDs() const { return trainIDs; }
	
	/**
	 * Get a string representation of this action
	 */
	virtual const string toString() const = 0;

	/**
	 * Get the name of the generator that can turn this SimpleAction into an Action
	 */
	virtual const string GetGeneratorName() const = 0;
	
	/**
	 * Create a clone of this instance using new
	 */
	virtual const SimpleAction* Clone() const = 0;
};

#ifndef SIMPLE_ACTION_DEFINE
/** MACRO for defining SimpleAction's subclasses to prevent duplicate code */
#define SIMPLE_ACTION_DEFINE(name, generator_name) \
class name : public SimpleAction { \
public: \
	name() = delete; \
	/** \
	 * Construct a name for the given ShuntingUnit. The Train ids are stored. \
	 */ \
	name(const ShuntingUnit* su) : SimpleAction(su) {} \
	/** \
	 * Construct a name for a ShuntingUnit with the given train ids. \
	 */ \
	name(const vector<int>& trainIDs) : SimpleAction(trainIDs) {} \
	/** \
	 * Default copy constructor \
	 */ \
	name(const name& action) = default; \
	inline const string toString() const override { return #name ": "  + GetTrainsToString(); } \
	inline const string GetGeneratorName() const override { return generator_name; } \
	inline const name* Clone() const override { return new name(*this); } \
};
#endif

/**
 * The BeginMove action changes the state of a ShuntingUnit to moving.
 * After this action the ShuntingUnit can be moved.
 */
SIMPLE_ACTION_DEFINE(BeginMove, "move_helper")

/**
 * The EndMove action changes the state of a ShuntingUnit to not moving.
 * After this action the ShuntingUnit can no longer be moved.
 */
SIMPLE_ACTION_DEFINE(EndMove, "move_helper")

/**
 * The Wait action instructs this ShuntingUnit to wait until the next Event.
 */
SIMPLE_ACTION_DEFINE(Wait, "wait")

/**
 * The Setback action changes the direction of the ShuntingUnit
 */
SIMPLE_ACTION_DEFINE(Setback, "set_back")

/**
 * The Service action executes a task on the specified train.
 * 
 * Note that service tasks are executed on trains, not on shunting units. The reason is that trains are persistent and atomic. 
 * Shunting units can change.
 */
class Service : public SimpleAction {
private:
	const Task task;
	const Train train;
	int facilityID;
public:
	Service() = delete;
	
	/** Construct a Service action from the given parameters */
	Service(const vector<int>& trainIDs, const Task& task, const Train& train, int facilityID)
		: SimpleAction(trainIDs), task(task), train(train), facilityID(facilityID) {}
	
	/** Construct a Service action from the given parameters */
	Service(const vector<int>& trainIDs, const Task& task, const Train& train, const Facility* facility)
		: Service(trainIDs, task, train, facility->GetID()) {}
	
	/** Construct a Service action from the given parameters */
	Service(const ShuntingUnit* su, const Task& task, const Train& train, const Facility* facility)
		: Service(su->GetTrainIDs(), task, train, facility->GetID()) {}
	
	/** Default copy constructor */
	Service(const Service& service) = default;
	
	/** Get the service Task */
	inline const Task& GetTask() const { return task; }

	/** Get the Train that the Task should be operated on */
	inline const Train& GetTrain() const { return train; }
	
	/** Get the id of the Facility where the Task is operated */
	inline int GetFacilityID() const { return facilityID; }
	
	inline const string toString() const override {
		return "Service: " + GetTrainsToString() + " perform " + GetTask().toString() + " on " + GetTrain().toString() + " at facility " + to_string(facilityID);
	}
	inline const string GetGeneratorName() const override { return "service"; }
	inline const Service* Clone() const override { return new Service(*this); }
};

/**
 * The Arrive action lets a scheduled ShuntingUnit arrive on the shunting yard.
 */
class Arrive : public SimpleAction {
private:
	int incomingID;
public:
	Arrive() = delete;
	/** Construct an Arrive action from the Incoming event */
	Arrive(const Incoming* inc) : SimpleAction(inc->GetShuntingUnit()), incomingID(inc->GetID()) {}
	/** Default copy constructor */
	Arrive(const Arrive& arrive) = default;
	/** Get the id of the Incoming event */
	inline const int GetIncomingID() const { return incomingID; }
	inline const string toString() const override { return "Arrive: " + GetTrainsToString(); }
	inline const string GetGeneratorName() const override { return "arrive"; }
	inline const Arrive* Clone() const override { return new Arrive(*this); }
};

/**
 * The Exit action lets a scheduled ShuntingUnit depart from the shunting yard.
 */
class Exit : public SimpleAction {
private:
	int outgoingID;
public:
	Exit() = delete;
	
	/**
	 * Construct an Exit action for the ShuntingUnit described by the train ids and the 
	 * id of the Outgoing event.
	 * 
	 * The ShuntingUnit attribute of the Outgoing event may refer to unspecified trains, that is trains of 
	 * some type, with no set id. Therefore the train ids of the ShuntingUnit in the shunting yard need to 
	 * be specified seperately.
	 */
	Exit(const vector<int>& ids, int outgoingID) : SimpleAction(ids), outgoingID(outgoingID) {}
	
	/**
	 * Construct an Exit action for the ShuntingUnit and the Outgoing event.
	 * 
	 * The ShuntingUnit attribute of the Outgoing event may refer to unspecified trains, that is trains of 
	 * some type, with no set id. Therefore the ShuntingUnit in the shunting yard need to 
	 * be specified seperately.
	 */
	Exit(const ShuntingUnit* su, const Outgoing* out) : Exit(su->GetTrainIDs(), out->GetID()) {}
	/** The default copy constructor */
	Exit(const Exit& exit) = default;
	/** Get the id of the Outgoing event */
	inline const int GetOutgoingID() const { return outgoingID; }
	inline const string toString() const override { return "Exit: " + GetTrainsToString(); }
	inline const string GetGeneratorName() const override { return "exit"; }
	inline const Exit* Clone() const override { return new Exit(*this); }
};

/**
 * The Move action moves a ShuntingUnit from one Track to a neighboring Railroad Track.
 */
class Move : public SimpleAction {
private:
	string destinationID;
public:
	Move() = delete;
	/** Construct a Move action for the ShuntingUnit described by the train ids to the Track with id destinationID */
	Move(const vector<int>& trainIDs, string destinationID) : SimpleAction(trainIDs), destinationID(destinationID) {}
	/** Construct a Move action for the ShuntingUnit described by the train ids to the destination Track */
	Move(const vector<int>& trainIDs, const Track* destination) : Move(trainIDs, destination->GetID()) {}
	/** Construct a Move action for the ShuntingUnit su to the destination Track */
	Move(const ShuntingUnit* su, const Track* destination) : Move(su->GetTrainIDs(), destination->GetID()) {}
	/** Default copy constructor */
	Move(const Move& move) = default;
	/** Get the id of the destination track */
	inline const string& GetDestinationID() const { return destinationID; }
	inline const string toString() const override { return "Move: " + GetTrainsToString() + " to track with id " + destinationID; }
	inline const string GetGeneratorName() const override { return "move"; }
	inline const Move* Clone() const override { return new Move(*this); }
};

/**
 * The MultiMove action moves a ShuntingUnit from one Track to another Railroad Track.
 */
class MultiMove : public SimpleAction {
private:
	vector<string> trackIDs;
public:
	MultiMove() = delete;
	/** Construct a MultiMove action for the ShuntingUnit described by the train ids over the given list of Track IDs */
	MultiMove(const vector<int>& trainIDs, const vector<string>& trackIDs) : SimpleAction(trainIDs), trackIDs(trackIDs) {}
	/** Construct a MultiMove action for the ShuntingUnit described by the train ids over the given vector of Track%s */
	MultiMove(const vector<int>& trainIDs, const vector<const Track*>& tracks);
	/** Construct a MultiMove action for the ShuntingUnit su over the given vector of Track%s */
	MultiMove(const ShuntingUnit* su, const vector<const Track*>& tracks);
	/** Construct a MultiMove action for the ShuntingUnit described by the train ids over the given list of Track%s */
	MultiMove(const vector<int>& trainIDs, const list<const Track*>& tracks);
	/** Construct a MultiMove action for the ShuntingUnit su over the given list of Track%s */
	MultiMove(const ShuntingUnit* su, const list<const Track*>& tracks);
	/** Default copy constructor */
	MultiMove(const MultiMove& move) = default;
	/** Get the id of the destination track */
	inline const string& GetDestinationID() const { return trackIDs.back(); }
	/** Get the Track IDs of the route */
	inline const vector<string>& GetTrackIDs() const {return trackIDs; }
	inline const string toString() const override { return "Move: " + GetTrainsToString() + " along path " + Join(trackIDs," - "); }
	inline const string GetGeneratorName() const override { return "move"; }
	inline const MultiMove* Clone() const override { return new MultiMove(*this); }
};

/**
 * The Split action splits a ShuntingUnit into two separate ShuntingUnits based on the splitIndex.
 */
class Split : public SimpleAction {
private:
	int splitIndex;
public:
	Split() = delete;
	/** Construct a Split action for the ShuntingUnit described by the train ids. Split the ShuntingUnit at the specified index */
	Split(const vector<int>& trainIDs, const int splitIndex) : SimpleAction(trainIDs), splitIndex(splitIndex) {}
	/** Construct a Split action for the ShuntingUnit su. Split the ShuntingUnit at the specified index */
	Split(const ShuntingUnit* su, const int splitIndex) : Split(su->GetTrainIDs(), splitIndex) {}
	/** The default copy constructor */
	Split(const Split& s) = default;
	
	/**
	 * Get the index for the split operation.
	 * 
	 * A ShuntingUnit consisting of Trains A-B-C-D will split as follow with split index:
	 * 1. SU1: A, SU2: B-C-D
	 * 2. SU1: A-B, SU2: C-D
	 * 3. SU1: A-B-C, SU2: D
	 */
	inline const int GetSplitIndex() const { return splitIndex; }
	inline const string toString() const override { return "Split: " + GetTrainsToString() + " at position " +to_string(splitIndex); }
	inline const string GetGeneratorName() const override { return "split"; }
	inline const Split* Clone() const override { return new Split(*this); }
};

/**
 * The Combine action combines two ShuntingUnits into one ShuntingUnit.
 */
class Combine : public SimpleAction {
private:
	vector<int> secondTrainIDs;
protected:
	/** Get a string representation of the second ShuntingUnit */
	const string GetSecondTrainsToString() const;
public:
	Combine() = delete;
	/** Construct a Combine action for the two shunting units described by the train ids. */
	Combine(const vector<int>& trainIDs, const vector<int>& secondTrainIDs)
		: SimpleAction(trainIDs), secondTrainIDs(secondTrainIDs) {}
	/** Construct a Combine action for the two shunting units su and secondShuntingUnit */
	Combine(const ShuntingUnit* su, const ShuntingUnit* secondShuntingUnit)
		: Combine(su->GetTrainIDs(), secondShuntingUnit->GetTrainIDs()) {}
	/** Default copy constructor */
	Combine(const Combine& c) = default;
	/** Get the IDs of the second ShuntingUnit */
	inline const vector<int>& GetSecondTrainIDs() const { return secondTrainIDs; }
	inline const string toString() const override { return "Combine: " + GetTrainsToString() + " and " + GetSecondTrainsToString(); }
	inline const string GetGeneratorName() const override { return "combine"; }
	inline const Combine* Clone() const override { return new Combine(*this); }
};


/////////////////////////////////////
//////      Action Classes    ///////
/////////////////////////////////////

#ifndef ACTION_OVERRIDE
/** MACRO for defining some of Action's subclass' methods to prevent duplicate code */
#define ACTION_OVERRIDE(name) \
	void Start(State* state) const override; \
	void Finish(State* state) const override; \
	inline name* Clone() const override { return new name(*this); } \
	const string toString() const override;
#endif

/**
 * The abstract base class for action descriptions.
 * Action describes an action more extensively than SimpleAction.
 * It it therefore used to check for validity and to execute.
 * 
 * SimpleActions can be transferred into Actions by using Engine::GenerateAction or LocationEngine::GenerateAction
 */
class Action
{
private:
	static int newUID;
protected:
	const int uid;								/**< The unique id of this Action */
	const ShuntingUnit* su;						/**< The ShuntingUnit that this Action operates on */
	const int duration;							/**< The duration in seconds for this Action to complete */
	const vector<const Employee*> employees;	/**< The Employees involved in this Action (not yet implemented) */
	const vector<const Track*> reserved;		/**< A list of tracks that are reserved during this Action */
public:
	Action() = delete;
	/** Construct an Action from the given parameters */
	Action(const ShuntingUnit* su, vector<const Track*> reserved, vector<const Employee*> employees, int duration) 
		: uid(newUID++), su(su), reserved(reserved), employees(employees), duration(duration) {};
	/** Construct an Action from the given parameters */
	Action(const ShuntingUnit* su, const Employee* employee, int duration)
		: Action(su, {}, vector<const Employee*> {employee}, duration) {};
	/** Construct an Action from the given parameters */
	Action(const ShuntingUnit* su, vector<const Track*> reserved, const Employee* employee, int duration) 
		: Action(su, reserved, vector<const Employee*> {employee}, duration) {};
	/** Construct an Action from the given parameters */
	Action(const ShuntingUnit* su) : Action(su, {}, {}, -1) { };
	/** Default copy constructor */
	Action(const Action& action) = default;
	/** Default destructor  */
	virtual ~Action() = default;
	
	/**
	 * Get the duration of this action.
	 */
	inline int GetDuration() const { return duration; }
	
	/**
	 * Get the ShuntingUnit that this action operates on.
	 */
	inline const ShuntingUnit* GetShuntingUnit() const { return su; }
	
	/**
	 * Get the employees that are involved with this action (employees is not yet implemented).
	 */
	inline const vector<const Employee*>& GetEmployees() const { return employees; }
	
	/**
	 * Start the execution action and modify the state accordingly.
	 */
	virtual void Start(State* state) const = 0;
	
	/**
	 * End the execution of the action and modify the state accordingly.
	 */
	virtual void Finish(State* state) const = 0;

	/**
	 * Generate the SimpleAction equivalent of this action.
	 */
	virtual const SimpleAction* CreateSimple() const = 0;

	/**
	 * Create a copy of this action using new
	 */
	virtual Action* Clone() const = 0;

	/**
	 * Generate a string representation of this action.
	 */
	virtual const string toString() const = 0;

	/**
	 * Check if two actions are equal by comparing their unique id.
	 */
	inline const bool IsEqual(const Action& a) const { return uid == a.uid; }
	
	/**
	 * Check if two actions are equal by comparing their unique id.
	 */
	inline virtual bool operator==(const Action& a) const { return IsEqual(a); }
	
	/**
	 * Check if two actions are not equal by comparing their unique id.
	 */
	inline virtual bool operator!=(const Action& a) const { return !IsEqual(a); }

	/**
	 * Get the Tracks that are reserved by this action.
	 */
	inline const vector<const Track*>&  GetReservedTracks() const { return reserved; }
};

/**
 * The ArriveAction lets a scheduled ShuntingUnit arrive on the shunting yard.
 */
class ArriveAction : public Action { 
private:
	const Incoming* incoming;
public:
	ArriveAction() = delete;
	/** Construct an ArriveAction from the given parameters */
	ArriveAction(const ShuntingUnit* su, int duration, const Incoming* incoming) 
		: Action(su, incoming->IsInstanding() ? vector<const Track*>() : vector<const Track*>({incoming->GetParkingTrack()}), 
				{}, duration), incoming(incoming) {};
	/** Construct an ArriveAction from the given parameters */
	ArriveAction(const ShuntingUnit* su, const Incoming* incoming) : Action(su), incoming(incoming) {};
	/** Get the destination Track for this Incoming ShuntingUnit */
	inline const Track* GetDestinationTrack() const { return incoming->GetParkingTrack(); }
	/** Get the Incoming event for this ArriveAction */
	inline const Incoming* GetIncoming() const { return incoming; }
	inline const Arrive* CreateSimple() const { return new Arrive(incoming); }
	ACTION_OVERRIDE(ArriveAction)
};

/**
 * The ExitAction lets a scheduled ShuntingUnit depart from the shunting yard.
 */
class ExitAction : public Action {
private:
	const Outgoing* outgoing;
public:
	ExitAction() = delete;
	/**
	 * Construct an ExitAction from the given parameters
	 * 
	 * The ShuntingUnit attribute of the Outgoing event may refer to unspecified trains, that is trains of 
	 * some type, with no set id. Therefore the ShuntingUnit in the shunting yard need to 
	 * be specified seperately.
	 */
	ExitAction(const ShuntingUnit* su, int duration, const Outgoing* outgoing) 
		: Action(su, outgoing->IsInstanding() ? vector<const Track*>() : vector<const Track*>({ outgoing->GetParkingTrack() }), 
			{}, duration), outgoing(outgoing) {};
	/**
	 * Construct an ExitAction from the given parameters
	 *  
	 * The ShuntingUnit attribute of the Outgoing event may refer to unspecified trains, that is trains of 
	 * some type, with no set id. Therefore the ShuntingUnit in the shunting yard need to 
	 * be specified seperately.
	 */
	ExitAction(const ShuntingUnit* su, const Outgoing* outgoing) : Action(su), outgoing(outgoing) {};
	
	/** Get the destination Track for this Outgoing ShuntingUnit */
	inline const Track* GetDestinationTrack() const { return outgoing->GetParkingTrack(); }
	/** Get the Outgoing event for this ArriveAction */
	inline const Outgoing* GetOutgoing() const { return outgoing; }
	inline const Exit* CreateSimple() const { return new Exit(GetShuntingUnit(), outgoing); }
	ACTION_OVERRIDE(ExitAction)
};

/**
 * The BeginMoveAction changes the state of a ShuntingUnit to moving.
 * After this action the ShuntingUnit can be moved.
 */
class BeginMoveAction : public Action {
public:
	BeginMoveAction() = delete;
	/** Construct a BeginMoveAction from the given parameters */
	BeginMoveAction(const ShuntingUnit* su, int duration) : Action(su, {}, duration) {};
	inline const BeginMove* CreateSimple() const {return new BeginMove(GetShuntingUnit()); }
	ACTION_OVERRIDE(BeginMoveAction)
};

/**
 * The EndMoveAction changes the state of a ShuntingUnit to not moving.
 * After this action the ShuntingUnit can no longer be moved.
 */
class EndMoveAction : public Action {
public:
	EndMoveAction() = delete;
	/** Construct a EndMoveAction from the given parameters */
	EndMoveAction(const ShuntingUnit* su, int duration) : Action(su, {}, duration) {};
	inline const EndMove* CreateSimple() const {return new EndMove(GetShuntingUnit()); }
	ACTION_OVERRIDE(EndMoveAction)
};

/**
 * The MoveAction moves a ShuntingUnit from one Track to a neighboring Railroad Track.
 */
class MoveAction : public Action {
private:
	const vector<const Track*> tracks;
	bool stepMove;
public:
	MoveAction() = delete;
	/**
	 * Construct a MoveAction from the given parameters
	 * 
	 * The tracks are all the tracks that the ShuntingUnit passes in reaching its destination.
	 * The reserved tracks are all those tracks except the first track, the current track of the ShuntingUnit. 
	 */
	MoveAction(const ShuntingUnit* su, const vector<const Track*> &tracks, int duration, bool stepMove) : 
		Action(su, vector<const Track*>(tracks.begin()+1, tracks.end()), {}, duration), tracks(tracks), stepMove(stepMove) {};
	/** Get the destination Track of this MoveAction */
	inline const Track* GetDestinationTrack() const { return tracks.back(); }
	/** Get the previous Track of the moving ShuntingUnit when it has arrived on its destination Track */
	inline const Track* GetPreviousTrack() const { return tracks[tracks.size()-2]; }
	/** Get all the tracks that are used for this MoveAction */
	inline const vector<const Track*>& GetTracks() const { return tracks; }
	/** True if this MoveAction is a step-by-step move, otherwise false */
	inline bool IsStepMove() const { return stepMove; }
	inline const Move* CreateSimple() const {return new Move(GetShuntingUnit(), GetDestinationTrack()); }
	ACTION_OVERRIDE(MoveAction)
};

/**
 * The CombineAction combines two ShuntingUnits into one ShuntingUnit.
 */
class CombineAction : public Action {
private:
	const ShuntingUnit *rearSU, combinedSU;
	const string suString;
	bool inNeutral;
	int position;
public:
	CombineAction() = delete;
	/** 
	 * Generate a CombineAction from the given parameters
	 * 
	 * The inNeutral parameter describes whether the resulting ShuntingUnit will be in neutral direction. 
	 * The position parameter is the index of the resulting ShuntingUnit on the Track.
	 */
	CombineAction(const ShuntingUnit* frontSU, const ShuntingUnit* rearSU, const ShuntingUnit& combinedSU,
		const Track* track, int duration, bool inNeutral, int position) :
		Action(frontSU, {track}, {}, duration), rearSU(rearSU), combinedSU(combinedSU),
		inNeutral(inNeutral), position(position), suString(frontSU->GetTrainString() + " and " +rearSU->GetTrainString()) {};
	/** Get the front ShuntingUnit to be combined */
	inline const ShuntingUnit* GetFrontShuntingUnit() const { return GetShuntingUnit(); }
	/** Get the rear ShuntingUnit to be combined */
	inline const ShuntingUnit* GetRearShuntingUnit() const { return rearSU; }
	/** Get the combined ShuntingUnit */
	inline const ShuntingUnit* GetCombinedShuntingUnit() const { return &combinedSU; }
	inline const Combine* CreateSimple() const {return new Combine(GetShuntingUnit(), rearSU); }
	ACTION_OVERRIDE(CombineAction)
};

/**
 * The SplitAction splits a ShuntingUnit into two separate ShuntingUnits.
 */
class SplitAction : public Action {
private: 
	const ShuntingUnit suA, suB;
	const string suString;
public:
	SplitAction() = delete;
	/** Construct a SplitAction from the given parameters */
	SplitAction(const ShuntingUnit* su, const Track* track, int duration, const ShuntingUnit& suA, const ShuntingUnit& suB) : 
		Action(su, {track}, {}, duration), suA(suA), suB(suB), suString(su->toString()) {};
	/** Get the resulting ShuntingUnit that will apear on the A-side of the track. */
	inline const ShuntingUnit* GetASideShuntingUnit() const { return &suA; }
	/** Get the resulting ShuntingUnit that will apear on the B-side of the track. */
	inline const ShuntingUnit* GetBSideShuntingUnit() const { return &suB; }
	
	/**
	 * Get the split index of the split. 
	 * 
	 * A ShuntingUnit consisting of Trains A-B-C-D will split as follow with split index:
	 * 1. SU1: A, SU2: B-C-D
	 * 2. SU1: A-B, SU2: C-D
	 * 3. SU1: A-B-C, SU2: D
	 */
	inline const int GetSplitIndex() const { return GetASideShuntingUnit()->GetTrains().size(); }
	inline const Split* CreateSimple() const {return new Split(GetShuntingUnit(), GetSplitIndex()); }
	ACTION_OVERRIDE(SplitAction)
};

/**
 * The ServiceAction executes a task on the specified train.
 */
class ServiceAction : public Action {
private:
	const Train * train;
	const Facility* facility;
	const Task task;
public:
	ServiceAction() = delete;

	/**
	 *  Construct a ServiceAction for the given parameters
	 * 
	 *  Note that service tasks are executed on trains, not on shunting units. The reason is that trains are persistent and atomic. 
	 * Shunting units can change.
	 */
	ServiceAction(const ShuntingUnit* su, const Train* tu, const Task& ta, const Facility* fa, vector<const Employee*> employees) :
		Action(su, {}, employees, ta.duration), train(tu), task(ta), facility(fa) {}
	/** Get the Train that this service Task is operated on */
	inline const Train* GetTrain() const { return train; }
	/** Get the Facility at which the Task will be executed */
	inline const Facility* GetFacility() const { return facility; }
	/** Get the Task that will be executed */
	inline const Task* GetTask() const { return &task; }
	inline const Service* CreateSimple() const {return new Service(GetShuntingUnit(), *GetTask(), *GetTrain(), GetFacility()); }
	ACTION_OVERRIDE(ServiceAction)
};

/**
 * The SetbackAction changes the direction of the ShuntingUnit
 */
class SetbackAction : public Action {
public:
	SetbackAction() = delete;
	/** Construct a SetbackAction from the given parameters */
	SetbackAction(const ShuntingUnit* su, vector<const Employee*> drivers, int duration) : Action(su, {}, drivers, duration) {}
	/** Get the drivers for this action (not yet implemented) */
	const inline vector<const Employee*>& GetDrivers() const { return GetEmployees(); }
	inline const Setback* CreateSimple() const {return new Setback(GetShuntingUnit()); }
	ACTION_OVERRIDE(SetbackAction)
};

/**
 * The WaitAction instructs this ShuntingUnit to wait until the next Event.
 */
class WaitAction : public Action {
public:
	WaitAction() = delete;
	/** Construct a WaitAction with duration in seconds */
	WaitAction(const ShuntingUnit* su, int duration) : Action(su, {}, duration) {}
	inline const Wait* CreateSimple() const {return new Wait(GetShuntingUnit()); }
	ACTION_OVERRIDE(WaitAction)
};

///////////////////////////////
////// Action Generator ///////
///////////////////////////////

/**
 * An abstract base class for the ActionGenerators.
 * 
 * An ActionGenerator generates Actions based on a State.
 */
class ActionGenerator {
protected:
	const Location* location; /**< a reference to the Location object */
	/** Run an initial error check on the action and return the ShuntingUnit from the State */
	inline const ShuntingUnit* InitialCheck(const State* state, const SimpleAction& action) const {
		return InitialCheck(state, action.GetTrainIDs());
	}
	/** Run an initial error check on ShuntingUnit described the train IDs and return the ShuntingUnit from the State */
	const ShuntingUnit* InitialCheck(const State* state, const vector<int>& trainIDs) const;
public:
	ActionGenerator() = delete;
	ActionGenerator(const ActionGenerator& am) = delete;
	/** Construct this ActionGenerator based on the parameters defined in the json object */
	ActionGenerator(const json& params, const Location* location) : location(location) {}
	/** The default destructor */
	~ActionGenerator() = default;
	/** Generate actions given the State and store the result in the out list */
	virtual void Generate(const State* state, list<const Action*>& out) const = 0;
	/** Generate an Action object from a SimpleAction object given a State */
	virtual const Action* Generate(const State* state, const SimpleAction& action) const = 0;
};

/**
 * An ActionManager contains all the ActionGenerators and ActionValidators
 * 
 * The ActionManager is used to generate actions and to use the configured business rules to filter valid actions.
 */
class ActionManager {
private:
	unordered_map<string,const ActionGenerator*> generatorMap;
	vector<const ActionGenerator*> generators;
	vector<const BusinessRule*> validators;
	const Config* config;
	const Location* location;
	void AddGenerators();
	void AddValidators();
	void AddGenerator(const string& name, const ActionGenerator* generator);
public:
	ActionManager() = delete;
	ActionManager(const ActionManager& am) = delete;
	/** Construct the ActionManager from the given Config object */
	ActionManager(const Config* config, const Location* location) : config(config), location(location) {
		AddGenerators();
		AddValidators();
	}
	/** Destruct this ActionManager by destructing all its ActionGenerators */
	~ActionManager();
	/** Generate valid Action%s given the State and store the result in the out list */
	void Generate(const State* state, list<const Action*>& out) const;
	/** Get the ActionGenerator based on its name */
	inline const ActionGenerator* GetGenerator(const string& name) const { return generatorMap.at(name); }
	
	/** 
	 * Check the validity of an Action in a certain State 
	 * 
	 * @return a tuple (bool, string) describing
	 * 1. Whether the Action is valid
	 * 2. If not valid, why
	 */
	pair<bool, string> IsValid(const State* state, const Action* action) const;
	
};

#ifndef OVERRIDE_ACTIONGENERATOR
/** MACRO for defining some of ActionGenerator's subclasses to prevent duplicate code */
#define OVERRIDE_ACTIONGENERATOR(name) \
	name() = delete; \
	name(const name& n) = delete; \
	/** Generate actions given the State and store the result in the out list */ \
	void Generate(const State* state, list<const Action*>& out) const override; \
	/** Generate an Action object from a SimpleAction object given a State */ \
	const Action* Generate(const State* state, const SimpleAction& action) const override;
#endif

#ifndef DEFINE_ACTIONGENERATOR
/** MACRO for defining ActionGenerator's subclasses to prevent duplicate code */
#define DEFINE_ACTIONGENERATOR(name) \
class name : public ActionGenerator { \
public: \
	/** Construct this name based on the parameters defined in the json object */ \
	name(const json& params, const Location* location) : ActionGenerator(params, location) {}; \
	OVERRIDE_ACTIONGENERATOR(name) \
};
#endif

/** The ArriveActionGenerator generates ArriveAction%s */
DEFINE_ACTIONGENERATOR(ArriveActionGenerator)
/** The ExitActionGenerator generates ExitAction%s */
DEFINE_ACTIONGENERATOR(ExitActionGenerator)
/** The MoveHelperGenerator generates BeginMoveAction%s, EndMoveAction%s and single step MoveAction%s */
DEFINE_ACTIONGENERATOR(MoveHelperGenerator)
/** The WaitActionGenerator generates WaitAction%s */
DEFINE_ACTIONGENERATOR(WaitActionGenerator)
/** The ServiceActionGenerator generates ServiceAction%s */
DEFINE_ACTIONGENERATOR(ServiceActionGenerator)
/** The SplitActionGenerator generates SplitAction%s */
DEFINE_ACTIONGENERATOR(SplitActionGenerator)
/** The CombineActionGenerator generates CombineAction%s */
DEFINE_ACTIONGENERATOR(CombineActionGenerator)

/** The MoveActionGenerator generates MoveAction%s */
class MoveActionGenerator : public ActionGenerator {
private:
	int noRoutingDuration, constantTime;
	bool defaultTime, normTime, walkTime;
	void GenerateMovesFrom(const ShuntingUnit* su, const vector<const Track*> &tracks,
			const Track* previous, int duration, list<const Action*> &out) const;
public:
	/** Construct this MoveActionGenerator based on the parameters defined in the json object */ \
	MoveActionGenerator(const json& params, const Location* location);
	/** Generate a Path for the given Move action */
	const Path& GeneratePath(const State* state, const Move& action) const;
	OVERRIDE_ACTIONGENERATOR(MoveActionGenerator)
};

/** The SetbackActionGenerator generates SetbackAction%s */
class SetbackActionGenerator : public ActionGenerator {
private:
	bool defaultTime, normTime, walkTime;
	int constantTime;
	int GetDuration(const State* state, const ShuntingUnit* su, int numDrivers) const;
public:
	/** Construct this SetbackActionGenerator based on the parameters defined in the json object */ \
	SetbackActionGenerator(const json& params, const Location* location);
	OVERRIDE_ACTIONGENERATOR(SetbackActionGenerator)
};

//!\cond NO_DOC
STREAM_OPERATOR(Action);
STREAM_OPERATOR(SimpleAction);
//!\endcond 
#endif