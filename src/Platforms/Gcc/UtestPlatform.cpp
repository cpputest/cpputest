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
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ``AS IS'' AND ANY
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

#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>
#ifndef __MINGW32__
#include <sys/wait.h>
#endif
#include <pthread.h>

#include "CppUTest/PlatformSpecificFunctions.h"

static jmp_buf test_exit_jmp_buf[10];
static int jmp_buf_index = 0;

#ifdef __MINGW32__

static void GccNoPThreadPlatformSpecificRunTestInASeperateProcess(UtestShell* shell, TestPlugin* plugin, TestResult* result)
{
    printf("-p doesn't work on MinGW as it is lacking fork. Running inside the process\b");
    shell->runOneTestInCurrentProcess(plugin, *result);
}

void (*PlatformSpecificRunTestInASeperateProcess)(UtestShell* shell, TestPlugin* plugin, TestResult* result) =
        GccNoPThreadPlatformSpecificRunTestInASeperateProcess;

#else

static void GccCygwinPlatformSpecificRunTestInASeperateProcess(UtestShell* shell, TestPlugin* plugin, TestResult* result)
{
    pid_t cpid, w;
    int status;

    cpid = fork();
    
    if (cpid == -1) {
        result->addFailure(TestFailure(shell, "Call to fork() failed"));
        return;
    }

    if (cpid == 0) {            /* Code executed by child */
        shell->runOneTestInCurrentProcess(plugin, *result);
        _exit(result->getFailureCount());
    } else {                    /* Code executed by parent */
        do {
            w = waitpid(cpid, &status, WUNTRACED | WCONTINUED);
            if (w == -1) {
                result->addFailure(TestFailure(shell, "Call to waitpid() failed"));
                return;
            }

            if (WIFEXITED(status) && WEXITSTATUS(status) > result->getFailureCount()) {
                result->addFailure(TestFailure(shell, "Failed in separate process"));
            } else if (WIFSIGNALED(status)) {
                SimpleString signal(StringFrom(WTERMSIG(status)));
                {
                    SimpleString message("Failed in separate process - killed by signal ");
                    message += signal;
                    result->addFailure(TestFailure(shell, message));
                }
            } else if (WIFSTOPPED(status)) {
                result->addFailure(TestFailure(shell, "Stopped in separate process"));
            } 
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

void (*PlatformSpecificRunTestInASeperateProcess)(UtestShell* shell, TestPlugin* plugin, TestResult* result) =
        GccCygwinPlatformSpecificRunTestInASeperateProcess;

#endif

TestOutput::WorkingEnvironment PlatformSpecificGetWorkingEnvironment()
{
    return TestOutput::eclipse;
}

extern "C" {

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

/*
 * MacOSX clang 3.0 doesn't seem to recognize longjmp and thus complains about __no_return_.
 * The later clang compilers complain when it isn't there. So only way is to check the clang compiler here :(
 */
#if !((__clang_major__ == 3) && (__clang_minor__ == 0))
__no_return__
#endif
void PlatformSpecificLongJmp()
{
    jmp_buf_index--;
    longjmp(test_exit_jmp_buf[jmp_buf_index], 1);
}

void PlatformSpecificRestoreJumpBuffer()
{
    jmp_buf_index--;
}

///////////// Time in millis

static long TimeInMillisImplementation()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return (tv.tv_sec * 1000) + (long)((double)tv.tv_usec * 0.001);
}

long (*GetPlatformSpecificTimeInMillis)() = TimeInMillisImplementation;

static const char* TimeStringImplementation()
{
    time_t tm = time(NULL);
    static char dateTime[80];
    struct tm *tmp = localtime(&tm);
    strftime(dateTime, 80, "%Y-%m-%dT%H:%M:%S", tmp);
    return dateTime;
}

const char* (*GetPlatformSpecificTimeString)() = TimeStringImplementation;

/* Wish we could add an attribute to the format for discovering mis-use... but the __attribute__(format) seems to not work on va_list */
#ifdef __clang__
#pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif

int PlatformSpecificVSNprintf(char *str, size_t size, const char* format, va_list args)
{
   return vsnprintf( str, size, format, args);
}

PlatformSpecificFile PlatformSpecificFOpen(const char* filename, const char* flag)
{
   return fopen(filename, flag);
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

static int IsNanImplementation(double d)
{
    return isnan((float)d);
}

int (*PlatformSpecificIsNan)(double) = IsNanImplementation;


static PlatformSpecificMutex PThreadMutexCreate(void)
{
    pthread_mutex_t *mutex = new pthread_mutex_t;

    pthread_mutex_init(mutex, NULL);

    return (PlatformSpecificMutex)mutex;
}

static void PThreadMutexLock(PlatformSpecificMutex mtx)
{
    pthread_mutex_lock((pthread_mutex_t *)mtx);
}

static void PThreadMutexUnlock(PlatformSpecificMutex mtx)
{
    pthread_mutex_unlock((pthread_mutex_t *)mtx);
}

static void PThreadMutexDestroy(PlatformSpecificMutex mtx)
{
    pthread_mutex_t *mutex = (pthread_mutex_t *)mtx;
    pthread_mutex_destroy(mutex);
    delete mutex;
}

PlatformSpecificMutex (*PlatformSpecificMutexCreate)(void) = PThreadMutexCreate;
void (*PlatformSpecificMutexLock)(PlatformSpecificMutex) = PThreadMutexLock;
void (*PlatformSpecificMutexUnlock)(PlatformSpecificMutex) = PThreadMutexUnlock;
void (*PlatformSpecificMutexDestroy)(PlatformSpecificMutex) = PThreadMutexDestroy;

}
