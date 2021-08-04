/** \file Event.h
 * Describes the Event class
 */
#pragma once
#ifndef EVENT_H
#define EVENT_H
#include "Action.h"

/** The EventType of an Event */
enum class EventType {
	ActionFinish, 		/**< When an Action is finished */
	IncomingTrain,		/**< When an Incoming train arrives */
	OutgoingTrain,		/**< When an Outgoing train should depart */
	DisturbanceBegin,	/**< When a disturbance begins (not yet implemented) */
	DisturbanceEnd,		/**< When a disturbance ends (not yet implemented) */
	Trigger,			/**< (not yet implemented) */
	MoveUpdate			/**< (not yet implemented) */
};

/**
 * A triggered Event
 */
class Event
{
private:
	int time;
	const Action* action;
	const EventType type;
public:
	/** Construct an Event with type ActionFinish from the given Action */
	Event(int time, const Action* action);
	/** Construct an Event with type IncomingTrain from the given Incoming object */
	Event(const Incoming* in);
	/** Construct an Event with type OutgoingTrain from the given Outgoing object */
	Event(const Outgoing* out);
	/** Copy the Event */
	Event(const Event &e);
	/** Destroy the Event */
	~Event();
	/** Get the time of this Event */
	inline int GetTime() const { return time; }
	/** Get the Action that belongs to this Event */
	inline const Action* GetAction() const { return action; }
	/** Get the EventType of this Event */
	inline const EventType GetType() const { return type; }
	/** Get the < comparator for this Event, to order the events based on time */
	inline friend bool operator< (const Event& lhs, const Event& rhs) { return lhs.time < rhs.time; }
	/** Get the > comparator for this Event, to order the events based on time */
	inline friend bool operator> (const Event& lhs, const Event& rhs) { return lhs.time > rhs.time; }
	/** Get a string represenation of the Event */
	inline const string toString() const {
		switch (type) {
		case EventType::ActionFinish: return "Finish action " + (action == nullptr ? "X" : action->toString());
		case EventType::IncomingTrain: return "Incoming train";
		case EventType::OutgoingTrain: return "Outgoing train";
		default: return "Event";
		};
	}
};

//!\cond NO_DOC
struct EventCompare
{
    bool operator()(const Event* left, const Event* right) { return *left > *right; }
};

STREAM_OPERATOR(Event);
//!\endcond

/**
 * A priority queue of all the Event%s based on the time of the Event 
 */
class EventQueue : public priority_queue < const Event*, vector<const Event*>, EventCompare> {
public:
/** Destroy the EventQueue */
~EventQueue() {
    while(size() > 0) {
        delete top();
        pop();
    }
}
};

#endif