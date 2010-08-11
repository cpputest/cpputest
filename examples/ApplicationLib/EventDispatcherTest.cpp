
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "EventDispatcher.h"

class ObserverMock : public EventObserver
{
public:
	virtual void notify(const Event& event, int timeOutInSeconds)
	{
		mock().actualCall("notify").withParameterOfType("Event", "event", (void*) &event).withParameter("timeOutInSeconds", timeOutInSeconds);
	}
	virtual void notifyRegistration(EventObserver* newObserver)
	{
		mock().actualCall("notifyRegistration").withParameter("newObserver", newObserver);
	}
};

class EventComparator : public MockNamedValueComparator
{
public:
	virtual bool isEqual(void* object1, void* object2)
	{
		return ((Event*)object1)->type == ((Event*)object2)->type;
	}
	virtual SimpleString valueToString(void* object)
	{
		return StringFrom(((Event*)object)->type);
	}
};


TEST_GROUP(EventDispatcher)
{
	Event event;
	EventDispatcher* dispatcher;
	ObserverMock observer;
	EventComparator eventComparator;

	void setup()
	{
		dispatcher = new EventDispatcher;
		mock().installComparator("Event", eventComparator);
	}
	void teardown()
	{
		delete dispatcher;
		mock().removeAllComparators();
	}
};


TEST(EventDispatcher, EventWithoutRegistrationsResultsIntoNoCalls)
{
	dispatcher->dispatchEvent(event, 10);
	mock().checkExpectations();
}

TEST(EventDispatcher, EventWithRegistrationForEventResultsIntoCallback)
{
	mock().expectOneCall("notify").withParameterOfType("Event", "event", &event).withParameter("timeOutInSeconds", 10);
	event.type = IMPORTANT_EVENT;

	dispatcher->registerObserver(IMPORTANT_EVENT, &observer);
	dispatcher->dispatchEvent(event, 10);
	mock().checkExpectations();
}

TEST(EventDispatcher, DifferentEventWithRegistrationDoesNotResultIntoCallback)
{
	event.type = LESS_IMPORTANT_EVENT;
	dispatcher->registerObserver(IMPORTANT_EVENT, &observer);
	dispatcher->dispatchEvent(event, 10);
	mock().checkExpectations();
}

TEST(EventDispatcher, RegisterTwoObserversResultIntoTwoCallsAndARegistrationNotification)
{
	mock().expectOneCall("notify").withParameterOfType("Event", "event", &event).withParameter("timeOutInSeconds", 10);
	mock().expectOneCall("notify").withParameterOfType("Event", "event", &event).withParameter("timeOutInSeconds", 10);
	mock().expectOneCall("notifyRegistration").withParameter("newObserver", &observer);

	event.type = IMPORTANT_EVENT;
	dispatcher->registerObserver(IMPORTANT_EVENT, &observer);
	dispatcher->registerObserver(IMPORTANT_EVENT, &observer);
	dispatcher->dispatchEvent(event, 10);
	mock().checkExpectations();
}
