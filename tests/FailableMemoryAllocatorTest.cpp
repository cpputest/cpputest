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
#include "CppUTest/FailableMemoryAllocator.h"
#include "CppUTest/TestTestingFixture.h"


// Allocator must be global. Otherwise, it does not exist when memory leak detector
// reports memory leaks.
static FailableMemoryAllocator failableMallocAllocator("Failable Malloc Allocator", "malloc", "free");
static FailableMemoryAllocator failableNewAllocator("Failable New Allocator", "new", "delete");
static FailableMemoryAllocator failableNewArrayAllocator("Failable New [] Allocator", "new []", "delete []");


#if CPPUTEST_USE_MEM_LEAK_DETECTION

TEST_GROUP(FailableMemoryAllocator)
{
    void setup()
    {
        failableMallocAllocator.clearFailedAllocations();
        failableNewAllocator.clearFailedAllocations();
        failableNewArrayAllocator.clearFailedAllocations();

        setCurrentMallocAllocator(&failableMallocAllocator);
        setCurrentNewAllocator(&failableNewAllocator);
        setCurrentNewArrayAllocator(&failableNewArrayAllocator);
    }
    void teardown()
    {
        setCurrentMallocAllocatorToDefault();
        setCurrentNewAllocatorToDefault();
        setCurrentNewArrayAllocatorToDefault();
    }
};

#if CPPUTEST_USE_MALLOC_MACROS
TEST(FailableMemoryAllocator, MallocWorksNormallyIfNotAskedToFail)
{
    int *memory = (int*)malloc(sizeof(int));
    *memory = 1;
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

static void _setUpTooManyFailedMallocs()
{
    FailableMemoryAllocator allocator;
    for (int i = 0; i <= allocator.MAX_NUMBER_OF_FAILED_ALLOCS; i++)
        allocator.failAllocNumber(i + 1);
}

TEST(FailableMemoryAllocator, SettingUpTooManyFailedAllocsWillFail)
{
    TestTestingFixture fixture;
    fixture.setTestFunction(_setUpTooManyFailedMallocs);

    fixture.runAllTests();

    LONGS_EQUAL(1, fixture.getFailureCount());
    fixture.assertPrintContains("Maximum number of failed memory allocations exceeded");
}
#endif

TEST(FailableMemoryAllocator, NewWorksNormallyIfNotAskedToFail)
{
    int *memory = new int;
    *memory = 1;
    CHECK(memory != NULL);
    delete memory;
}

TEST(FailableMemoryAllocator, NewArrayWorksNormallyIfNotAskedToFail)
{
    int *memory = new int[10];
    memory[0] = 1;
    CHECK(memory != NULL);
    delete [] memory;
}

#if CPPUTEST_USE_STD_CPP_LIB

TEST(FailableMemoryAllocator, FailSecondNewRaisesException)
{
    failableNewAllocator.failAllocNumber(2);
    int *memory1 = new int;

    CHECK_THROWS(std::bad_alloc, new int);

    delete memory1;
}

TEST(FailableMemoryAllocator, FailSecondNewArrayRaisesException)
{
    failableNewArrayAllocator.failAllocNumber(2);
    int *memory1 = new int[10];

    CHECK_THROWS(std::bad_alloc, new int[10]);

    delete [] memory1;
}

#endif

#ifdef __clang__
IGNORE_TEST(FailableMemoryAllocator, FailSecondAndFourthNewNoThrow)
{
    /*  Clang misbehaves with -O2 - it will not overload operator new or
     *  operator new[] no matter what. Therefore, this test is must be ignored.
     */
}
#else

#if CPPUTEST_USE_STD_CPP_LIB
#define STD_NOTHROW (std::nothrow)
#else
#define STD_NOTHROW
#endif

TEST(FailableMemoryAllocator, FailSecondAndFourthNewNoThrow)
{
#undef new
    failableNewAllocator.failAllocNumber(2);
    failableNewAllocator.failAllocNumber(4);
    int *memory1 = new STD_NOTHROW int;
    int *memory2 = new STD_NOTHROW int;
    int *memory3 = new STD_NOTHROW int;
    int *memory4 = new STD_NOTHROW int;

    CHECK(NULL != memory1);
    POINTERS_EQUAL(NULL, memory2);
    CHECK(NULL != memory3);
    POINTERS_EQUAL(NULL, memory4);

    delete memory1;
    delete memory3;
}
#endif

#ifdef __clang__
IGNORE_TEST(FailableMemoryAllocator, FailSecondAndFourthNewArrayNoThrow)
{
    /*  Clang misbehaves with -O2 - it will not overload operator new or
     *  operator new[] no matter what. Therefore, this test is must be ignored.
     */
}
#else

TEST(FailableMemoryAllocator, FailSecondAndFourthNewArrayNoThrow)
{
    failableNewArrayAllocator.failAllocNumber(2);
    failableNewArrayAllocator.failAllocNumber(4);
    int *memory1 = new STD_NOTHROW int[10];
    int *memory2 = new STD_NOTHROW int[10];
    int *memory3 = new STD_NOTHROW int[10];
    int *memory4 = new STD_NOTHROW int[10];

    CHECK(NULL != memory1);
    POINTERS_EQUAL(NULL, memory2);
    CHECK(NULL != memory3);
    POINTERS_EQUAL(NULL, memory4);

    delete [] memory1;
    delete [] memory3;
}
#endif

#endif
