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
extern "C" {
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
}
#undef far
#include "CppUTest/PlatformSpecificFunctions.h"

static jmp_buf test_exit_jmp_buf[10];
static int jmp_buf_index = 0;

#if USE_BUFFER_OUTPUT
    // Buffer for crude output routine
    #define BUFFER_SIZE 4096
    static char buffer [BUFFER_SIZE]; /* "never used" warning is OK */
    static int idx = 0;
#endif

TestOutput::WorkingEnvironment PlatformSpecificGetWorkingEnvironment()
{
    return TestOutput::eclipse;
}

static void C2000RunTestInASeperateProcess(UtestShell* shell, TestPlugin* plugin, TestResult* result)
{
    result->addFailure(TestFailure(shell, "-p doesn't work on this platform, as it is lacking fork.\b"));
}

void (*PlatformSpecificRunTestInASeperateProcess)(UtestShell*, TestPlugin*, TestResult*) =
    C2000RunTestInASeperateProcess;

extern "C" {

static int C2000SetJmp(void (*function) (void* data), void* data)
{
    if (0 == setjmp(test_exit_jmp_buf[jmp_buf_index])) {
        jmp_buf_index++;
        function(data);
        jmp_buf_index--;
        return 1;
    }
    return 0;
}

static void  C2000LongJmp()
{
    jmp_buf_index--;
    longjmp(test_exit_jmp_buf[jmp_buf_index], 1);
}

static void  C2000RestoreJumpBuffer()
{
    jmp_buf_index--;
}

int (*PlatformSpecificSetJmp)(void (*function) (void*), void*) = C2000SetJmp;
void (*PlatformSpecificLongJmp)(void) = C2000LongJmp;
void (*PlatformSpecificRestoreJumpBuffer)(void) = C2000RestoreJumpBuffer;

static long C2000TimeInMillis()
{
    /* The TI c2000 platform does not have Posix support and thus lacks struct timespec.
     * Also, clock() always returns 0 in the simulator. Hence we work with struct tm.tm_hour
     * This has two consequences:
     *   (1) We need to sum up the part in order to get an "elapsed since" time value,
     *       rather than just using tm_sec.
     *   (2) There is a possibility of overflow, since we stop at the hour
     *   (3) Resolution is 1 s, even though we return ms.
     */
    time_t t        = time((time_t*)0);
    struct tm * ptm = gmtime(&t);
    long result = (long)
        ((ptm->tm_sec + ptm->tm_min * (time_t)60 + ptm->tm_hour * (time_t)3600) * (time_t)1000);
    return result;
}

static const char* TimeStringImplementation()
{
    time_t tm = time(NULL);
    return ctime(&tm);
}

long (*GetPlatformSpecificTimeInMillis)() = C2000TimeInMillis;
const char* (*GetPlatformSpecificTimeString)() = TimeStringImplementation;

extern int vsnprintf(char*, size_t, const char*, va_list); // not std::vsnprintf()

extern int (*PlatformSpecificVSNprintf)(char *, size_t, const char*, va_list) = vsnprintf;

PlatformSpecificFile C2000FOpen(const char* filename, const char* flag)
{
    return fopen(filename, flag);
}

static void C2000FPuts(const char* str, PlatformSpecificFile file)
{
#if USE_BUFFER_OUTPUT
    if (file == PlatformSpecificStdOut) {
        while (*str && (idx < BUFFER_SIZE)) {
            buf[idx++] = *str++;
        }
    }
    else
#endif
    { 
        fputs(str, (FILE*)file);
    }
}

static void C2000FClose(PlatformSpecificFile file)
{
   fclose((FILE*)file);
}

PlatformSpecificFile PlatformSpecificStdOut = stdout;
PlatformSpecificFile (*PlatformSpecificFOpen)(const char* filename, const char* flag) = C2000FOpen;
void (*PlatformSpecificFPuts)(const char* str, PlatformSpecificFile file) = C2000FPuts;
void (*PlatformSpecificFClose)(PlatformSpecificFile file) = C2000FClose;

static void CL2000Flush()
{
  fflush(stdout);
}

extern void (*PlatformSpecificFlush)(void) = CL2000Flush;

static void* C2000Malloc(size_t size)
{
   return (void*)malloc((unsigned long)size);
}

static void* C2000Realloc (void* memory, size_t size)
{
    return (void*)realloc(memory, (unsigned long)size);
}

static void C2000Free(void* memory)
{
    free(memory);
}

static void* C2000MemCpy(void* s1, const void* s2, size_t size)
{
    return (void*)memcpy(s1, s2, size);
}

static void* C2000Memset(void* mem, int c, size_t size)
{
    register unsigned long i = size;
    register long p = (long) mem;
    while (i--) *__farptr_to_word(p++) = c;
    return mem;
}

void* (*PlatformSpecificMalloc)(size_t size) = C2000Malloc;
void* (*PlatformSpecificRealloc)(void* memory, size_t size) = C2000Realloc;
void (*PlatformSpecificFree)(void* memory) = C2000Free;
void* (*PlatformSpecificMemCpy)(void* s1, const void* s2, size_t size) = C2000MemCpy;
void* (*PlatformSpecificMemset)(void* mem, int c, size_t size) = C2000Memset;

/*
double PlatformSpecificFabs(double d)
{
   return fabs(d);
}
*/
double (*PlatformSpecificFabs)(double) = fabs;

static int IsNanImplementation(double d)
{
    return 0;
}

static int IsInfImplementation(double d)
{
    return 0;
}

int (*PlatformSpecificIsNan)(double d) = IsNanImplementation;
int (*PlatformSpecificIsInf)(double d) = IsInfImplementation;

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
void (*PlatformSpecificSrand)(unsigned int) = srand;
int (*PlatformSpecificRand)(void) = rand;
void (*PlatformSpecificMutexLock)(PlatformSpecificMutex) = DummyMutexLock;
void (*PlatformSpecificMutexUnlock)(PlatformSpecificMutex) = DummyMutexUnlock;
void (*PlatformSpecificMutexDestroy)(PlatformSpecificMutex) = DummyMutexDestroy;
void (*PlatformSpecificAbort)(void) = abort;

}
