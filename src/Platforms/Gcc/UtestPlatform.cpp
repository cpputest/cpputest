
#include "CppUTest/TestHarness.h"
#include <time.h>
#include <sys/time.h>

void Utest::executePlatformSpecificTestBody()
{
	testBody();
}

///////////// Time in millis

static long TimeInMillisImplementation()
{
	struct timeval tv;
	struct timezone tz;
	::gettimeofday(&tv, &tz);
	return (tv.tv_sec * 1000) + (long)(tv.tv_usec * 0.001);	
}

static long (*timeInMillisFp) () = TimeInMillisImplementation;

long GetPlatformSpecificTimeInMillis()
{
	return timeInMillisFp();
}

void SetPlatformSpecificTimeInMillisMethod(long (*platformSpecific) ())
{
	timeInMillisFp = (platformSpecific == 0) ? TimeInMillisImplementation : platformSpecific;
}

///////////// Time in String

static SimpleString TimeStringImplementation()
{
	time_t tm = time(NULL);
	return ctime(&tm);
}

static SimpleString (*timeStringFp) () = TimeStringImplementation;

SimpleString GetPlatformSpecificTimeString()
{
	return timeStringFp();
}

void SetPlatformSpecificTimeStringMethod(SimpleString (*platformMethod) ())
{
	timeStringFp = (platformMethod == 0) ? TimeStringImplementation : platformMethod;
}


///////////// Run one test with exit on first error
void TestRegistry::platformSpecificRunOneTest(Utest* test, TestResult& result)
{
    try {
        runOneTest(test, result) ;
    } 
    catch (int) {
        //exiting test early
    }
     
}

void PlatformSpecificExitCurrentTestImpl() 
{
    throw(1);
}

void FakePlatformSpecificExitCurrentTest() 
{
}

void (*PlatformSpecificExitCurrentTest)() = PlatformSpecificExitCurrentTestImpl;


