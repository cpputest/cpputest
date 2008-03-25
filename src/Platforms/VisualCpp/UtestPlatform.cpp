
#include "CppUTest/TestHarness.h"
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>

void Utest::executePlatformSpecificTestBody()
{
	testBody();
}

///////////// Time in millis

static long TimeInMillisImplementation()
{
    return timeGetTime()/1000;
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
	return "Windows time needs work";
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

