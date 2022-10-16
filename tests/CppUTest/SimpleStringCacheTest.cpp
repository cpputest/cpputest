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
#include "CppUTest/SimpleStringInternalCache.h"
#include "CppUTest/TestTestingFixture.h"

class TestFunctionWithCache : public ExecFunction
{
public:
    void (*testFunction)(SimpleStringInternalCache*, size_t);
    SimpleStringInternalCache* parameter;
    size_t allocationSize;

    void exec() _override
    {
        testFunction(parameter, allocationSize);
    }
};

TEST_GROUP(SimpleStringInternalCache)
{
    SimpleStringInternalCache cache;
    MemoryAccountant accountant;
    MemoryLeakAllocator* defaultAllocator;
    AccountingTestMemoryAllocator* allocator;

    TestFunctionWithCache testFunction;
    TestTestingFixture fixture;

    void setup() _override
    {
        fixture.setTestFunction(&testFunction);
        testFunction.parameter = &cache;

        defaultAllocator = new MemoryLeakAllocator(defaultMallocAllocator());
        allocator = new AccountingTestMemoryAllocator(accountant, defaultAllocator);
        cache.setAllocator(defaultAllocator);
    }

    void teardown() _override
    {
        cache.clearAllIncludingCurrentlyUsedMemory();
        accountant.clear();
        delete allocator;
        delete defaultAllocator;
    }

    void createCacheForSize(size_t size, size_t amount)
    {
        for (size_t i = 0; i < amount; i++) {
            char* memory = cache.alloc(size);
            cache.dealloc(memory, size);
        }
    }
};

TEST(SimpleStringInternalCache, cacheHitWithOneEntry)
{
    createCacheForSize(10, 1);
    cache.setAllocator(allocator);

    char* mem = cache.alloc(10);
    mem[0] = 'B';
    mem[3] = 'A';
    mem[9] = 'S';

    cache.setAllocator(allocator->originalAllocator());

    LONGS_EQUAL(0, accountant.totalAllocationsOfSize(10));
    CHECK(!cache.hasFreeBlocksOfSize(10));

    cache.setAllocator(allocator);
}

TEST(SimpleStringInternalCache, cacheHitWithTwoEntries)
{
    createCacheForSize(10, 2);
    cache.setAllocator(allocator);

    cache.alloc(10);
    cache.alloc(10);

    cache.setAllocator(allocator->originalAllocator());

    LONGS_EQUAL(0, accountant.totalAllocationsOfSize(10));
    CHECK(!cache.hasFreeBlocksOfSize(10));

    cache.setAllocator(allocator);
}

TEST(SimpleStringInternalCache, allocatingMoreThanCacheAvailable)
{
    createCacheForSize(10, 1);
    cache.setAllocator(allocator);

    cache.alloc(10);
    cache.alloc(10);

    cache.setAllocator(allocator->originalAllocator());

    LONGS_EQUAL(1, accountant.totalAllocationsOfSize(32));
    CHECK(!cache.hasFreeBlocksOfSize(10));

    cache.setAllocator(allocator);
}


TEST(SimpleStringInternalCache, allocationWillReuseTheAllocatedBlocks)
{
    cache.setAllocator(allocator);

    char* mem = cache.alloc(10);
    cache.dealloc(mem, 10);
    mem = cache.alloc(10);
    cache.dealloc(mem, 10);

    LONGS_EQUAL(1, accountant.totalAllocationsOfSize(32));
}


TEST(SimpleStringInternalCache, multipleDifferentSizeAllocationsAndDeallocations)
{
    cache.setAllocator(allocator);

    char* mem10 = cache.alloc(10);
    char* mem11 = cache.alloc(11);

    char* mem100 = cache.alloc(100);
    cache.dealloc(mem100, 100);

    char* mem101 = cache.alloc(101);
    char* mem102 = cache.alloc(102);
    char* mem103 = cache.alloc(103);
    cache.dealloc(mem101, 102);
    cache.dealloc(mem102, 103);
    cache.dealloc(mem103, 104);

    cache.alloc(105);
    cache.alloc(106);
    cache.alloc(107);

    cache.dealloc(mem10, 10);
    cache.dealloc(mem11, 11);

    LONGS_EQUAL(2, accountant.totalAllocationsOfSize(32));
    LONGS_EQUAL(3, accountant.totalAllocationsOfSize(128));
}

TEST(SimpleStringInternalCache, deallocOfCachedMemoryWillNotDealloc)
{
    cache.setAllocator(allocator);

    char* mem = cache.alloc(10);
    cache.dealloc(mem, 10);

    LONGS_EQUAL(0, accountant.totalDeallocationsOfSize(32));
}

TEST(SimpleStringInternalCache, clearCacheWillRemoveAllCachedMemoryButNotAllUsedMemory)
{
    cache.setAllocator(allocator);

    char* mem = cache.alloc(10);
    cache.dealloc(mem, 10);

    mem = cache.alloc(60);

    cache.clearCache();

    LONGS_EQUAL(1, accountant.totalDeallocationsOfSize(32));
    LONGS_EQUAL(0, accountant.totalDeallocationsOfSize(64));
}

TEST(SimpleStringInternalCache, clearAllIncludingCurrentlyUsedMemory)
{
    cache.setAllocator(allocator);

    cache.alloc(60);

    cache.clearAllIncludingCurrentlyUsedMemory();

    LONGS_EQUAL(1, accountant.totalDeallocationsOfSize(64));
}



TEST(SimpleStringInternalCache, allocatingLargerStringThanCached)
{
    cache.setAllocator(allocator);

    char* mem = cache.alloc(1234);
    cache.dealloc(mem, 1234);

    LONGS_EQUAL(1, accountant.totalAllocationsOfSize(1234));
    LONGS_EQUAL(1, accountant.totalDeallocationsOfSize(1234));
}

TEST(SimpleStringInternalCache, allocatingMultipleLargerStringThanCached)
{
    cache.setAllocator(allocator);

    char* mem = cache.alloc(1234);
    char* mem2 = cache.alloc(1234);
    char* mem3 = cache.alloc(1234);

    cache.dealloc(mem2, 1234);
    cache.dealloc(mem, 1234);
    cache.dealloc(mem3, 1234);

    LONGS_EQUAL(3, accountant.totalAllocationsOfSize(1234));
    LONGS_EQUAL(3, accountant.totalDeallocationsOfSize(1234));
}


TEST(SimpleStringInternalCache, clearAllIncludingCurrentlyUsedMemoryAlsoReleasesLargeNonCachesMemory)
{
    cache.setAllocator(allocator);

    cache.alloc(1234);
    cache.alloc(1234);
    cache.alloc(1234);

    cache.clearAllIncludingCurrentlyUsedMemory();

    LONGS_EQUAL(3, accountant.totalAllocationsOfSize(1234));
    LONGS_EQUAL(3, accountant.totalDeallocationsOfSize(1234));
}

static void deallocatingStringMemoryThatWasntAllocatedWithCache_(SimpleStringInternalCache* cache, size_t allocationSize)
{
    char* mem = defaultMallocAllocator()->alloc_memory(allocationSize, __FILE__, __LINE__);
    mem[0] = 'B';
    mem[1] = 'a';
    mem[2] = 's';
    mem[3] = '\0';
    cache->dealloc(mem, allocationSize);
    defaultMallocAllocator()->free_memory(mem, allocationSize, __FILE__, __LINE__);
}

TEST(SimpleStringInternalCache, deallocatingMemoryThatWasntAllocatedWhileCacheWasInPlaceProducesWarning)
{
    testFunction.testFunction = deallocatingStringMemoryThatWasntAllocatedWithCache_;
    testFunction.allocationSize = 123;

    cache.setAllocator(allocator);
    fixture.runAllTests();

    fixture.assertPrintContains("\nWARNING: Attempting to deallocate a String buffer that was allocated while not caching. Ignoring it!\n"
                                "This is likely due statics and will cause problems.\n"
                                "Only warning once to avoid recursive warnings.\n"
                                "String we are deallocating: \"Bas\"\n");

}

static void deallocatingStringMemoryTwiceThatWasntAllocatedWithCache_(SimpleStringInternalCache* cache, size_t allocationSize)
{
    char* mem = defaultMallocAllocator()->alloc_memory(allocationSize, __FILE__, __LINE__);
    mem[0] = '\0';
    cache->dealloc(mem, allocationSize);
    cache->dealloc(mem, allocationSize);
    defaultMallocAllocator()->free_memory(mem, allocationSize, __FILE__, __LINE__);
}

TEST(SimpleStringInternalCache, deallocatingMemoryThatWasntAllocatedWhileCacheWasInPlaceProducesWarningButOnlyOnce)
{
    testFunction.testFunction = deallocatingStringMemoryTwiceThatWasntAllocatedWithCache_;
    testFunction.allocationSize = 123;

    cache.setAllocator(allocator);
    fixture.runAllTests();

    LONGS_EQUAL(1, fixture.getOutput().count("WARNING"));
}

TEST(SimpleStringInternalCache, deallocatingLargeMemoryThatWasntAllocatedWhileCacheWasInPlaceProducesWarning)
{
    testFunction.testFunction = deallocatingStringMemoryThatWasntAllocatedWithCache_;
    testFunction.allocationSize = 12345;

    cache.setAllocator(allocator);
    fixture.runAllTests();

    fixture.assertPrintContains("\nWARNING: Attempting to deallocate a String buffer that was allocated while not caching. Ignoring it!\n"
                                "This is likely due statics and will cause problems.\n"
                                "Only warning once to avoid recursive warnings.\n"
                                "String we are deallocating: \"Bas\"\n");

}

TEST(SimpleStringInternalCache, deallocatingLargeMemoryThatWasntAllocatedWhileCacheWasInPlaceProducesWarningButOnlyOnce)
{
    testFunction.testFunction = deallocatingStringMemoryTwiceThatWasntAllocatedWithCache_;
    testFunction.allocationSize = 12345;

    cache.setAllocator(allocator);
    fixture.runAllTests();

    LONGS_EQUAL(1, fixture.getOutput().count("WARNING"));
}

TEST_GROUP(SimpleStringCacheAllocator)
{
    SimpleStringCacheAllocator* allocator;
    SimpleStringInternalCache cache;
    MemoryAccountant accountant;
    AccountingTestMemoryAllocator* accountingAllocator;

    void setup() _override
    {
        accountingAllocator = new AccountingTestMemoryAllocator(accountant, defaultMallocAllocator());
        allocator = new SimpleStringCacheAllocator(cache, accountingAllocator);
    }

    void teardown() _override
    {
        cache.clearCache();
        delete allocator;
        delete accountingAllocator;
    }
};

TEST(SimpleStringCacheAllocator, allocationIsCached)
{
    char* mem = allocator->alloc_memory(10, __FILE__, __LINE__);
    allocator->free_memory(mem, 10,  __FILE__, __LINE__);

    size_t totalAllocations = accountant.totalAllocations();
    size_t totalDeallocations = accountant.totalDeallocations();

    mem = allocator->alloc_memory(10, __FILE__, __LINE__);
    allocator->free_memory(mem, 10,  __FILE__, __LINE__);

    LONGS_EQUAL(totalAllocations, accountant.totalAllocations());
    LONGS_EQUAL(totalDeallocations, accountant.totalDeallocations());
}

TEST(SimpleStringCacheAllocator, originalAllocator)
{
    POINTERS_EQUAL(defaultMallocAllocator(), allocator->actualAllocator());
    STRCMP_EQUAL(defaultMallocAllocator()->alloc_name(), allocator->alloc_name());
    STRCMP_EQUAL(defaultMallocAllocator()->free_name(), allocator->free_name());
}

TEST(SimpleStringCacheAllocator, name)
{
    STRCMP_EQUAL("SimpleStringCacheAllocator", allocator->name());
}



TEST_GROUP(GlobalSimpleStringCache)
{
};

TEST(GlobalSimpleStringCache, installsAndRemovedCache)
{
    TestMemoryAllocator* originalStringAllocator = SimpleString::getStringAllocator();
    {
        GlobalSimpleStringCache cache;
        STRCMP_EQUAL("SimpleStringCacheAllocator", SimpleString::getStringAllocator()->name());
        POINTERS_EQUAL(cache.getAllocator(), SimpleString::getStringAllocator());
    }
    POINTERS_EQUAL(originalStringAllocator, SimpleString::getStringAllocator());
}
