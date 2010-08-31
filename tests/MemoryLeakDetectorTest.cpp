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
#include "CppUTest/MemoryLeakDetector.h"
#include "CppUTest/MemoryLeakAllocator.h"
#include "CppUTest/PlatformSpecificFunctions.h"

class MemoryLeakFailureForTest: public MemoryLeakFailure
{
public:
	virtual ~MemoryLeakFailureForTest()
	{
	}
	;
	virtual void fail(char* fail_string)
	{
		*message = fail_string;
	}

	SimpleString *message;
};

class NewAllocatorForMemoryLeakDetectionTest: public StandardNewAllocator
{
public:
	NewAllocatorForMemoryLeakDetectionTest() :
		alloc_called(0), free_called(0)
	{
	}

	int alloc_called;
	int free_called;
	char* alloc_memory(size_t size, const char*, int)
	{
		alloc_called++;
		return StandardNewAllocator::alloc_memory(size, "file", 1);
	}
	void free_memory(char* memory, const char* file, int line)
	{
		free_called++;
		StandardNewAllocator::free_memory(memory, file, line);
	}
};

class MallocAllocatorForMemoryLeakDetectionTest: public StandardMallocAllocator
{
public:
	MallocAllocatorForMemoryLeakDetectionTest() :
		alloc_called(0), free_called(0), allocMemoryLeakNodeCalled(0), freeMemoryLeakNodeCalled(0)
	{
	}

	int alloc_called;
	int free_called;
	int allocMemoryLeakNodeCalled;
	int freeMemoryLeakNodeCalled;
	char* alloc_memory(size_t size, const char* file, int line)
	{
		alloc_called++;
		return StandardMallocAllocator::alloc_memory(size, file, line);
	}
	void free_memory(char* memory, const char* file, int line)
	{
		free_called++;
		StandardMallocAllocator::free_memory(memory, file, line);
	}

	char* allocMemoryLeakNode(size_t size)
	{
		allocMemoryLeakNodeCalled++;
		return StandardMallocAllocator::alloc_memory(size, __FILE__, __LINE__);
	}

	void freeMemoryLeakNode(char* memory)
	{
		freeMemoryLeakNodeCalled++;
		StandardMallocAllocator::free_memory(memory, __FILE__, __LINE__);
	}
};

TEST_GROUP(MemoryLeakDetectorTest)
{
	MemoryLeakDetector* detector;
	MemoryLeakFailureForTest *reporter;
	MallocAllocatorForMemoryLeakDetectionTest* mallocAllocator;
	NewAllocatorForMemoryLeakDetectionTest* newAllocator;
	StandardNewArrayAllocator* newArrayAllocator;

	void setup()
	{
		detector = new MemoryLeakDetector;
		reporter = new MemoryLeakFailureForTest;
		mallocAllocator = new MallocAllocatorForMemoryLeakDetectionTest;
		newAllocator = new NewAllocatorForMemoryLeakDetectionTest;
		newArrayAllocator = new StandardNewArrayAllocator;
		detector->init(reporter);
		detector->enable();
		detector->startChecking();
		reporter->message = new SimpleString();
	}
	void teardown()
	{
		delete reporter->message;
		delete detector;
		delete reporter;
		delete mallocAllocator;
		delete newAllocator;
		delete newArrayAllocator;
	}
};

TEST(MemoryLeakDetectorTest, OneLeak)
{
	char* mem = detector->allocMemory(newAllocator, 3);
	detector->stopChecking();
	SimpleString output = detector->report(mem_leak_period_checking);
	CHECK(output.contains(MEM_LEAK_HEADER));
	CHECK(output.contains("size: 3"));
	CHECK(output.contains("new"));
	CHECK(output.contains(MEM_LEAK_FOOTER));
	PlatformSpecificFree(mem);
	LONGS_EQUAL(1, newAllocator->alloc_called);
	LONGS_EQUAL(0, newAllocator->free_called);
}

#include <stdio.h>

TEST(MemoryLeakDetectorTest, OneHundredLeaks)
{
	const int amount_alloc = 100;
	char *mem[amount_alloc];
	for (int i = 0; i < amount_alloc; i++)
		mem[i] = detector->allocMemory(mallocAllocator, 3);
	detector->stopChecking();

	SimpleString output = detector->report(mem_leak_period_checking);

	STRCMP_CONTAINS(MEM_LEAK_HEADER, output.asCharString());
	STRCMP_CONTAINS(MEM_LEAK_FOOTER, output.asCharString());
	STRCMP_CONTAINS(MEM_LEAK_ADDITION_MALLOC_WARNING, output.asCharString());

	//don't reuse i for vc6 compatibility
	for (int j = 0; j < amount_alloc; j++)
		PlatformSpecificFree(mem[j]);
}

TEST(MemoryLeakDetectorTest, OneLeakOutsideCheckingPeriod)
{
	detector->stopChecking();
	char* mem = detector->allocMemory(newAllocator, 4);
	SimpleString output = detector->report(mem_leak_period_all);
	CHECK(output.contains(MEM_LEAK_HEADER));
	CHECK(output.contains("size: 4"));
	CHECK(output.contains("new"));
	CHECK(output.contains(MEM_LEAK_FOOTER));
	PlatformSpecificFree(mem);
}

TEST(MemoryLeakDetectorTest, NoLeaksWhatsoever)
{
	detector->stopChecking();
	STRCMP_EQUAL(MEM_LEAK_NONE, detector->report(mem_leak_period_checking));
	STRCMP_EQUAL(MEM_LEAK_NONE, detector->report(mem_leak_period_all));
}

TEST(MemoryLeakDetectorTest, TwoLeaksUsingOperatorNew)
{
	char* mem = detector->allocMemory(newAllocator, 4);
	char* mem2 = detector->allocMemory(newAllocator, 8);
	detector->stopChecking();
	SimpleString output = detector->report(mem_leak_period_checking);
	LONGS_EQUAL(2, detector->totalMemoryLeaks(mem_leak_period_checking));
	CHECK(output.contains("size: 8"));
	CHECK(output.contains("size: 4"));
	PlatformSpecificFree(mem);
	PlatformSpecificFree(mem2);
}

TEST(MemoryLeakDetectorTest, OneAllocButNoLeak)
{
	char* mem = detector->allocMemory(newAllocator, 4);
	detector->deallocMemory(newAllocator, mem);
	detector->stopChecking();
	STRCMP_EQUAL(MEM_LEAK_NONE, detector->report(mem_leak_period_checking));
	LONGS_EQUAL(1, newAllocator->alloc_called);
	LONGS_EQUAL(1, newAllocator->free_called);
}

TEST(MemoryLeakDetectorTest, TwoAllocOneFreeOneLeak)
{
	char* mem = detector->allocMemory(newAllocator, 4);
	char* mem2 = detector->allocMemory(newAllocator, 12);
	detector->deallocMemory(newAllocator, mem);
	detector->stopChecking();
	SimpleString output = detector->report(mem_leak_period_checking);
	LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_checking));
	CHECK(output.contains("size: 12"));
	CHECK(!output.contains("size: 4"));
	PlatformSpecificFree(mem2);
	LONGS_EQUAL(2, newAllocator->alloc_called);
	LONGS_EQUAL(1, newAllocator->free_called);
}

TEST(MemoryLeakDetectorTest, TwoAllocOneFreeOneLeakReverseOrder)
{
	char* mem = detector->allocMemory(newAllocator, 4);
	char* mem2 = detector->allocMemory(newAllocator, 12);
	detector->deallocMemory(newAllocator, mem2);
	detector->stopChecking();
	SimpleString output = detector->report(mem_leak_period_checking);
	LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_checking));
	CHECK(!output.contains("size: 12"));
	CHECK(output.contains("size: 4"));
	PlatformSpecificFree(mem);
}

TEST(MemoryLeakDetectorTest, DeleteNonAlocatedMemory)
{
	char a;
	char* pa = &a;
	detector->deallocMemory(mallocAllocator, pa, "FREE.c", 100);
	detector->stopChecking();
	CHECK(reporter->message->contains(MEM_LEAK_DEALLOC_NON_ALLOCATED));
	CHECK(reporter->message->contains("   allocated at file: <unknown> line: 0 size: 0 type: unknown"));
	CHECK(reporter->message->contains("   deallocated at file: FREE.c line: 100 type: free"));
	LONGS_EQUAL(0, newAllocator->free_called);
}

TEST(MemoryLeakDetectorTest, IgnoreMemoryAllocatedOutsideCheckingPeriod)
{
	detector->stopChecking();
	char* mem = detector->allocMemory(newAllocator, 4);
	LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_checking));
	LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_all));
	detector->deallocMemory(newAllocator, mem);
}

TEST(MemoryLeakDetectorTest, IgnoreMemoryAllocatedOutsideCheckingPeriodComplicatedCase)
{
	char* mem = detector->allocMemory(newAllocator, 4);
	detector->stopChecking();

	char* mem2 = detector->allocMemory(newAllocator, 8);
	LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_checking));
	detector->clearAllAccounting(mem_leak_period_checking);
	PlatformSpecificFree(mem);
	LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_checking));
	LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_all));

	detector->startChecking();
	char* mem3 = detector->allocMemory(newAllocator, 4);
	detector->stopChecking();
	LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_checking));
	LONGS_EQUAL(2, detector->totalMemoryLeaks(mem_leak_period_all));

	detector->clearAllAccounting(mem_leak_period_checking);
	LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_checking));
	LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_all));

	detector->clearAllAccounting(mem_leak_period_all);
	LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_all));
	PlatformSpecificFree(mem2);
	PlatformSpecificFree(mem3);
}

TEST(MemoryLeakDetectorTest, OneLeakUsingOperatorNewWithFileLine)
{
	char* mem = detector->allocMemory(newAllocator, 4, "file.cpp", 1234);
	detector->stopChecking();
	SimpleString output = detector->report(mem_leak_period_checking);
	CHECK(output.contains("file.cpp"));
	CHECK(output.contains("1234"));
	PlatformSpecificFree(mem);
}

TEST(MemoryLeakDetectorTest, OneAllocAndFreeUsingArrayNew)
{
	char* mem = detector->allocMemory(newArrayAllocator, 10, "file.cpp", 1234);
	char* mem2 = detector->allocMemory(newArrayAllocator, 12);
	LONGS_EQUAL(2, detector->totalMemoryLeaks(mem_leak_period_all));
	SimpleString output = detector->report(mem_leak_period_checking);
	CHECK(output.contains("new []"));
	detector->deallocMemory(newArrayAllocator, mem);
	detector->deallocMemory(newArrayAllocator, mem2);
	LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_all));
	detector->stopChecking();
}

TEST(MemoryLeakDetectorTest, OneAllocAndFree)
{
	char* mem = detector->allocMemory(mallocAllocator, 10, "file.cpp", 1234);
	char* mem2 = detector->allocMemory(mallocAllocator, 12);
	LONGS_EQUAL(2, detector->totalMemoryLeaks(mem_leak_period_checking));
	SimpleString output = detector->report(mem_leak_period_checking);
	CHECK(output.contains("malloc"));
	detector->deallocMemory(mallocAllocator, mem);
	detector->deallocMemory(mallocAllocator, mem2, "file.c", 5678);
	LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_all));
	detector->stopChecking();
}

TEST(MemoryLeakDetectorTest, OneRealloc)
{
	char* mem1 = detector->allocMemory(mallocAllocator, 10, "file.cpp", 1234);

	char* mem2 = detector->reallocMemory(mallocAllocator, mem1, 1000, "other.cpp", 5678);

	LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_checking));
	SimpleString output = detector->report(mem_leak_period_checking);
	CHECK(output.contains("other.cpp"));

	detector->deallocMemory(mallocAllocator, mem2);
	LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_all));
	detector->stopChecking();
	LONGS_EQUAL(1, mallocAllocator->alloc_called);
	LONGS_EQUAL(1, mallocAllocator->free_called);
	LONGS_EQUAL(2, mallocAllocator->allocMemoryLeakNodeCalled);
	LONGS_EQUAL(2, mallocAllocator->freeMemoryLeakNodeCalled);
}


TEST(MemoryLeakDetectorTest, AllocAndFreeWithDifferenceInstancesOfTheSameAllocatorType)
{
	StandardNewArrayAllocator newArrayAllocatorTwo;
	char* mem = detector->allocMemory(newArrayAllocator, 100, "ALLOC.c", 10);
	detector->deallocMemory(&newArrayAllocatorTwo, mem, "FREE.c", 100);
	detector->stopChecking();
	STRCMP_EQUAL("", reporter->message->asCharString());
}

TEST(MemoryLeakDetectorTest, AllocOneTypeFreeAnotherType)
{
	char* mem = detector->allocMemory(newArrayAllocator, 100, "ALLOC.c", 10);
	detector->deallocMemory(mallocAllocator, mem, "FREE.c", 100);
	detector->stopChecking();
	CHECK(reporter->message->contains(MEM_LEAK_ALLOC_DEALLOC_MISMATCH));
	CHECK(reporter->message->contains("   allocated at file: ALLOC.c line: 10 size: 100 type: new []"));
	CHECK(reporter->message->contains("   deallocated at file: FREE.c line: 100 type: free"));
}

TEST(MemoryLeakDetectorTest, AllocOneTypeFreeAnotherTypeWithCheckingDisabled)
{
	detector->disableAllocationTypeChecking();
	char* mem = detector->allocMemory(newArrayAllocator, 100, "ALLOC.c", 10);
	detector->deallocMemory(newArrayAllocator, mem, "FREE.c", 100);
	detector->stopChecking();
	STRCMP_EQUAL("", reporter->message->asCharString());
	detector->enableAllocationTypeChecking();
}

TEST(MemoryLeakDetectorTest, mallocLeakGivesAdditionalWarning)
{
	char* mem = detector->allocMemory(mallocAllocator, 100, "ALLOC.c", 10);
	detector->stopChecking();
	SimpleString output = detector->report(mem_leak_period_checking);
	STRCMP_CONTAINS("Memory leak reports about malloc and free can be caused by allocating using the cpputest version of malloc", output.asCharString());
	PlatformSpecificFree(mem);
}

TEST(MemoryLeakDetectorTest, newLeakDoesNotGiveAdditionalWarning)
{
	char* mem = detector->allocMemory(newAllocator, 100, "ALLOC.c", 10);
	detector->stopChecking();
	SimpleString output = detector->report(mem_leak_period_checking);
	CHECK(! output.contains("Memory leak reports about malloc and free"));
	PlatformSpecificFree(mem);
}

TEST(MemoryLeakDetectorTest, MarkCheckingPeriodLeaksAsNonCheckingPeriod)
{
	char* mem = detector->allocMemory(newArrayAllocator, 100);
	char* mem2 = detector->allocMemory(newArrayAllocator, 100);
	detector->stopChecking();
	LONGS_EQUAL(2, detector->totalMemoryLeaks(mem_leak_period_checking));
	LONGS_EQUAL(2, detector->totalMemoryLeaks(mem_leak_period_all));
	detector->markCheckingPeriodLeaksAsNonCheckingPeriod();
	LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_checking));
	LONGS_EQUAL(2, detector->totalMemoryLeaks(mem_leak_period_all));
	PlatformSpecificFree(mem);
	PlatformSpecificFree(mem2);
}

TEST(MemoryLeakDetectorTest, memoryCorruption)
{
	char* mem = detector->allocMemory(mallocAllocator, 10, "ALLOC.c", 10);
	mem[10] = 'O';
	mem[11] = 'H';
	detector->deallocMemory(mallocAllocator, mem, "FREE.c", 100);
	detector->stopChecking();
	CHECK(reporter->message->contains(MEM_LEAK_MEMORY_CORRUPTION));
	CHECK(reporter->message->contains("   allocated at file: ALLOC.c line: 10 size: 10 type: malloc"));
	CHECK(reporter->message->contains("   deallocated at file: FREE.c line: 100 type: free"));
}

TEST(MemoryLeakDetectorTest, safelyDeleteNULL)
{
	detector->deallocMemory(newAllocator, 0);
	STRCMP_EQUAL("", reporter->message->asCharString());
}

TEST(MemoryLeakDetectorTest, periodDisabled)
{
	detector->disable();
	char* mem = detector->allocMemory(mallocAllocator, 2);
	LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_all));
	LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_disabled));
	LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_enabled));
	LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_checking));
	detector->deallocMemory(mallocAllocator, mem);
}

TEST(MemoryLeakDetectorTest, periodEnabled)
{
	detector->enable();
	char* mem = detector->allocMemory(mallocAllocator, 2);
	LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_all));
	LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_disabled));
	LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_enabled));
	LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_checking));
	detector->deallocMemory(mallocAllocator, mem);
}

TEST(MemoryLeakDetectorTest, periodChecking)
{
	char* mem = detector->allocMemory(mallocAllocator, 2);
	LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_all));
	LONGS_EQUAL(0, detector->totalMemoryLeaks(mem_leak_period_disabled));
	LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_enabled));
	LONGS_EQUAL(1, detector->totalMemoryLeaks(mem_leak_period_checking));
	detector->deallocMemory(mallocAllocator, mem);
}

TEST(MemoryLeakDetectorTest, allocateWithANullAllocatorCausesNoProblems)
{
	char* mem = detector->allocMemory(NullUnknownAllocator::defaultAllocator(), 2);
	detector->deallocMemory(NullUnknownAllocator::defaultAllocator(), mem);
}


TEST_GROUP(SimpleStringBuffer)
{
};

TEST(SimpleStringBuffer, simpleTest)
{
	SimpleStringBuffer buffer;
	buffer.add("Hello");
	buffer.add(" World");
	STRCMP_EQUAL("Hello World", buffer.toString());
}

TEST(SimpleStringBuffer, writePastLimit)
{
	SimpleStringBuffer buffer;
	for (int i = 0; i < SimpleStringBuffer::SIMPLE_STRING_BUFFER_LEN * 2; i++)
		buffer.add("h");
	SimpleString str("h", SimpleStringBuffer::SIMPLE_STRING_BUFFER_LEN-1);
	STRCMP_EQUAL(str.asCharString(), buffer.toString());
}

TEST(SimpleStringBuffer, setWriteLimit)
{
	SimpleStringBuffer buffer;
	buffer.setWriteLimit(10);
	for (int i = 0; i < SimpleStringBuffer::SIMPLE_STRING_BUFFER_LEN ; i++)
		buffer.add("h");
	SimpleString str("h", 10);
	STRCMP_EQUAL(str.asCharString(), buffer.toString());
}

TEST(SimpleStringBuffer, setWriteLimitTooHighIsIgnored)
{
	SimpleStringBuffer buffer;
	buffer.setWriteLimit(SimpleStringBuffer::SIMPLE_STRING_BUFFER_LEN+10);
	for (int i = 0; i < SimpleStringBuffer::SIMPLE_STRING_BUFFER_LEN+10; i++)
		buffer.add("h");
	SimpleString str("h", SimpleStringBuffer::SIMPLE_STRING_BUFFER_LEN-1);
	STRCMP_EQUAL(str.asCharString(), buffer.toString());
}

TEST(SimpleStringBuffer, resetWriteLimit)
{
	SimpleStringBuffer buffer;
	buffer.setWriteLimit(10);
	for (int i = 0; i < SimpleStringBuffer::SIMPLE_STRING_BUFFER_LEN ; i++)
		buffer.add("h");
	buffer.resetWriteLimit();
	buffer.add(SimpleString("h", 10).asCharString());

	SimpleString str("h", 20);
	STRCMP_EQUAL(str.asCharString(), buffer.toString());
}

TEST_GROUP(ReallocBugReported) { };

TEST(ReallocBugReported, ThisSituationShouldntCrash)
{
	StandardMallocAllocator allocator;
	MemoryLeakDetector detector;
	char* mem = detector.allocMemory(&allocator, 5, "file", 1);
	mem = detector.reallocMemory(&allocator, mem, 19, "file", 1);
	detector.deallocMemory(&allocator, mem);
}
