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

///////////////////////////////////////////////////////////////////////////////
//
// TESTHARNESS.H
//
// The primary include file for the framework.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef D_TestHarness_h
#define D_TestHarness_h

#ifndef UT_NEW_MACROS_ENABLED
   #ifdef UT_NEW_MACROS_DISABLED
      #define UT_NEW_MACROS_ENABLED 0
   #else
      #define UT_NEW_MACROS_ENABLED 1
   #endif
#endif

#ifndef UT_NEW_OVERRIDES_ENABLED
   #ifdef UT_NEW_OVERRIDES_DISABLED
      #define UT_NEW_OVERRIDES_ENABLED 1
   #else
      #define UT_NEW_OVERRIDES_ENABLED 0
      #undef UT_NEW_MACROS_ENABLED
      #define UT_NEW_MACROS_ENABLED 0
   #endif
#endif

#include "Utest.h"
#include "UtestMacros.h"
#include "SimpleString.h"
#include "TestResult.h"
#include "Failure.h"
#include "TestPlugin.h"
#include "TestRegistry.h"
#include "MemoryLeakWarningPlugin.h"
#include "TestPlugin.h"
#include <stdarg.h>

long GetPlatformSpecificTimeInMillis();
void SetPlatformSpecificTimeInMillisMethod(long (*platformSpecific) ());

SimpleString GetPlatformSpecificTimeString();
void SetPlatformSpecificTimeStringMethod(SimpleString (*platformMethod) ());

extern void (*PlatformSpecificExitCurrentTest)();
extern void FakePlatformSpecificExitCurrentTest();
extern void PlatformSpecificExitCurrentTestImpl();

int PlatformSpecificSprintf(char *str, unsigned int size, const char *format, ...);
int PlatformSpecificVSNprintf(char *str, unsigned int size, const char* format, void* va_args_list);
int PlatformSpecificVSNprintf(char *str, unsigned int size, const char* format, va_list va_args_list);
int PlatformSpecificVSNprintf2(char *str, unsigned int size, const char* format, va_list va_args_list);
int PlatformSpecificPutchar(int c);
void PlatformSpecificFlush();
#endif

