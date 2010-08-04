
#ifndef EVENTDISPATCHER__H
#define EVENTDISPATCHER__H

#include <list>
using namespace std;

enum EventType
{
	IMPORTANT_EVENT,
	LESS_IMPORTANT_EVENT
};

class Event
{
public:
	EventType type;
};

class EventObserver
{
public:
	virtual void notify(const Event& event, int timeOutInSeconds)=0;
	virtual void notifyRegistration(EventObserver* newObserver)=0;
};

class EventDispatcher
{
	list<pair<EventType, EventObserver*> > observerList_;
public:
	EventDispatcher();

	void registerObserver(EventType type, EventObserver* observer);
	void dispatchEvent(const Event& event, int timeoutSeconds);
};

#endif
