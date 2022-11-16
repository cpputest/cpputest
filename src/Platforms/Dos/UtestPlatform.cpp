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

/* Un-comment to use buffer instead of std out */
// #define USE_BUFFER_OUTPUT 1
#include <cstdlib>

#include "CppUTest/TestHarness.h"
#undef malloc
#undef free
#undef calloc
#undef realloc
#undef strdup
#undef strndup

#define  far  // eliminate "meaningless type qualifier" warning
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#undef far

#include "CppUTest/PlatformSpecificFunctions.h"

static jmp_buf test_exit_jmp_buf[10];
static int jmp_buf_index = 0;

TestOutput::WorkingEnvironment PlatformSpecificGetWorkingEnvironment()
{
    return TestOutput::eclipse;
}

static void DummyRunTestInASeperateProcess(UtestShell* shell, TestPlugin* plugin, TestResult* result)
{
    result->addFailure(TestFailure(shell, "-p doesn't work on this platform, as it is lacking fork.\b"));
}

static int DummyPlatformSpecificFork(void)
{
    return 0;
}

static int DummyPlatformSpecificWaitPid(int, int*, int)
{
    return 0;
}

void (*PlatformSpecificRunTestInASeperateProcess)(UtestShell*, TestPlugin*, TestResult*) = DummyRunTestInASeperateProcess;
int (*PlatformSpecificFork)() = DummyPlatformSpecificFork;
int (*PlatformSpecificWaitPid)(int, int*, int) = DummyPlatformSpecificWaitPid;

extern "C" {

static int DosSetJmp(void (*function) (void* data), void* data)
{
    if (0 == setjmp(test_exit_jmp_buf[jmp_buf_index])) {
        jmp_buf_index++;
        function(data);
        jmp_buf_index--;
        return 1;
    }
    return 0;
}

static void  DosLongJmp()
{
    jmp_buf_index--;
    longjmp(test_exit_jmp_buf[jmp_buf_index], 1);
}

static void  DosRestoreJumpBuffer()
{
    jmp_buf_index--;
}

int (*PlatformSpecificSetJmp)(void (*function) (void*), void*) = DosSetJmp;
void (*PlatformSpecificLongJmp)(void) = DosLongJmp;
void (*PlatformSpecificRestoreJumpBuffer)(void) = DosRestoreJumpBuffer;

static long DosTimeInMillis()
{
    return clock() * 1000 / CLOCKS_PER_SEC;
}

static const char* DosTimeString()
{
    time_t tm = time(NULL);
    return ctime(&tm);
}

static int DosVSNprintf(char* str, size_t size, const char* format, va_list args) {
    return vsnprintf(str, size, format, args);
}

long (*GetPlatformSpecificTimeInMillis)() = DosTimeInMillis;
const char* (*GetPlatformSpecificTimeString)() = DosTimeString;
int (*PlatformSpecificVSNprintf)(char *, size_t, const char*, va_list) = DosVSNprintf;

PlatformSpecificFile DosFOpen(const char* filename, const char* flag)
{
    return fopen(filename, flag);
}

static void DosFPuts(const char* str, PlatformSpecificFile file)
{
   fputs(str, (FILE*)file);
}

static void DosFClose(PlatformSpecificFile file)
{
   fclose((FILE*)file);
}

PlatformSpecificFile PlatformSpecificStdOut = stdout;
PlatformSpecificFile (*PlatformSpecificFOpen)(const char* filename, const char* flag) = DosFOpen;
void (*PlatformSpecificFPuts)(const char* str, PlatformSpecificFile file) = DosFPuts;
void (*PlatformSpecificFClose)(PlatformSpecificFile file) = DosFClose;

static void DosFlush()
{
  fflush(stdout);
}

extern void (*PlatformSpecificFlush)(void) = DosFlush;

static void* DosMalloc(size_t size)
{
   return malloc(size);
}

static void* DosRealloc (void* memory, size_t size)
{
    return realloc(memory, size);
}

static void DosFree(void* memory)
{
    free(memory);
}

static void* DosMemCpy(void* s1, const void* s2, size_t size)
{
    return memcpy(s1, s2, size);
}

static void* DosMemset(void* mem, int c, size_t size)
{
    return memset(mem, c, size);
}

void* (*PlatformSpecificMalloc)(size_t size) = DosMalloc;
void* (*PlatformSpecificRealloc)(void* memory, size_t size) = DosRealloc;
void (*PlatformSpecificFree)(void* memory) = DosFree;
void* (*PlatformSpecificMemCpy)(void* s1, const void* s2, size_t size) = DosMemCpy;
void* (*PlatformSpecificMemset)(void* mem, int c, size_t size) = DosMemset;

static void DosSrand(unsigned int seed)
{
    srand(seed);
}

static int DosRand()
{
    return rand();
}

static double DosFabs(double d)
{
    return fabs(d);
}

static int DosIsNan(double d)
{
    return isnan(d);
}

static int DosIsInf(double d)
{
    return isinf(d);
}

void (*PlatformSpecificSrand)(unsigned int) = DosSrand;
int (*PlatformSpecificRand)(void) = DosRand;
double (*PlatformSpecificFabs)(double) = DosFabs;
int (*PlatformSpecificIsNan)(double d) = DosIsNan;
int (*PlatformSpecificIsInf)(double d) = DosIsInf;

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

PlatformSpecificMutex (*PlatformSpecificMutexCreate)(void) = DummyMutexCreate;
void (*PlatformSpecificMutexLock)(PlatformSpecificMutex) = DummyMutexLock;
void (*PlatformSpecificMutexUnlock)(PlatformSpecificMutex) = DummyMutexUnlock;
void (*PlatformSpecificMutexDestroy)(PlatformSpecificMutex) = DummyMutexDestroy;

static void DosAbort()
{
    abort();
}

void (*PlatformSpecificAbort)(void) = DosAbort;

}
