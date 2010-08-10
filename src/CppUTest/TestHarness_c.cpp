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
// TESTHARNESS_c.H
//
//
///////////////////////////////////////////////////////////////////////////////
#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetector.h"
#include "CppUTest/MemoryLeakAllocator.h"
#include "CppUTest/PlatformSpecificFunctions.h"

extern "C"
{

#include "CppUTest/TestHarness_c.h"

void CHECK_EQUAL_C_INT_LOCATION(int expected, int actual, const char* fileName, int lineNumber)
{
	CHECK_EQUAL_LOCATION((long)expected, (long)actual, fileName, lineNumber);
}

void CHECK_EQUAL_C_REAL_LOCATION(double expected, double actual, double threshold, const char* fileName, int lineNumber)
{
	DOUBLES_EQUAL_LOCATION(expected, actual, threshold, fileName, lineNumber);
}

void CHECK_EQUAL_C_CHAR_LOCATION(char expected, char actual, const char* fileName, int lineNumber)
{
	CHECK_EQUAL_LOCATION(expected, actual, fileName, lineNumber);
}

void CHECK_EQUAL_C_STRING_LOCATION(const char* expected, const char* actual, const char* fileName, int lineNumber)
{
	STRCMP_EQUAL_LOCATION(expected, actual, fileName, lineNumber);
}

void FAIL_TEXT_C_LOCATION(const char* text, const char* fileName, int lineNumber)
{
	FAIL_LOCATION(text, fileName, lineNumber);
}

void FAIL_C_LOCATION(const char* fileName, int lineNumber)
{
	FAIL_LOCATION("", fileName, lineNumber);
}

void CHECK_C_LOCATION(int condition, const char* conditionString, const char* fileName, int lineNumber)
{
	CHECK_LOCATION(((condition) == 0 ? false : true), conditionString, fileName, lineNumber);
}

void cpputest_malloc_set_out_of_memory()
{
	MemoryLeakAllocator::setCurrentMallocAllocator(NullUnknownAllocator::defaultAllocator());
}

void cpputest_malloc_set_not_out_of_memory()
{
	MemoryLeakAllocator::setCurrentMallocAllocatorToDefault();
}

void* cpputest_malloc(size_t size)
{
	return cpputest_malloc_location(size, "<unknown>", 0);
}

void* cpputest_calloc(size_t num, size_t size)
{
	return cpputest_calloc_location(num, size, "<unknown>", 0);
}

void* cpputest_realloc(void* ptr, size_t size)
{
	return cpputest_realloc_location(ptr, size, "<unknown>", 0);
}

void cpputest_free(void* buffer)
{
	cpputest_free_location(buffer, "<unknown>", 0);
}

void* cpputest_malloc_location(size_t size, const char* file, int line)
{
	return MemoryLeakWarningPlugin::getGlobalDetector()->allocMemory(MemoryLeakAllocator::getCurrentMallocAllocator(), size, file, line);
}

void* cpputest_calloc_location(size_t num, size_t size, const char* file, int line)
{
	void* mem = cpputest_malloc_location(num * size, file, line);
	PlatformSpecificMemset(mem, 0, num*size);
	return mem;
}

void* cpputest_realloc_location(void* memory, size_t size, const char* file, int line)
{
	return MemoryLeakWarningPlugin::getGlobalDetector()->reallocMemory(MemoryLeakAllocator::getCurrentMallocAllocator(), (char*) memory, size, file, line);
}

void cpputest_free_location(void* buffer, const char* file, int line)
{
	MemoryLeakWarningPlugin::getGlobalDetector()->deallocMemory(MemoryLeakAllocator::getCurrentMallocAllocator(), (char*) buffer, file, line);
}

}
