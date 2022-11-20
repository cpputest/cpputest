/*
 * Copyright (c) 2011, Michael Feathers, James Grenning and Bas Vodde
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

#ifndef GTEST__H_
#define GTEST__H_

#undef new
#undef strdup
#undef strndup

#undef RUN_ALL_TESTS

#include "gtest/gtest.h"

#ifdef CPPUTEST_USE_NEW_MACROS
#include "CppUTest/MemoryLeakDetectorNewMacros.h"
#endif

#ifdef CPPUTEST_USE_MALLOC_MACROS
#include "CppUTest/MemoryLeakDetectorMallocMacros.h"
#endif

#include "CppUTestExt/GTestSupport.h"

#ifndef RUN_ALL_TESTS
#define GTEST_VERSION_GTEST_1_7
#else
#ifdef ADD_FAILURE_AT
#define GTEST_VERSION_GTEST_1_6
#else
#define GTEST_VERSION_GTEST_1_5
#endif
#endif

#undef RUN_ALL_TESTS

#endif
