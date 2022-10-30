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

#include "CppUTest/TestHarness.h"
#undef malloc
#undef free
#undef calloc
#undef realloc
#undef strdup
#undef strndup

#include "CppUTest/PlatformSpecificFunctions.h"

void (*PlatformSpecificRunTestInASeperateProcess)(UtestShell*, TestPlugin*, TestResult*) = nullptr;
int (*PlatformSpecificFork)() = nullptr;
int (*PlatformSpecificWaitPid)(int, int*, int) = nullptr;

TestOutput::WorkingEnvironment PlatformSpecificGetWorkingEnvironment()
{
    return TestOutput::eclipse;
}

void (*PlatformSpecificLongJmp)() = nullptr;
int (*PlatformSpecificSetJmp)(void (*)(void*), void*) = nullptr;
void (*PlatformSpecificRestoreJumpBuffer)() = nullptr;

long (*GetPlatformSpecificTimeInMillis)() = nullptr;
const char* (*GetPlatformSpecificTimeString)() = nullptr;

/* IO operations */
PlatformSpecificFile PlatformSpecificStdOut = nullptr;
PlatformSpecificFile (*PlatformSpecificFOpen)(const char* filename, const char* flag) = nullptr;
void (*PlatformSpecificFPuts)(const char* str, PlatformSpecificFile file) = nullptr;
void (*PlatformSpecificFClose)(PlatformSpecificFile file) = nullptr;

void (*PlatformSpecificFlush)(void) = nullptr;

int (*PlatformSpecificVSNprintf)(char *str, size_t size, const char* format, va_list va_args_list) = nullptr;

/* Dynamic Memory operations */
void* (*PlatformSpecificMalloc)(size_t) = nullptr;
void* (*PlatformSpecificRealloc)(void*, size_t) = nullptr;
void (*PlatformSpecificFree)(void*) = nullptr;
void* (*PlatformSpecificMemCpy)(void*, const void*, size_t) = nullptr;
void* (*PlatformSpecificMemset)(void*, int, size_t) = nullptr;

double (*PlatformSpecificFabs)(double) = nullptr;
int (*PlatformSpecificIsNan)(double) = nullptr;
int (*PlatformSpecificIsInf)(double) = nullptr;
int (*PlatformSpecificAtExit)(void(*func)(void)) = nullptr;

PlatformSpecificMutex (*PlatformSpecificMutexCreate)(void) = nullptr;
void (*PlatformSpecificMutexLock)(PlatformSpecificMutex mtx) = nullptr;
void (*PlatformSpecificMutexUnlock)(PlatformSpecificMutex mtx) = nullptr;
void (*PlatformSpecificMutexDestroy)(PlatformSpecificMutex mtx) = nullptr;

void (*PlatformSpecificSrand)(unsigned int) = nullptr;
int (*PlatformSpecificRand)(void) = nullptr;
