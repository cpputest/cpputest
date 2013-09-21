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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/*
 * This file is added for some specific CppUTest configurations that earlier were spread out into multiple files.
 *
 * The goal of this file is to stay really small and not to include other things, but mainly to remove duplication
 * from other files and resolve dependencies in #includes.
 *
 */

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

/* Create a __no_return__ macro, which is used to flag a function as not returning.
 * Used for functions that always throws for instance.
 *
 * This is needed for compiling with clang, without breaking other compilers.
 */
#ifndef __has_attribute
  #define __has_attribute(x) 0
#endif

#if __has_attribute(noreturn)
  #define __no_return__ __attribute__((noreturn))
#else
  #define __no_return__
#endif

#if __has_attribute(format)
  #define __check_format__(type, format_parameter, other_parameters) __attribute__ ((format (type, format_parameter, other_parameters)))
#else
  #define __check_format__(type, format_parameter, other_parameters) /* type, format_parameter, other_parameters */
#endif

/*
 * When we don't link Standard C++, then we won't throw exceptions as we assume the compiler might not support that!
 */

#if CPPUTEST_USE_STD_CPP_LIB
#define UT_THROW(exception) throw (exception)
#define UT_NOTHROW throw()
#else
#define UT_THROW(exception)
#define UT_NOTHROW
#endif

/*
 * CLang's operator delete requires an NOTHROW block. For now, when we use CLang, then have an empty exception specifier.
 * However, this ought to be done inside the configure.ac in the future.
 */

#ifdef __clang__
#undef UT_NOTHROW
#define UT_NOTHROW throw()
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
 * Detection of different 64 bit environments
 */

#if defined(__LP64__) || defined(_LP64) || (defined(__WORDSIZE) && (__WORDSIZE == 64 )) || defined(__x86_64) || defined(_WIN64)
#define CPPUTEST_64BIT
#if defined(_WIN64)
#define CPPUTEST_64BIT_32BIT_LONGS
#endif
#endif

/* Should be the only #include here. Standard C library wrappers */
#include "StandardCLibrary.h"

#endif
