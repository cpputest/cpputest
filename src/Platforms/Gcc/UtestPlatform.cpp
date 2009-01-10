
#include "CppUTest/TestHarness.h"
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdarg.h>


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


///////////// Run one test with exit on first error, using setjmp/longjmp

void FakePlatformSpecificExitCurrentTest()
{
}

void (*PlatformSpecificExitCurrentTest)() = PlatformSpecificExitCurrentTestImpl;

int PlatformSpecificSprintf(char *str, unsigned int size, const char *format, ...)
{
   va_list args;
   va_start(args, format);
   size_t count = vsnprintf( str, size, format, args);
   va_end(args);
   if (size < count)
       return -1;
   else
       return count;
}

int PlatformSpecificVSNprintf(char *str, unsigned int size, const char* format, void* args)
{
   size_t count = vsnprintf( str, size, format, (va_list) args);
   if (size < count)
       return -1;
   else
       return count;
}

