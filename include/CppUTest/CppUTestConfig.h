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

#ifndef CPPUTESTCONFIG_H_
#define CPPUTESTCONFIG_H_

#ifndef CPPUTEST_USE_OWN_CONFIGURATION
#include "CppUTestGeneratedConfig.h"
#endif

/*
 * This file is added for some specific CppUTest configurations that earlier were spread out into multiple files.
 *
 * The goal of this file is to stay really small and not to include other things, but mainly to remove duplication
 * from other files and resolve dependencies in #includes.
 *
 */

#ifdef __clang__
 #pragma clang diagnostic push
 #if (__clang_major__ == 3 && __clang_minor__ >= 6) || __clang_major__ >= 4
  #pragma clang diagnostic ignored "-Wreserved-id-macro"
 #endif
#endif

/*
 * Lib C dependencies that are currently still left:
 *
 * stdarg.h -> We use formatting functions and va_list requires to include stdarg.h in SimpleString
 * stdlib.h -> The TestHarness_c.h includes this to try to avoid conflicts in its malloc #define. This dependency can
 * easily be removed by not enabling the MALLOC overrides.
 *
 * Lib C++ dependencies are all under the CPPUTEST_USE_STD_CPP_LIB.
 * The only dependency is to <new> which has the bad_alloc struct
 *
 */

/* Do we use Standard C or not? When doing Kernel development, standard C usage is out. */
#ifndef CPPUTEST_USE_STD_C_LIB
 #ifdef CPPUTEST_STD_C_LIB_DISABLED
  #define CPPUTEST_USE_STD_C_LIB 0
 #else
  #define CPPUTEST_USE_STD_C_LIB 1
 #endif
#endif


/* Do we use Standard C++ or not? */
#ifndef CPPUTEST_USE_STD_CPP_LIB
 #ifdef CPPUTEST_STD_CPP_LIB_DISABLED
  #define CPPUTEST_USE_STD_CPP_LIB 0
 #else
  #define CPPUTEST_USE_STD_CPP_LIB 1
 #endif
#endif

/* Is memory leak detection enabled?
 *   Controls the override of the global operator new/deleted and malloc/free.
 *   Without this, there will be no memory leak detection in C/C++.
*/

#ifndef CPPUTEST_USE_MEM_LEAK_DETECTION
 #ifdef CPPUTEST_MEM_LEAK_DETECTION_DISABLED
  #define CPPUTEST_USE_MEM_LEAK_DETECTION 0
 #else
  #define CPPUTEST_USE_MEM_LEAK_DETECTION 1
 #endif
#endif

/* Should be the only #include here. Standard C library wrappers */
#include "StandardCLibrary.h"

/* Create a _no_return_ macro, which is used to flag a function as not returning.
 * Used for functions that always throws for instance.
 *
 * This is needed for compiling with clang, without breaking other compilers.
 */
#ifndef __has_attribute
  #define __has_attribute(x) 0
#endif

#if __has_attribute(noreturn)
  #define _no_return_ __attribute__((noreturn))
#else
  #define _no_return_
#endif

#if __has_attribute(format)
  #define _check_format_(type, format_parameter, other_parameters) __attribute__ ((format (type, format_parameter, other_parameters)))
#else
  #define _check_format_(type, format_parameter, other_parameters) /* type, format_parameter, other_parameters */
#endif

/*
 * When we don't link Standard C++, then we won't throw exceptions as we assume the compiler might not support that!
 */

#if CPPUTEST_USE_STD_CPP_LIB
  #if defined(__cplusplus) && __cplusplus >= 201103L
    #define UT_THROW(exception)
    #define UT_NOTHROW noexcept
  #else
    #define UT_THROW(exception) throw (exception)
    #define UT_NOTHROW throw()
  #endif
#else
  #define UT_THROW(exception)
  #ifdef __clang__
    #define UT_NOTHROW throw()
  #else
    #define UT_NOTHROW
  #endif
#endif

/*
 * Visual C++ doesn't define __cplusplus as C++11 yet (201103), however it doesn't want the throw(exception) either, but
 * it does want throw().
 */

#ifdef _MSC_VER
  #undef UT_THROW
  #define UT_THROW(exception)
#endif

#if defined(__cplusplus) && __cplusplus >= 201103L
    #define DEFAULT_COPY_CONSTRUCTOR(classname) classname(const classname &) = default;
#else
    #define DEFAULT_COPY_CONSTRUCTOR(classname)
#endif

/*
 * g++-4.7 with stdc++11 enabled On MacOSX! will have a different exception specifier for operator new (and thank you!)
 * I assume they'll fix this in the future, but for now, we'll change that here.
 * (This should perhaps also be done in the configure.ac)
 */

#ifdef __GXX_EXPERIMENTAL_CXX0X__
#ifdef __APPLE__
#ifdef _GLIBCXX_THROW
#undef UT_THROW
#define UT_THROW(exception) _GLIBCXX_THROW(exception)
#endif
#endif
#endif

/*
 * Address sanitizer is a good thing... and it causes some conflicts with the CppUTest tests
 * To check whether it is on or off, we create a CppUTest define here.
*/
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
#define CPPUTEST_SANITIZE_ADDRESS 1
#endif
#endif

#ifdef __SANITIZE_ADDRESS__
#define CPPUTEST_SANITIZE_ADDRESS 1
#endif

#ifndef CPPUTEST_SANITIZE_ADDRESS
#define CPPUTEST_SANITIZE_ADDRESS 0
#endif

#if CPPUTEST_SANITIZE_ADDRESS
#define CPPUTEST_SANITIZE_ADDRESS 1
#define CPPUTEST_DO_NOT_SANITIZE_ADDRESS __attribute__((no_sanitize_address))
  #if defined(__linux__) && defined(__clang__)
    #if CPPUTEST_USE_MEM_LEAK_DETECTION 
    #warning Compiling with Address Sanitizer with clang on linux will cause duplicate symbols for operator new. Turning off memory leak detection. Compile with -DCPPUTEST_MEM_LEAK_DETECTION_DISABLED to get rid of this warning.
    #undef CPPUTEST_USE_MEM_LEAK_DETECTION
    #define CPPUTEST_USE_MEM_LEAK_DETECTION 0
    #endif
  #endif
#else
#define CPPUTEST_SANITIZER_ADDRESS 0
#define CPPUTEST_DO_NOT_SANITIZE_ADDRESS
#endif

/*
 * Handling of IEEE754 floating point exceptions via fenv.h
 * Predominantly works on non-Visual C++ compilers and Visual C++ 2008 and newer
 */

#if CPPUTEST_USE_STD_C_LIB && \
  (!defined(_MSC_VER) || (_MSC_VER >= 1800)) && \
  (!defined(__APPLE__)) && \
  (!defined(__ghs__) || !defined(__ColdFire__))
#define CPPUTEST_HAVE_FENV
#if defined(__WATCOMC__) || defined(__ARMEL__) || defined(__m68k__)
#define CPPUTEST_FENV_IS_WORKING_PROPERLY 0
#else
#define CPPUTEST_FENV_IS_WORKING_PROPERLY 1
#endif
#endif

/*
 * Detection of different 64 bit environments
 */

#if defined(__LP64__) || defined(_LP64) || (defined(__WORDSIZE) && (__WORDSIZE == 64 )) || defined(__x86_64) || defined(_WIN64)
#define CPPUTEST_64BIT
#if defined(_WIN64)
#define CPPUTEST_64BIT_32BIT_LONGS
#endif
#endif

/* Handling of systems with a different byte-width (e.g. 16 bit).
 * Since CHAR_BIT is defined in limits.h (ANSI C), use default of 8 when building without Std C library.
 */
#if CPPUTEST_USE_STD_C_LIB
#define CPPUTEST_CHAR_BIT CHAR_BIT
#else
#define CPPUTEST_CHAR_BIT 8
#endif

/* Handling of systems with a different int-width (e.g. 16 bit).
 */
#if CPPUTEST_USE_STD_C_LIB && (INT_MAX == 0x7fff)
#define CPPUTEST_16BIT_INTS
#endif

/*
 * Support for "long long" type.
 *
 * Not supported when CPPUTEST_LONG_LONG_DISABLED is set.
 * Can be overridden by using CPPUTEST_USE_LONG_LONG
 *
 * CPPUTEST_HAVE_LONG_LONG_INT is set by configure
 * LLONG_MAX is set in limits.h. This is a crude attempt to detect long long support when no configure is used
 *
 */

#if !defined(CPPUTEST_LONG_LONG_DISABLED) && !defined(CPPUTEST_USE_LONG_LONG)
#if defined(CPPUTEST_HAVE_LONG_LONG_INT) || defined(LLONG_MAX)
#define CPPUTEST_USE_LONG_LONG 1
#endif
#endif

#ifdef CPPUTEST_USE_LONG_LONG
typedef long long cpputest_longlong;
typedef unsigned long long cpputest_ulonglong;
#else
/* Define some placeholders to disable the overloaded methods.
 * It's not required to have these match the size of the "real" type, but it's occasionally convenient.
 */

#if defined(CPPUTEST_64BIT) && !defined(CPPUTEST_64BIT_32BIT_LONGS)
#define CPPUTEST_SIZE_OF_FAKE_LONG_LONG_TYPE 16
#else
#define CPPUTEST_SIZE_OF_FAKE_LONG_LONG_TYPE 8
#endif

struct cpputest_longlong
{
#if defined(__cplusplus)
  cpputest_longlong() {}
  cpputest_longlong(int) {}
#endif
  char dummy[CPPUTEST_SIZE_OF_FAKE_LONG_LONG_TYPE];
};

struct cpputest_ulonglong
{
#if defined(__cplusplus)
  cpputest_ulonglong() {}
  cpputest_ulonglong(int) {}
#endif
  char dummy[CPPUTEST_SIZE_OF_FAKE_LONG_LONG_TYPE];
};

#if !defined(__cplusplus)
typedef struct cpputest_longlong cpputest_longlong;
typedef struct cpputest_ulonglong cpputest_ulonglong;
#endif

#endif

/* Visual C++ 10.0+ (2010+) supports the override keyword, but doesn't define the C++ version as C++11 */
#if defined(__cplusplus) && ((__cplusplus >= 201103L) || (defined(_MSC_VER) && (_MSC_VER >= 1600)))
#if !defined(__ghs__)
#define CPPUTEST_COMPILER_FULLY_SUPPORTS_CXX11
#define _override override
#else
/* GreenHills is not compatible with other compilers with regards to where
 * it expects the override specifier to be on methods that return function
 * pointers. Given this, it is easiest to not use the override specifier.
 */
#define _override
#endif
#define NULLPTR nullptr
#else
#define _override
#define NULLPTR NULL
#endif

/* Visual C++ 11.0+ (2012+) supports the override keyword on destructors */
#if defined(__cplusplus) && ((__cplusplus >= 201103L) || (defined(_MSC_VER) && (_MSC_VER >= 1700)))
#define _destructor_override override
#else
#define _destructor_override
#endif

#ifdef __clang__
 #pragma clang diagnostic pop
#endif


#endif
