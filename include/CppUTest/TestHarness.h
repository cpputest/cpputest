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

#ifndef D_TestHarness_h
#define D_TestHarness_h

/* Memory leak detector macros:
 *
 * UT_NEW_MACROS_ENABLED/DISABLED
 *   Control the #define new. When DISABLED, new with not be #defined.
 *   Without this, the memory leak detector will not know the file/line location of leaks.
 * UT_NEW_OVERRIDES_ENABLED/DISABLED
 *   Controls the override of the global operator new/deleted.
 *   Without this, there will be no memory leak detection in C++.
 * UT_MALLOC_MACROS_ENABLED/DISABLED
 *   Controls the #define of malloc for enabling memory leak detection in C.
 */

#ifndef UT_NEW_MACROS_ENABLED
   #ifdef UT_NEW_MACROS_DISABLED
      #define UT_NEW_MACROS_ENABLED 0
   #else
      #define UT_NEW_MACROS_ENABLED 1
   #endif
#endif

#ifndef UT_NEW_OVERRIDES_ENABLED
   #ifdef UT_NEW_OVERRIDES_DISABLED
      #define UT_NEW_OVERRIDES_ENABLED 0
      #undef UT_NEW_MACROS_ENABLED
      #define UT_NEW_MACROS_ENABLED 0
   #else
      #define UT_NEW_OVERRIDES_ENABLED 1
   #endif
#endif

/* original value was 9973 which works will with large programs. Now set to smaller since it takes
 * a lot of memory in embedded apps. Change it if you experience the memory leak detector to be slow.
 */

#define MEMORY_LEAK_HASH_TABLE_SIZE 73

/*
 * Lib C dependencies that are currently still left:
 *
 * stdarg.h -> We use formatting functions and va_list requires to include stdarg.h
 * stdlib.h -> The TestHarness_c.h includes this to try to avoid conflicts in its malloc #define. This dependency can
 * easily be removed by not enabling the MALLOC overrides.
 */

#include <stdarg.h>

#include "Utest.h"
#include "UtestMacros.h"
#include "SimpleString.h"
#include "TestResult.h"
#include "Failure.h"
#include "TestPlugin.h"
#include "MemoryLeakWarningPlugin.h"
#endif
