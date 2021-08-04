#include "Event.h"

Event::Event(int time, const Action* action) : 
	time(time), type(EventType::ActionFinish) {
	if (action != nullptr)
		this->action = action->Clone();
	else this->action = nullptr;
}

Event::Event(const Incoming* in) :
	time(in->GetTime()), action(nullptr), type(EventType::IncomingTrain) {}

Event::Event(const Outgoing* out) :
	time(out->GetTime()), action(nullptr), type(EventType::OutgoingTrain) {}

Event::Event(const Event &e) : time(e.time), type(e.type) {
	if (e.action != nullptr)
		action = e.action->Clone();
	else action = nullptr;
}

Event::~Event()
{
	if(action != nullptr)
		delete action;
}
