
#include "CppUTest/TestHarness.h"
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <stdarg.h>

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

#if 0

void TestRegistry::platformSpecificRunOneTest(Utest* test, TestResult& result)
{
    try {
        runOneTest(test, result) ;
    }
    catch (int) {
        //exiting test early
    }

}

void Utest::executePlatformSpecificTestBody()
{
	testBody();
}

void PlatformSpecificExitCurrentTestImpl()
{
    throw(1);
}

#endif

int PlatformSpecificSprintf(char *str, unsigned int size, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    memset(str, 0, size);
    return _vsnprintf( str, size-1, format, args);
}

int PlatformSpecificVSNprintf(char *str, unsigned int size, const char* format, void* args)
{
   return _vsnprintf( str, size, format, (va_list) args);
}


//platform specific test running stuff
#if 0
#include <setjmp.h>

static jmp_buf test_exit_jmp_buf[10];
static int jmp_buf_index = 0;

bool Utest::executePlatformSpecificSetup()
{
   if (0 == setjmp(test_exit_jmp_buf[jmp_buf_index])) {
      jmp_buf_index++;
      setup();
      jmp_buf_index--;
      return true;
   }
   return false;
}

void Utest::executePlatformSpecificTestBody()
{
   if (0 == setjmp(test_exit_jmp_buf[jmp_buf_index])) {
      jmp_buf_index++;
      testBody();
      jmp_buf_index--;
   }
}

void Utest::executePlatformSpecificTeardown()
{
   if (0 == setjmp(test_exit_jmp_buf[jmp_buf_index])) {
      jmp_buf_index++;
      teardown();
      jmp_buf_index--;
   }
}

void TestRegistry::platformSpecificRunOneTest(Utest* test, TestResult& result)
{
    if (0 == setjmp(test_exit_jmp_buf[jmp_buf_index])) {
       jmp_buf_index++;
       runOneTest(test, result);
       jmp_buf_index--;
    }
}


void PlatformSpecificExitCurrentTestImpl()
{
   jmp_buf_index--;
   longjmp(test_exit_jmp_buf[jmp_buf_index], 1);
}

static jmp_buf test_exit_jmp_buf[10];
static int jmp_buf_index = 0;
#endif

bool Utest::executePlatformSpecificSetup()
{
	try {
      setup();
	}
	catch (int) {
		return false;
	}
    return true;
}

void Utest::executePlatformSpecificTestBody()
{
	try {
      testBody();
	}
	catch (int) {
	}

}

void Utest::executePlatformSpecificTeardown()
{
	try {
      teardown();
	}
	catch (int) {
	}

}

void TestRegistry::platformSpecificRunOneTest(Utest* test, TestResult& result)
{
	try {
       runOneTest(test, result);
	}
	catch (int) {
	}

}


void PlatformSpecificExitCurrentTestImpl()
{
	throw(1);
}


void (*PlatformSpecificExitCurrentTest)() = PlatformSpecificExitCurrentTestImpl;

void FakePlatformSpecificExitCurrentTest()
{
}
