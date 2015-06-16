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

extern "C" int PlatformSpecificSetJmpImplementation(void (*function) (void* data), void* data)
{
    if (0 == setjmp(test_exit_jmp_buf[jmp_buf_index])) {
        jmp_buf_index++;
        function(data);
        jmp_buf_index--;
        return 1;
    }
    return 0;
}

static void PlatformSpecificLongJmpImplementation()
{
    jmp_buf_index--;
    longjmp(test_exit_jmp_buf[jmp_buf_index], 1);
}

static void PlatformSpecificRestoreJumpBufferImplementation()
{
    jmp_buf_index--;
}

extern "C" void (*PlatformSpecificLongJmp)() = PlatformSpecificLongJmpImplementation;
extern "C" int (*PlatformSpecificSetJmp)(void (*)(void*), void*) = PlatformSpecificSetJmpImplementation;
extern "C" void (*PlatformSpecificRestoreJumpBuffer)() = PlatformSpecificRestoreJumpBufferImplementation;


static void PlatformSpecificRunTestInASeperateProcessImplementation(UtestShell* shell, TestPlugin* plugin, TestResult* result)
{
    printf("-p isn' implemented for armcc. Running inside the process\b");
    shell->runOneTest(plugin, *result);
}

void (*PlatformSpecificRunTestInASeperateProcess)(UtestShell* shell, TestPlugin* plugin, TestResult* result) =
        PlatformSpecificRunTestInASeperateProcessImplementation;


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
extern "C" int (*PlatformSpecificVSNprintf)(char *str, size_t size, const char* format, va_list args) = vsnprintf;

static PlatformSpecificFile PlatformSpecificFOpenImplementation(const char* filename, const char* flag)
{
   return fopen(filename, flag);
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

extern "C" PlatformSpecificFile (*PlatformSpecificFOpen)(const char*, const char*) = PlatformSpecificFOpenImplementation;
extern "C" void (*PlatformSpecificFPuts)(const char*, PlatformSpecificFile) = PlatformSpecificFPutsImplementation;
extern "C" void (*PlatformSpecificFClose)(PlatformSpecificFile) = PlatformSpecificFCloseImplementation;

extern "C" int (*PlatformSpecificPutchar)(int) = putchar;
extern "C" void (*PlatformSpecificFlush)() = PlatformSpecificFlushImplementation;
extern "C" void* (*PlatformSpecificMalloc)(size_t) = malloc;
extern "C" void* (*PlatformSpecificRealloc) (void*, size_t) = realloc;
extern "C" void (*PlatformSpecificFree)(void*) = free;
extern "C" void* (*PlatformSpecificMemCpy)(void* s1, const void* s2, size_t size) = memcpy;
extern "C" void* (*PlatformSpecificMemset)(void*, int, size_t) = memset;

static int IsNanImplementation(double d)
{
    return isnan(d);
}

extern "C" int (*PlatformSpecificIsNan)(double) = IsNanImplementation;
extern "C" double (*PlatformSpecificFabs)(double) = fabs;extern "C" int (*PlatformSpecificAtExit)(void(*func)(void)) = atexit;  /// this was undefined before

static PlatformSpecificMutex DummyMutexCreate(void)
{
    return 0;
}

static void DummyMutexLock(PlatformSpecificMutex mtx)
{
}

static void DummyMutexUnlock(PlatformSpecificMutex mtx)
{
}

static void DummyMutexDestroy(PlatformSpecificMutex mtx)
{
}

extern "C" PlatformSpecificMutex (*PlatformSpecificMutexCreate)(void) = DummyMutexCreate;
extern "C" void (*PlatformSpecificMutexLock)(PlatformSpecificMutex) = DummyMutexLock;
extern "C" void (*PlatformSpecificMutexUnlock)(PlatformSpecificMutex) = DummyMutexUnlock;
extern "C" void (*PlatformSpecificMutexDestroy)(PlatformSpecificMutex) = DummyMutexDestroy;

