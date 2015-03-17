#include "Platform.h"
#include <stdlib.h>
#include "CppUTest/TestHarness.h"
#undef malloc
#undef free
#undef calloc
#undef realloc

#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include "CppUTest/PlatformSpecificFunctions.h"

#include <windows.h>
#include <mmsystem.h>


#include <setjmp.h>

#ifdef STDC_WANT_SECURE_LIB
    #define FOPEN(fp, filename, flag) fopen_s((fp), (filename), (flag))
    #define _VSNPRINTF(str, size, trunc, format, args) _vsnprintf_s((str), (size), (trunc), (format), (args))
    #define LOCALTIME(_tm, timer) localtime_s((_tm), (timer))
#else
    #define FOPEN(fp, filename, flag) *(fp) = fopen((filename), (flag))
    #define _VSNPRINTF(str, size, trunc, format, args) _vsnprintf((str), (size), (format), (args))
    #define LOCALTIME(_tm, timer) memcpy(_tm, localtime(timer), sizeof(tm));
#endif

static jmp_buf test_exit_jmp_buf[10];
static int jmp_buf_index = 0;

int PlatformSpecificSetJmp(void (*function) (void* data), void* data)
{
    if (0 == setjmp(test_exit_jmp_buf[jmp_buf_index])) {
        jmp_buf_index++;
        function(data);
        jmp_buf_index--;
        return 1;
    }
    return 0;
}

void PlatformSpecificLongJmp()
{
    jmp_buf_index--;
    longjmp(test_exit_jmp_buf[jmp_buf_index], 1);
}

void PlatformSpecificRestoreJumpBuffer()
{
    jmp_buf_index--;
}

static void VisualCppPlatformSpecificRunTestInASeperateProcess(UtestShell* shell, TestPlugin* plugin, TestResult* result)
{
   result->addFailure(TestFailure(shell, "-p doesn't work on Visual C++ as it is lacking fork.\b"));
}

void (*PlatformSpecificRunTestInASeperateProcess)(UtestShell* shell, TestPlugin* plugin, TestResult* result) =
        VisualCppPlatformSpecificRunTestInASeperateProcess;

TestOutput::WorkingEnvironment PlatformSpecificGetWorkingEnvironment()
{
    return TestOutput::vistualStudio;
}

///////////// Time in millis

static long TimeInMillisImplementation()
{
    return timeGetTime();
}

long (*GetPlatformSpecificTimeInMillis)() = TimeInMillisImplementation;

///////////// Time in String

static const char* TimeStringImplementation()
{
    time_t the_time = time(NULL);
    struct tm the_local_time;
    static char dateTime[80];
    LOCALTIME(&the_local_time, &the_time);
    strftime(dateTime, 80, "%Y-%m-%dT%H:%M:%S", &the_local_time);
    return dateTime;
}

const char* (*GetPlatformSpecificTimeString)() = TimeStringImplementation;

////// taken from gcc

int PlatformSpecificVSNprintf(char *str, size_t size, const char* format, va_list args)
{
    char* buf = 0;
    size_t sizeGuess = size;

    int result = _VSNPRINTF( str, size, _TRUNCATE, format, args);
    str[size-1] = 0;
    while (result == -1)
    {
        if (buf != 0)
            free(buf);
        sizeGuess += 10;
        buf = (char*)malloc(sizeGuess);
        result = _VSNPRINTF( buf, sizeGuess, _TRUNCATE, format, args);
    }

    if (buf != 0)
        free(buf);
    return result;

}

PlatformSpecificFile PlatformSpecificFOpen(const char* filename, const char* flag)
{
   FILE* file;
   FOPEN(&file, filename, flag);
   return file;
}

void PlatformSpecificFPuts(const char* str, PlatformSpecificFile file)
{
   fputs(str, (FILE*)file);
}

void PlatformSpecificFClose(PlatformSpecificFile file)
{
   fclose((FILE*)file);
}

void PlatformSpecificFlush()
{
  fflush(stdout);
}

int PlatformSpecificPutchar(int c)
{
  return putchar(c);
}

void* PlatformSpecificMalloc(size_t size)
{
   return malloc(size);
}

void* PlatformSpecificRealloc (void* memory, size_t size)
{
   return realloc(memory, size);
}

void PlatformSpecificFree(void* memory)
{
   free(memory);
}

void* PlatformSpecificMemCpy(void* s1, const void* s2, size_t size)
{
   return memcpy(s1, s2, size);
}

void* PlatformSpecificMemset(void* mem, int c, size_t size)
{
    return memset(mem, c, size);
}

double PlatformSpecificFabs(double d)
{
   return fabs(d);
}

extern "C" int (*PlatformSpecificIsNan)(double) = _isnan;

int PlatformSpecificVSNprintf(char *str, unsigned int size, const char* format, void* args)
{
   return _VSNPRINTF( str, size, _TRUNCATE, format, (va_list) args);
}

static PlatformSpecificMutex Win32MutexCreate(void)
{
	CRITICAL_SECTION *critical_section = new CRITICAL_SECTION;
	InitializeCriticalSection(critical_section);
	return (PlatformSpecificMutex)critical_section;
}

static void Win32MutexLock(PlatformSpecificMutex mutex)
{
	EnterCriticalSection((CRITICAL_SECTION*)mutex);
}

static void Win32MutexUnlock(PlatformSpecificMutex mutex)
{
	LeaveCriticalSection((CRITICAL_SECTION*)mutex);
}

static void Win32MutexDestroy(PlatformSpecificMutex mutex)
{
	CRITICAL_SECTION *critical_section = (CRITICAL_SECTION*)mutex;
	DeleteCriticalSection(critical_section);
	delete critical_section;
}

PlatformSpecificMutex (*PlatformSpecificMutexCreate)(void) = Win32MutexCreate;
void (*PlatformSpecificMutexLock)(PlatformSpecificMutex) = Win32MutexLock;
void (*PlatformSpecificMutexUnlock)(PlatformSpecificMutex) = Win32MutexUnlock;
void (*PlatformSpecificMutexDestroy)(PlatformSpecificMutex) = Win32MutexDestroy;

void PlatformSpecificInstallCrashTrap(void (*crash_handler)(void *context, const char *crash_message), void *callback_context)
{
    (void)crash_handler;
    (void)callback_context;
}

void PlatformSpecificRemoveCrashTrap(void)
{
}
