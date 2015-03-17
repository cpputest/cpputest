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

#include <cstdlib>
#include "CppUTest/TestHarness.h"
#undef malloc
#undef free
#undef calloc
#undef realloc

#define  far  // eliminate "meaningless type qualifier" warning
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "CppUTest/PlatformSpecificFunctions.h"

static jmp_buf test_exit_jmp_buf[10];
static int jmp_buf_index = 0;

/* The ARMCC compiler will compile this function with C++ linkage, unless
 * we specifically tell it to use C linkage again, in the function definiton.
 */
extern "C" int PlatformSpecificSetJmp(void (*function) (void* data), void* data)
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

void PlatformSpecificRunTestInASeperateProcess(UtestShell* shell, TestPlugin* plugin, TestResult* result)
{
    printf("-p isn' implemented for armcc. Running inside the process\b");
    shell->runOneTest(plugin, *result);
}

TestOutput::WorkingEnvironment PlatformSpecificGetWorkingEnvironment()
{
    return TestOutput::eclipse;
}

///////////// Time in millis
/*
*  In Keil MDK-ARM, clock() default implementation used semihosting.
*  Resolutions is user adjustable (1 ms for now)
*/
static long TimeInMillisImplementation()
{
    clock_t t = clock();
    return t;
}

extern "C" long (*GetPlatformSpecificTimeInMillis)() = TimeInMillisImplementation;

static const char* TimeStringImplementation()
{
    time_t tm = time(NULL);
    return ctime(&tm);
}

extern "C" const char* (*GetPlatformSpecificTimeString)() = TimeStringImplementation;

int PlatformSpecificAtoI(const char* str)
{
    return atoi(str);
}

/* The ARMCC compiler will compile this function with C++ linkage, unless
 * we specifically tell it to use C linkage again, in the function definiton.
 */
// extern "C" int PlatformSpecificVSNprintf(char *str, size_t size, const char* format, va_list args)
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
    return isnan(d);
}

extern "C" int (*PlatformSpecificIsNan)(double) = IsNanImplementation;

static PlatformSpecificMutex DummyMutexCreate(void)
{
    FAIL("PlatformSpecificMutexCreate is not implemented");
    return 0;
}

static void DummyMutexLock(PlatformSpecificMutex mtx)
{
    FAIL("PlatformSpecificMutexLock is not implemented");
}

static void DummyMutexUnlock(PlatformSpecificMutex mtx)
{
    FAIL("PlatformSpecificMutexUnlock is not implemented");
}

static void DummyMutexDestroy(PlatformSpecificMutex mtx)
{
    FAIL("PlatformSpecificMutexDestroy is not implemented");
}

extern "C" PlatformSpecificMutex (*PlatformSpecificMutexCreate)(void) = DummyMutexCreate;
extern "C" void (*PlatformSpecificMutexLock)(PlatformSpecificMutex) = DummyMutexLock;
extern "C" void (*PlatformSpecificMutexUnlock)(PlatformSpecificMutex) = DummyMutexUnlock;
extern "C" void (*PlatformSpecificMutexDestroy)(PlatformSpecificMutex) = DummyMutexDestroy;

void PlatformSpecificInstallCrashTrap(void (*crash_handler)(void *context, const char *crash_message), void *callback_context)
{
    (void)crash_handler;
    (void)callback_context;
}

void PlatformSpecificRemoveCrashTrap(void)
{
}
