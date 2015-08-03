#include "CppUTest/TestHarness.h"
#include "CppUTest/FailableMemoryAllocator.h"
#include "CppUTest/PlatformSpecificFunctions.h"


FailableMallocAllocator::FailableMallocAllocator(const char* name_str, const char* alloc_name_str, const char* free_name_str)
: TestMemoryAllocator(name_str, alloc_name_str, free_name_str)
, toFailCount_(0), currentAllocNumber_(0)
{
    memset(allocsToFail_, 0, sizeof(allocsToFail_));
}

void FailableMallocAllocator::failMallocNumber(int number)
{
    if (toFailCount_ >= MAX_NUMBER_OF_FAILED_ALLOCS)
        FAIL("Maximum number of failed memory allocations exceeded")
    allocsToFail_[toFailCount_++] = number;
}

char* FailableMallocAllocator::alloc_memory(size_t size, const char* file, int line)
{
    currentAllocNumber_++;
    if (shouldBeFailedAlloc_())
        return NULL;
    return TestMemoryAllocator::alloc_memory(size, file, line);
}

bool FailableMallocAllocator::shouldBeFailedAlloc_()
{
    for (int i = 0; i < toFailCount_; i++)
        if (currentAllocNumber_ == allocsToFail_[i])
            return true;
    return false;
}

char* FailableMallocAllocator::allocMemoryLeakNode(size_t size)
{
    return (char*)PlatformSpecificMalloc(size);
}

void FailableMallocAllocator::clearFailedAllocations()
{
    toFailCount_ = 0;
    currentAllocNumber_ = 0;
    memset(allocsToFail_, 0, sizeof(allocsToFail_));
}

