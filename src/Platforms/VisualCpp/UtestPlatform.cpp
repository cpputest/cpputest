#include <stdlib.h>
#include "CppUTest/TestHarness.h"
#undef malloc
#undef free
#undef calloc
#undef realloc
#undef strdup
#undef strndup

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

static int VisualCppSetJmp(void (*function) (void* data), void* data)
{
    if (0 == setjmp(test_exit_jmp_buf[jmp_buf_index])) {
        jmp_buf_index++;
        function(data);
        jmp_buf_index--;
        return 1;
    }
    return 0;
}

static void VisualCppLongJmp()
{
    jmp_buf_index--;
    longjmp(test_exit_jmp_buf[jmp_buf_index], 1);
}

static void VisualCppRestoreJumpBuffer()
{
    jmp_buf_index--;
}

int (*PlatformSpecificSetJmp)(void (*function) (void*), void* data) = VisualCppSetJmp;
void (*PlatformSpecificLongJmp)(void) = VisualCppLongJmp;
void (*PlatformSpecificRestoreJumpBuffer)(void) = VisualCppRestoreJumpBuffer;

static void VisualCppRunTestInASeperateProcess(UtestShell* shell, TestPlugin* plugin, TestResult* result)
{
    result->addFailure(TestFailure(shell, "-p doesn't work on this platform, as it is lacking fork.\b"));
}

void (*PlatformSpecificRunTestInASeperateProcess)(UtestShell* shell, TestPlugin* plugin, TestResult* result) =
        VisualCppRunTestInASeperateProcess;

TestOutput::WorkingEnvironment PlatformSpecificGetWorkingEnvironment()
{
    return TestOutput::visualStudio;
}

///////////// Time in millis

static long VisualCppTimeInMillis()
{
	static LARGE_INTEGER s_frequency;
	static const BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	if (s_use_qpc)
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (long)((now.QuadPart * 1000) / s_frequency.QuadPart);
	} 
	else 
	{
	#ifdef TIMERR_NOERROR
		return (long)timeGetTime();
	#else
		#if !defined(_WIN32_WINNT) || !defined(_WIN32_WINNT_VISTA) || (_WIN32_WINNT < _WIN32_WINNT_VISTA)
			return (long)GetTickCount();
		#else
			return (long)GetTickCount64();
		#endif
	#endif
	}
}

long (*GetPlatformSpecificTimeInMillis)() = VisualCppTimeInMillis;

///////////// Time in String

static const char* VisualCppTimeString()
{
    time_t the_time = time(NULL);
    struct tm the_local_time;
    static char dateTime[80];
    LOCALTIME(&the_local_time, &the_time);
    strftime(dateTime, 80, "%Y-%m-%dT%H:%M:%S", &the_local_time);
    return dateTime;
}

const char* (*GetPlatformSpecificTimeString)() = VisualCppTimeString;

////// taken from gcc

static int VisualCppVSNprintf(char *str, size_t size, const char* format, va_list args)
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

int (*PlatformSpecificVSNprintf)(char *str, size_t size, const char* format, va_list va_args_list) = VisualCppVSNprintf;

static PlatformSpecificFile VisualCppFOpen(const char* filename, const char* flag)
{
    FILE* file;
    FOPEN(&file, filename, flag);
    return file;
}

static void VisualCppFPuts(const char* str, PlatformSpecificFile file)
{
    fputs(str, (FILE*)file);
}

static void VisualCppFClose(PlatformSpecificFile file)
{
    fclose((FILE*)file);
}

PlatformSpecificFile (*PlatformSpecificFOpen)(const char* filename, const char* flag) = VisualCppFOpen;
void (*PlatformSpecificFPuts)(const char* str, PlatformSpecificFile file) = VisualCppFPuts;
void (*PlatformSpecificFClose)(PlatformSpecificFile file) = VisualCppFClose;

static void VisualCppFlush()
{
    fflush(stdout);
}

int (*PlatformSpecificPutchar)(int c) = putchar;
void (*PlatformSpecificFlush)(void) = VisualCppFlush;

static void* VisualCppMalloc(size_t size)
{
    return malloc(size);
}

static void* VisualCppReAlloc(void* memory, size_t size)
{
    return realloc(memory, size);
}

static void VisualCppFree(void* memory)
{
    free(memory);
}

void (*PlatformSpecificSrand)(unsigned int) = srand;
int (*PlatformSpecificRand)(void) = rand;
void* (*PlatformSpecificMalloc)(size_t size) = VisualCppMalloc;
void* (*PlatformSpecificRealloc)(void* memory, size_t size) = VisualCppReAlloc;
void (*PlatformSpecificFree)(void* memory) = VisualCppFree;
void* (*PlatformSpecificMemCpy)(void* s1, const void* s2, size_t size) = memcpy;
void* (*PlatformSpecificMemset)(void* mem, int c, size_t size) = memset;

static int IsInfImplementation(double d)
{
    return !_finite(d);
}

double (*PlatformSpecificFabs)(double d) = fabs;
extern "C" int (*PlatformSpecificIsNan)(double) = _isnan;
extern "C" int (*PlatformSpecificIsInf)(double) = IsInfImplementation;
int (*PlatformSpecificAtExit)(void(*func)(void)) = atexit;

static PlatformSpecificMutex VisualCppMutexCreate(void)
{
	CRITICAL_SECTION *critical_section = new CRITICAL_SECTION;
	InitializeCriticalSection(critical_section);
	return (PlatformSpecificMutex)critical_section;
}

static void VisualCppMutexLock(PlatformSpecificMutex mutex)
{
	EnterCriticalSection((CRITICAL_SECTION*)mutex);
}

static void VisualCppMutexUnlock(PlatformSpecificMutex mutex)
{
	LeaveCriticalSection((CRITICAL_SECTION*)mutex);
}

static void VisualCppMutexDestroy(PlatformSpecificMutex mutex)
{
	CRITICAL_SECTION *critical_section = (CRITICAL_SECTION*)mutex;
	DeleteCriticalSection(critical_section);
	delete critical_section;
}

PlatformSpecificMutex (*PlatformSpecificMutexCreate)(void) = VisualCppMutexCreate;
void (*PlatformSpecificMutexLock)(PlatformSpecificMutex) = VisualCppMutexLock;
void (*PlatformSpecificMutexUnlock)(PlatformSpecificMutex) = VisualCppMutexUnlock;
void (*PlatformSpecificMutexDestroy)(PlatformSpecificMutex) = VisualCppMutexDestroy;
