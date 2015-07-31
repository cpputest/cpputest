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


TEST_GROUP(FailableMemoryAllocator)
{
    FailableMallocAllocator *failableMallocAllocator;
    TestTestingFixture *fixture;

    void setup()
    {
        failableMallocAllocator = new FailableMallocAllocator("Failable malloc");
        fixture = new TestTestingFixture;
        setCurrentMallocAllocator(failableMallocAllocator);
    }
    void teardown()
    {
        setCurrentMallocAllocatorToDefault();
        delete failableMallocAllocator;
        delete fixture;
    }
};

TEST(FailableMemoryAllocator, MallocWorksNormallyIfNotAskedToFail)
{
    int *memory = (int*)malloc(sizeof(int));
    *memory = 1;
    CHECK(memory != NULL);
    free(memory); // Try commenting this out
}

TEST(FailableMemoryAllocator, FailFirstMalloc)
{
    failableMallocAllocator->failMallocNumber(1);
    LONGS_EQUAL(NULL, (int*)malloc(sizeof(int)));
}

TEST(FailableMemoryAllocator, FailSecondAndFourthMalloc)
{
    failableMallocAllocator->failMallocNumber(2);
    failableMallocAllocator->failMallocNumber(4);
    int *memory1 = (int*)malloc(sizeof(int));
    int *memory2 = (int*)malloc(sizeof(int));
    int *memory3 = (int*)malloc(sizeof(int));
    int *memory4 = (int*)malloc(sizeof(int));

    CHECK(NULL != memory1);
    LONGS_EQUAL(NULL, memory2);
    CHECK(NULL != memory3);
    LONGS_EQUAL(NULL, memory4);

    free(memory1);
    free(memory3);
}

static void setUpTooManyFailedMallocs()
{
    FailableMallocAllocator allocator;
    for (int i = 0; i <= allocator.MAX_NUMBER_OF_FAILED_ALLOCS; i++)
        allocator.failMallocNumber(i + 1);
}

TEST(FailableMemoryAllocator, SettingUpTooManyFailedAllocsWillFail)
{
    fixture->setTestFunction(setUpTooManyFailedMallocs);
    fixture->runAllTests();
    LONGS_EQUAL(1, fixture->getFailureCount());
    fixture->assertPrintContains("Maximum number of failed memory allocations exceeded");
}
