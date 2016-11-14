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
#include "CppUTest/TestMemoryAllocator.h"
#include "CppUTest/PlatformSpecificFunctions.h"

TEST_GROUP(TestMemoryAllocatorTest)
{
    TestMemoryAllocator* allocator;

    void setup()
    {
        allocator = NULL;
    }

    void teardown()
    {
        delete allocator;
    }
};

TEST(TestMemoryAllocatorTest, SetCurrentNewAllocator)
{
    allocator = new TestMemoryAllocator("new allocator for test");
    setCurrentNewAllocator(allocator);
    POINTERS_EQUAL(allocator, getCurrentNewAllocator());
    setCurrentNewAllocatorToDefault();
    POINTERS_EQUAL(defaultNewAllocator(), getCurrentNewAllocator());
}

TEST(TestMemoryAllocatorTest, SetCurrentNewArrayAllocator)
{
    allocator = new TestMemoryAllocator("new array allocator for test");
    setCurrentNewArrayAllocator(allocator);
    POINTERS_EQUAL(allocator, getCurrentNewArrayAllocator());
    setCurrentNewArrayAllocatorToDefault();
    POINTERS_EQUAL(defaultNewArrayAllocator(), getCurrentNewArrayAllocator());
}

TEST(TestMemoryAllocatorTest, SetCurrentMallocAllocator)
{
    allocator = new TestMemoryAllocator("malloc_allocator");
    setCurrentMallocAllocator(allocator);
    POINTERS_EQUAL(allocator, getCurrentMallocAllocator());
    setCurrentMallocAllocatorToDefault();
    POINTERS_EQUAL(defaultMallocAllocator(), getCurrentMallocAllocator());
}

TEST(TestMemoryAllocatorTest, MemoryAllocation)
{
    allocator = new TestMemoryAllocator();
    allocator->free_memory(allocator->alloc_memory(100, "file", 1), "file", 1);
}

TEST(TestMemoryAllocatorTest, MallocNames)
{
    STRCMP_EQUAL("Standard Malloc Allocator", defaultMallocAllocator()->name());
    STRCMP_EQUAL("malloc", defaultMallocAllocator()->alloc_name());
    STRCMP_EQUAL("free", defaultMallocAllocator()->free_name());
}

TEST(TestMemoryAllocatorTest, NewNames)
{
    STRCMP_EQUAL("Standard New Allocator", defaultNewAllocator()->name());
    STRCMP_EQUAL("new", defaultNewAllocator()->alloc_name());
    STRCMP_EQUAL("delete", defaultNewAllocator()->free_name());
}

TEST(TestMemoryAllocatorTest, NewArrayNames)
{
    STRCMP_EQUAL("Standard New [] Allocator", defaultNewArrayAllocator()->name());
    STRCMP_EQUAL("new []", defaultNewArrayAllocator()->alloc_name());
    STRCMP_EQUAL("delete []", defaultNewArrayAllocator()->free_name());
}

TEST(TestMemoryAllocatorTest, NullUnknownAllocation)
{
    allocator = new NullUnknownAllocator;
    allocator->free_memory(allocator->alloc_memory(100, "file", 1), "file", 1);
}

TEST(TestMemoryAllocatorTest, NullUnknownNames)
{
    allocator = new NullUnknownAllocator;
    STRCMP_EQUAL("Null Allocator", allocator->name());
    STRCMP_EQUAL("unknown", allocator->alloc_name());
    STRCMP_EQUAL("unknown", allocator->free_name());
}

#define MAX_SIZE_FOR_ALLOC ((size_t) -1 > (unsigned short)-1) ? (size_t) -97 : (size_t) -1

static void failTryingToAllocateTooMuchMemory(void)
{
    TestMemoryAllocator allocator;
    allocator.alloc_memory(MAX_SIZE_FOR_ALLOC, "file", 1);
} // LCOV_EXCL_LINE

#include "CppUTest/TestTestingFixture.h"

TEST(TestMemoryAllocatorTest, TryingToAllocateTooMuchFailsTest)
{
    TestTestingFixture fixture;
    fixture.setTestFunction(&failTryingToAllocateTooMuchMemory);
    fixture.runAllTests();
    fixture.assertPrintContains("malloc returned null pointer");
}

#if CPPUTEST_USE_MEM_LEAK_DETECTION
#if CPPUTEST_USE_MALLOC_MACROS

// FailableMemoryAllocator must be global. Otherwise, it does not exist when memory leak detector
// reports memory leaks.
static FailableMemoryAllocator failableMallocAllocator("Failable Malloc Allocator", "malloc", "free");


TEST_GROUP(FailableMemoryAllocator)
{
    TestTestingFixture *fixture;
    void setup()
    {
        fixture = new TestTestingFixture;
        failableMallocAllocator.clearFailedAllocs();
        setCurrentMallocAllocator(&failableMallocAllocator);
    }
    void teardown()
    {
        failableMallocAllocator.checkAllFailedAllocsWereDone();
        setCurrentMallocAllocatorToDefault();
        delete fixture;
    }
};

TEST(FailableMemoryAllocator, MallocWorksNormallyIfNotAskedToFail)
{
    int *memory = (int*)malloc(sizeof(int));
    CHECK(memory != NULL);
    free(memory);
}

TEST(FailableMemoryAllocator, FailFirstMalloc)
{
    failableMallocAllocator.failAllocNumber(1);
    POINTERS_EQUAL(NULL, (int*)malloc(sizeof(int)));
}

TEST(FailableMemoryAllocator, FailSecondAndFourthMalloc)
{
    failableMallocAllocator.failAllocNumber(2);
    failableMallocAllocator.failAllocNumber(4);
    int *memory1 = (int*)malloc(sizeof(int));
    int *memory2 = (int*)malloc(sizeof(int));
    int *memory3 = (int*)malloc(sizeof(int));
    int *memory4 = (int*)malloc(sizeof(int));

    CHECK(NULL != memory1);
    POINTERS_EQUAL(NULL, memory2);
    CHECK(NULL != memory3);
    POINTERS_EQUAL(NULL, memory4);

    free(memory1);
    free(memory3);
}

static void _failingAllocIsNeverDone()
{
    failableMallocAllocator.failAllocNumber(1);
    failableMallocAllocator.failAllocNumber(2);
    failableMallocAllocator.failAllocNumber(3);
    malloc(sizeof(int));
    malloc(sizeof(int));
    failableMallocAllocator.checkAllFailedAllocsWereDone();
}

TEST(FailableMemoryAllocator, CheckAllFailingAllocsWereDone)
{
    fixture->setTestFunction(_failingAllocIsNeverDone);

    fixture->runAllTests();

    LONGS_EQUAL(1, fixture->getFailureCount());
    fixture->assertPrintContains("Expected allocation number 3 was never done");
    failableMallocAllocator.clearFailedAllocs();
}

TEST(FailableMemoryAllocator, FailFirstAllocationAtGivenLine)
{
    failableMallocAllocator.failNthAllocAt(1, __FILE__, __LINE__ + 2);

    POINTERS_EQUAL(NULL, malloc(sizeof(int)));
}

TEST(FailableMemoryAllocator, FailThirdAllocationAtGivenLine)
{
    int *memory[10];
    int allocation;
    failableMallocAllocator.failNthAllocAt(3, __FILE__, __LINE__ + 4);

    for (allocation = 1; allocation <= 10; allocation++)
    {
        memory[allocation - 1] = (int *)malloc(sizeof(int));
        if (memory[allocation - 1] == NULL)
            break;
    }

    LONGS_EQUAL(3, allocation);
    free(memory[0]); free(memory[1]);
}

static void _failingLocationAllocIsNeverDone()
{
    failableMallocAllocator.failNthAllocAt(1, "TestMemoryAllocatorTest.cpp", __LINE__);
    failableMallocAllocator.checkAllFailedAllocsWereDone();
}

TEST(FailableMemoryAllocator, CheckAllFailingLocationAllocsWereDone)
{
    fixture->setTestFunction(_failingLocationAllocIsNeverDone);

    fixture->runAllTests();

    LONGS_EQUAL(1, fixture->getFailureCount());
    fixture->assertPrintContains("Expected failing alloc at TestMemoryAllocatorTest.cpp:");
    fixture->assertPrintContains("was never done");
    failableMallocAllocator.clearFailedAllocs();
}

#endif
#endif
