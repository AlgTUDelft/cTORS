#include "Event.h"

Event::Event(int time, const Action* action, const EventType eventType) : 
	time(time), type(eventType) {
	if (action != nullptr)
		this->action = action->clone();
	else this->action = nullptr;
}

Event::Event(const Event &e) : time(e.time), type(e.type) {
	if (e.action != nullptr)
		action = e.action->clone();
	else action = nullptr;
}

Event::~Event()
{
	if(action != nullptr)
		delete action;
}
