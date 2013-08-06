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

/* Un-comment to use buffer instead of std out */
// #define USE_BUFFER_OUTPUT 1
#include <cstdlib>

#include "CppUTest/TestHarness.h"
#undef malloc
#undef free
#undef calloc
#undef realloc

#define  far  // eliminate "meaningless type qualifier" warning
#include "CppUTest/TestRegistry.h"
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

#if USE_BUFFER_OUTPUT
    // Buffer for crude output routine 
    #define BUFFER_SIZE 4096
    static char buffer [BUFFER_SIZE]; /* "never used" warning is OK */
    static int idx = 0;
#endif

/* The TI cl2000 compiler will compile this function with C++ linkage, unless
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
   printf("-p isn' implemented for c2000. Running inside the process\b");
   shell->runOneTest(plugin, *result);
}

TestOutput::WorkingEnvironment PlatformSpecificGetWorkingEnvironment()
{
	return TestOutput::eclipse;
}

///////////// Time in millis

static long TimeInMillisImplementation()
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

static long (*timeInMillisFp) () = TimeInMillisImplementation;

long GetPlatformSpecificTimeInMillis()
{
	return timeInMillisFp();
}

/* The TI cl2000 compiler will compile this function with C++ linkage, unless
 * we specifically tell it to use C linkage again, in the function definiton. .
 */

typedef long (*cppLongCall)();

extern "C" void SetPlatformSpecificTimeInMillisMethod(long (*platformSpecific) ())
{
    if(platformSpecific == 0) 
	{
     	timeInMillisFp = TimeInMillisImplementation;
    }
    else
    {
        timeInMillisFp = reinterpret_cast<cppLongCall>(platformSpecific);
	}
}

///////////// Time in String

static const char* TimeStringImplementation()
{
	time_t tm = time(NULL);
	return ctime(&tm);
}

static const char* (*timeStringFp) () = TimeStringImplementation;

const char* GetPlatformSpecificTimeString()
{
	return timeStringFp();
}

/* The TI cl2000 compiler will compile this function with C++ linkage, unless
 * we specifically tell it to use C linkage again, in the function definiton. 
 * Having the extern "C" here unfortunately makes the function argument
 * extern "C" also. This has to be cast away explicitly.
 */

typedef const char* (*cppStringFp)();

extern "C" void SetPlatformSpecificTimeStringMethod(const char* (*platformMethod) ())
{
    if(platformMethod == 0)
	{
	    timeStringFp = TimeStringImplementation;
	}
	else
	{
	    timeStringFp = reinterpret_cast<cppStringFp>(platformMethod);
	}
}

int PlatformSpecificAtoI(const char* str)
{
   return atoi(str);
}

size_t PlatformSpecificStrLen(const char* str)
{
    size_t n = (size_t)-1;
    do n++; while (*str++);
    return n;
}

char* PlatformSpecificStrCat(char* s1, const char* s2)
{
    char *result = s1;

    while (*s1++);			       // FIND END OF STRING
    --s1;				           // BACK UP OVER NULL
    while (*s1++ = *s2++);	       // APPEND SECOND STRING
    return result;
}

char* PlatformSpecificStrCpy(char* dest, const char* src)
{
    register char       *d = dest;     
    register const char *s = src;

    while (*d++ = *s++);
    return dest;
}

char* PlatformSpecificStrNCpy(char* s1, const char* s2, size_t n)
{
    char *result = s1;

    if (n == 0) return result; 
    while ((*s1++ = *s2++) && --n != 0);  // COPY STRING
    if (n > 1)
    {
        --n;
        do *s1++ = 0; while (--n);       // TERMINATION PADDING
    }
    return result;
}

int PlatformSpecificStrCmp(const char* s1, const char* s2)
{
    for(;;)
    {
        if (!*s1) return(0 - *s2);
        if (*s1++ != *s2++)
	        return(*--s1 - *--s2);
    }
}

int PlatformSpecificStrNCmp(const char* s1, const char* s2, size_t n)
{
    unsigned int   ch1, diff;

    if (n == 0)  return 0;

    while (n-- && !(diff = (ch1 = *s1++) - *s2++) && ch1 != 0) ;
    return diff;
}

char* PlatformSpecificStrStr(const char* s1, const char* s2)
{
    const char *cmp;
    const char *wpos;
 
    // ----------------------------------------------------------------------
    // TRY EVERY POSITION IN THE CONTROL STRING
    // ----------------------------------------------------------------------
    for (wpos = s1; *s1; wpos = ++s1)
    {
        cmp = s2;
 
        do
        {
	        // ----------------------------------------------------------------
	        //  AT END OF COMPARISON STRING, MUST HAVE A MATCH OR EMPTY STRING
	        // ----------------------------------------------------------------
	        if (!*cmp)  return (char *)s1;
 
	        // ----------------------------------------------------------------
	        // AT END OF CONTROL STRING, NO MATCH IS POSSIBLE
	        // ----------------------------------------------------------------
	        if (!*wpos) return NULL;
 
       } while (*wpos++ == *cmp++);
   }
   return NULL;
}

extern "C" int vsnprintf(char*, size_t, const char*, va_list); // not std::vsnprintf()

int PlatformSpecificVSNprintf(char *str, size_t size, const char* format, va_list args)
{
    return vsnprintf(str, size, format, args);
}

char PlatformSpecificToLower(char c)
{
	return (char) tolower((char) c);
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
#if USE_BUFFER_OUTPUT
    if(idx < BUFFER_SIZE) {
        buffer[idx] = (char) c;
        idx++;
        /* "buffer[idx]" instead of "c" eliminates "never used" warning */
 		return (buffer[idx]);
    }
    else {
        return EOF;
    }
#else
    return putchar(c);
#endif
}

void* PlatformSpecificMalloc(size_t size)
{
   return (void*)far_malloc((unsigned long)size);
}

void* PlatformSpecificRealloc (void* memory, size_t size)
{
    return (void*)far_realloc((long)memory, (unsigned long)size);
}

void PlatformSpecificFree(void* memory)
{
    far_free((long)memory);
}

void* PlatformSpecificMemCpy(void* s1, const void* s2, size_t size)
{
    return (void*)far_memlcpy((long)s1, (long)s2, size);
}

void* PlatformSpecificMemset(void* mem, int c, size_t size)
{
    register unsigned long i = size;
	register long p = (long) mem;
	while (i--) *__farptr_to_word(p++) = c;
	return mem;
}

double PlatformSpecificFabs(double d)
{
   return fabs(d);
}

int PlatformSpecificIsNan(double d)
{
    /* This should work, but doesn't on the C2000, because it does not
     * seem to have any concept of NaN whatsoever (e.g. the constant
     * 0.0 / 0.0 evaluates to 0.0).
     */
    volatile double temp1 = d;
	volatile double temp2 = d;
	return temp1 != temp2;
}
