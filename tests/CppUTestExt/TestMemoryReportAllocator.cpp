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
#include "CppUTest/TestOutput.h"
#include "CppUTestExt/MemoryReportAllocator.h"
#include "CppUTestExt/MemoryReportFormatter.h"

void defaultFree(char* memory)
{
	StandardMallocAllocator::defaultAllocator()->free_memory(memory, __FILE__, __LINE__);
}

char* defaultAlloc(size_t size)
{
	return StandardMallocAllocator::defaultAllocator()->alloc_memory(size	, __FILE__, __LINE__);
}

TEST_GROUP(MemoryReportAllocator)
{
	StringBufferTestOutput testOutput;
	TestResult* testResult;
	MemoryReportAllocator* stdCAllocator;
	NormalMemoryReportFormatter formatter;

	void setup()
	{
		stdCAllocator = new MemoryReportAllocator;
		stdCAllocator->setFormatter(&formatter);
		testResult = new TestResult(testOutput);
		stdCAllocator->setRealAllocator(StandardMallocAllocator::defaultAllocator());
		stdCAllocator->setTestResult(testResult);
	}

	void teardown()
	{
		delete stdCAllocator;
		delete testResult;
	}
};

TEST(MemoryReportAllocator, NoAllocationResultsInAnEmptyString)
{
	STRCMP_EQUAL("", testOutput.getOutput().asCharString());
}

TEST(MemoryReportAllocator, MallocAllocationLeadsToPrintout)
{
	char* memory = stdCAllocator->alloc_memory(10, "file", 9);
	STRCMP_EQUAL(StringFromFormat("Allocation using malloc of size: 10 pointer: %p at file:9\n", memory).asCharString(), testOutput.getOutput().asCharString());
	defaultFree(memory);
}

TEST(MemoryReportAllocator, FreeAllocationLeadsToPrintout)
{
	char* memory = defaultAlloc(10);
	stdCAllocator->free_memory(memory, "file", 9);
	STRCMP_EQUAL(StringFromFormat("Deallocation using free of pointer: %p at file:9\n", memory).asCharString(), testOutput.getOutput().asCharString());
}
