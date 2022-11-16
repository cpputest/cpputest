/*
 * Copyright (c) 2007, Michael Feathers, James Grenning and Bas Vodde
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include "CppUTest/TestHarness.h"
#undef malloc
#undef free
#undef calloc
#undef realloc
#undef strdup
#undef strndup

#ifdef CPPUTEST_HAVE_GETTIMEOFDAY
#include <sys/time.h>
#endif
#if defined(CPPUTEST_HAVE_FORK) && defined(CPPUTEST_HAVE_WAITPID)
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#endif

#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <signal.h>

#ifdef CPPUTEST_HAVE_PTHREAD_MUTEX_LOCK
#include <pthread.h>
#endif

#include "CppUTest/PlatformSpecificFunctions.h"

static jmp_buf test_exit_jmp_buf[10];
static int jmp_buf_index = 0;

// There is a possibility that a compiler provides fork but not waitpid.
#if !defined(CPPUTEST_HAVE_FORK) || !defined(CPPUTEST_HAVE_WAITPID)

static void GccPlatformSpecificRunTestInASeperateProcess(UtestShell* shell, TestPlugin*, TestResult* result)
{
    result->addFailure(TestFailure(shell, "-p doesn't work on this platform, as it is lacking fork.\b"));
}

static int PlatformSpecificForkImplementation(void)
{
    return 0;
}

static int PlatformSpecificWaitPidImplementation(int, int*, int)
{
    return 0;
}

#else

static void SetTestFailureByStatusCode(UtestShell* shell, TestResult* result, int status)
{
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
        result->addFailure(TestFailure(shell, "Failed in separate process"));
    } else if (WIFSIGNALED(status)) {
        SimpleString message("Failed in separate process - killed by signal ");
        message += StringFrom(WTERMSIG(status));
        result->addFailure(TestFailure(shell, message));
    } else if (WIFSTOPPED(status)) {
        result->addFailure(TestFailure(shell, "Stopped in separate process - continuing"));
    }
}

static void GccPlatformSpecificRunTestInASeperateProcess(UtestShell* shell, TestPlugin* plugin, TestResult* result)
{
    const pid_t syscallError = -1;
    pid_t cpid;
    pid_t w;
    int status = 0;

    cpid = PlatformSpecificFork();

    if (cpid == syscallError) {
        result->addFailure(TestFailure(shell, "Call to fork() failed"));
        return;
    }

    if (cpid == 0) {            /* Code executed by child */
        const size_t initialFailureCount = result->getFailureCount(); // LCOV_EXCL_LINE
        shell->runOneTestInCurrentProcess(plugin, *result);        // LCOV_EXCL_LINE
        _exit(initialFailureCount < result->getFailureCount());    // LCOV_EXCL_LINE
    } else {                    /* Code executed by parent */
        size_t amountOfRetries = 0;
        do {
            w = PlatformSpecificWaitPid(cpid, &status, WUNTRACED);
            if (w == syscallError) {
                // OS X debugger causes EINTR
                if (EINTR == errno) {
                  if (amountOfRetries > 30) {
                    result->addFailure(TestFailure(shell, "Call to waitpid() failed with EINTR. Tried 30 times and giving up! Sometimes happens in debugger"));
                    return;
                  }
                  amountOfRetries++;
                }
                else {
                    result->addFailure(TestFailure(shell, "Call to waitpid() failed"));
                    return;
                }
            } else {
                SetTestFailureByStatusCode(shell, result, status);
                if (WIFSTOPPED(status)) kill(w, SIGCONT);
            }
        } while ((w == syscallError) || (!WIFEXITED(status) && !WIFSIGNALED(status)));
    }
}

static pid_t PlatformSpecificForkImplementation(void)
{
    return fork();
}

static pid_t PlatformSpecificWaitPidImplementation(int pid, int* status, int options)
{
    return waitpid(pid, status, options);
}

#endif

TestOutput::WorkingEnvironment PlatformSpecificGetWorkingEnvironment()
{
    return TestOutput::eclipse;
}

void (*PlatformSpecificRunTestInASeperateProcess)(UtestShell* shell, TestPlugin* plugin, TestResult* result) =
        GccPlatformSpecificRunTestInASeperateProcess;
int (*PlatformSpecificFork)(void) = PlatformSpecificForkImplementation;
int (*PlatformSpecificWaitPid)(int, int*, int) = PlatformSpecificWaitPidImplementation;

extern "C" {

static int PlatformSpecificSetJmpImplementation(void (*function) (void* data), void* data)
{
    if (0 == setjmp(test_exit_jmp_buf[jmp_buf_index])) {
        jmp_buf_index++;
        function(data);
        jmp_buf_index--;
        return 1;
    }
    return 0;
}

/*
 * MacOSX clang 3.0 doesn't seem to recognize longjmp and thus complains about _no_return_.
 * The later clang compilers complain when it isn't there. So only way is to check the clang compiler here :(
 */
#ifdef __clang__
 #if !((__clang_major__ == 3) && (__clang_minor__ == 0))
 _no_return_
 #endif
#endif
static void PlatformSpecificLongJmpImplementation()
{
    jmp_buf_index--;
    longjmp(test_exit_jmp_buf[jmp_buf_index], 1);
}

static void PlatformSpecificRestoreJumpBufferImplementation()
{
    jmp_buf_index--;
}

void (*PlatformSpecificLongJmp)() = PlatformSpecificLongJmpImplementation;
int (*PlatformSpecificSetJmp)(void (*)(void*), void*) = PlatformSpecificSetJmpImplementation;
void (*PlatformSpecificRestoreJumpBuffer)() = PlatformSpecificRestoreJumpBufferImplementation;

///////////// Time in millis

static long TimeInMillisImplementation()
{
#ifdef CPPUTEST_HAVE_GETTIMEOFDAY
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return (long)((tv.tv_sec * 1000) + (time_t)((double)tv.tv_usec * 0.001));
#else
    return 0;
#endif
}

static const char* TimeStringImplementation()
{
    time_t theTime = time(NULLPTR);
    static char dateTime[80];
#ifdef STDC_WANT_SECURE_LIB
    static struct tm lastlocaltime;
    localtime_s(&lastlocaltime, &theTime);
    struct tm *tmp = &lastlocaltime;
#else
    struct tm *tmp = localtime(&theTime);
#endif
    strftime(dateTime, 80, "%Y-%m-%dT%H:%M:%S", tmp);
    return dateTime;
}

long (*GetPlatformSpecificTimeInMillis)() = TimeInMillisImplementation;
const char* (*GetPlatformSpecificTimeString)() = TimeStringImplementation;

/* Wish we could add an attribute to the format for discovering mis-use... but the __attribute__(format) seems to not work on va_list */
#ifdef __clang__
#pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif

#ifdef __clang__
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
#endif
int (*PlatformSpecificVSNprintf)(char *str, size_t size, const char* format, va_list va_args_list) = vsnprintf;

static PlatformSpecificFile PlatformSpecificFOpenImplementation(const char* filename, const char* flag)
{
#ifdef STDC_WANT_SECURE_LIB
  FILE* file;
   fopen_s(&file, filename, flag);
   return file;
#else
   return fopen(filename, flag);
#endif
}

static void PlatformSpecificFPutsImplementation(const char* str, PlatformSpecificFile file)
{
   fputs(str, (FILE*)file);
}

static void PlatformSpecificFCloseImplementation(PlatformSpecificFile file)
{
   fclose((FILE*)file);
}

static void PlatformSpecificFlushImplementation()
{
  fflush(stdout);
}

PlatformSpecificFile PlatformSpecificStdOut = stdout;

PlatformSpecificFile (*PlatformSpecificFOpen)(const char*, const char*) = PlatformSpecificFOpenImplementation;
void (*PlatformSpecificFPuts)(const char*, PlatformSpecificFile) = PlatformSpecificFPutsImplementation;
void (*PlatformSpecificFClose)(PlatformSpecificFile) = PlatformSpecificFCloseImplementation;

void (*PlatformSpecificFlush)() = PlatformSpecificFlushImplementation;

void* (*PlatformSpecificMalloc)(size_t size) = malloc;
void* (*PlatformSpecificRealloc)(void*, size_t) = realloc;
void (*PlatformSpecificFree)(void* memory) = free;
void* (*PlatformSpecificMemCpy)(void*, const void*, size_t) = memcpy;
void* (*PlatformSpecificMemset)(void*, int, size_t) = memset;

/* GCC 4.9.x introduces -Wfloat-conversion, which causes a warning / error
 * in GCC's own (macro) implementation of isnan() and isinf().
 */
#if defined(__GNUC__) && (__GNUC__ >= 5 || (__GNUC__ == 4 && __GNUC_MINOR__ > 8))
#pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

static int IsNanImplementation(double d)
{
    return isnan(d);
}

static int IsInfImplementation(double d)
{
    return isinf(d);
}

double (*PlatformSpecificFabs)(double) = fabs;
void (*PlatformSpecificSrand)(unsigned int) = srand;
int (*PlatformSpecificRand)(void) = rand;
int (*PlatformSpecificIsNan)(double) = IsNanImplementation;
int (*PlatformSpecificIsInf)(double) = IsInfImplementation;
int (*PlatformSpecificAtExit)(void(*func)(void)) = atexit;  /// this was undefined before

static PlatformSpecificMutex PThreadMutexCreate(void)
{
#ifdef CPPUTEST_HAVE_PTHREAD_MUTEX_LOCK
    pthread_mutex_t *mutex = new pthread_mutex_t;

    pthread_mutex_init(mutex, NULLPTR);
    return (PlatformSpecificMutex)mutex;
#else
    return NULLPTR;
#endif

}

#ifdef CPPUTEST_HAVE_PTHREAD_MUTEX_LOCK
static void PThreadMutexLock(PlatformSpecificMutex mtx)
{
    pthread_mutex_lock((pthread_mutex_t *)mtx);
}
#else
static void PThreadMutexLock(PlatformSpecificMutex)
{
}
#endif

#ifdef CPPUTEST_HAVE_PTHREAD_MUTEX_LOCK
static void PThreadMutexUnlock(PlatformSpecificMutex mtx)
{
    pthread_mutex_unlock((pthread_mutex_t *)mtx);
}
#else
static void PThreadMutexUnlock(PlatformSpecificMutex)
{
}
#endif

#ifdef CPPUTEST_HAVE_PTHREAD_MUTEX_LOCK
static void PThreadMutexDestroy(PlatformSpecificMutex mtx)
{
    pthread_mutex_t *mutex = (pthread_mutex_t *)mtx;
    pthread_mutex_destroy(mutex);
    delete mutex;
}
#else
static void PThreadMutexDestroy(PlatformSpecificMutex)
{
}
#endif

PlatformSpecificMutex (*PlatformSpecificMutexCreate)(void) = PThreadMutexCreate;
void (*PlatformSpecificMutexLock)(PlatformSpecificMutex) = PThreadMutexLock;
void (*PlatformSpecificMutexUnlock)(PlatformSpecificMutex) = PThreadMutexUnlock;
void (*PlatformSpecificMutexDestroy)(PlatformSpecificMutex) = PThreadMutexDestroy;
void (*PlatformSpecificAbort)(void) = abort;

}
