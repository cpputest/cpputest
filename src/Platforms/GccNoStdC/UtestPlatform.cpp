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

#include "CppUTest/PlatformSpecificFunctions.h"

void (*PlatformSpecificRunTestInASeperateProcess)(UtestShell*, TestPlugin*, TestResult*) = NULL;
int (*PlatformSpecificFork)() = NULL;
int (*PlatformSpecificWaitPid)(int, int*, int) = NULL;

/* IO operations */
PlatformSpecificFile (*PlatformSpecificFOpen)(const char* filename, const char* flag) = NULL;
void (*PlatformSpecificFPuts)(const char* str, PlatformSpecificFile file) = NULL;
void (*PlatformSpecificFClose)(PlatformSpecificFile file) = NULL;

int (*PlatformSpecificPutchar)(int c) = NULL;
void (*PlatformSpecificFlush)(void) = NULL;

/* Dynamic Memory operations */
void* (*PlatformSpecificMalloc)(size_t) = NULL;
void* (*PlatformSpecificRealloc)(void*, size_t) = NULL;
void (*PlatformSpecificFree)(void*) = NULL;
void* (*PlatformSpecificMemCpy)(void*, const void*, size_t) = NULL;
void* (*PlatformSpecificMemset)(void*, int, size_t) = NULL;
int (*PlatformSpecificMemCmp)(const void*, const void*, size_t) = NULL;

PlatformSpecificMutex (*PlatformSpecificMutexCreate)(void) = NULL;
void (*PlatformSpecificMutexLock)(PlatformSpecificMutex mtx) = NULL;
void (*PlatformSpecificMutexUnlock)(PlatformSpecificMutex mtx) = NULL;
void (*PlatformSpecificMutexDestroy)(PlatformSpecificMutex mtx) = NULL;

