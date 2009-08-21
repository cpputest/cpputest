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

/******************************************************************************
 *
 * TESTHARNESS_c.H
 *
 * Provides an interface for when working with pure C
 *
 * Remember to use extern "C" when including in a cpp file!
 *
*******************************************************************************/

#ifndef D_TestHarness_c_h
#define D_TestHarness_c_h

#define CHECK_EQUAL_C_INT(expected,actual) \
  CHECK_EQUAL_C_INT_LOCATION(expected,actual,__FILE__,__LINE__)

#define CHECK_EQUAL_C_REAL(expected,actual,threshold) \
  CHECK_EQUAL_C_REAL_LOCATION(expected,actual,threshold,__FILE__,__LINE__)

#define CHECK_EQUAL_C_CHAR(expected,actual) \
  CHECK_EQUAL_C_CHAR_LOCATION(expected,actual,__FILE__,__LINE__)

#define CHECK_EQUAL_C_STRING(expected,actual) \
  CHECK_EQUAL_C_STRING_LOCATION(expected,actual,__FILE__,__LINE__)

#define FAIL_TEXT_C(text) \
  FAIL_TEXT_C_LOCATION(text,__FILE__,__LINE__)

#define FAIL_C() \
  FAIL_C_LOCATION(__FILE__,__LINE__)

#define CHECK_C(condition) \
  CHECK_C_LOCATION(condition, #condition, __FILE__,__LINE__)

/* CHECKS that can be used from C code */
extern void  CHECK_EQUAL_C_INT_LOCATION(int expected, int actual, const char* fileName, int lineNumber);
extern void  CHECK_EQUAL_C_REAL_LOCATION(double expected, double actual, double threshold, const char* fileName, int lineNumber);
extern void  CHECK_EQUAL_C_CHAR_LOCATION(char expected, char actual, const char* fileName, int lineNumber);
extern void  CHECK_EQUAL_C_STRING_LOCATION(const char* expected, const char* actual, const char* fileName, int lineNumber);
extern void  FAIL_TEXT_C_LOCATION(const char* text, const char* fileName, int lineNumber);
extern void  FAIL_C_LOCATION(const char* fileName, int lineNumber);
extern void  CHECK_C_LOCATION(int condition, const char* conditionString, const char* fileName, int lineNumber);

/* Memory allocation routines that use new, so the memory leak checker works */

#ifndef UT_MALLOC_MACROS_ENABLED
   #ifdef UT_MALLOC_MACROS_DISABLED
      #define UT_MALLOC_MACROS_ENABLED 0
   #else
      #define UT_MALLOC_MACROS_ENABLED 1
   #endif
#endif

#if UT_MALLOC_MACROS_ENABLED
   #undef malloc
   #undef free
   #define malloc std_malloc
   #define free std_free
   #define calloc std_calloc
   #define realloc std_realloc
   #include <stdlib.h>
   #undef malloc
   #undef free
   #undef realloc
   #undef calloc
#else
   #include <stdlib.h>
#endif

extern void* cpputest_malloc(size_t size);
extern void* cpputest_calloc(size_t num, size_t size);
extern void* cpputest_realloc(void* ptr, size_t size);
extern void cpputest_free(void* buffer);

extern void* cpputest_malloc_location(size_t size,const char* file, int line);
extern void* cpputest_calloc_location(size_t num, size_t size,const char* file, int line);
extern void* cpputest_realloc_location(void* memory, size_t size, const char* file, int line);
extern void cpputest_free_location(void* buffer, const char* file, int line);

#if UT_MALLOC_MACROS_ENABLED
   #define malloc(a) cpputest_malloc_location(a, __FILE__, __LINE__)
   #define calloc(a, b) cpputest_calloc_location(a, b, __FILE__, __LINE__)
   #define realloc(a, b) cpputest_realloc_location(a, b, __FILE__, __LINE__)
   #define free(a) cpputest_free_location(a, __FILE__, __LINE__)
#endif

void cpputest_malloc_set_out_of_memory();
void cpputest_malloc_set_not_out_of_memory();

#endif
