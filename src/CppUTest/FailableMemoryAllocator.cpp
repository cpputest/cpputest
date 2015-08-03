#include "CppUTest/TestHarness.h"
#include "CppUTest/FailableMemoryAllocator.h"
#include "CppUTest/PlatformSpecificFunctions.h"


FailableMemoryAllocator::FailableMemoryAllocator(const char* name_str, const char* alloc_name_str, const char* free_name_str)
: TestMemoryAllocator(name_str, alloc_name_str, free_name_str)
, toFailCount_(0), currentAllocNumber_(0)
{
    memset(allocsToFail_, 0, sizeof(allocsToFail_));
}

void FailableMemoryAllocator::failMallocNumber(int number)
{
    if (toFailCount_ >= MAX_NUMBER_OF_FAILED_ALLOCS)
        FAIL("Maximum number of failed memory allocations exceeded")
    allocsToFail_[toFailCount_++] = number;
}

char* FailableMemoryAllocator::alloc_memory(size_t size, const char* file, int line)
{
    currentAllocNumber_++;
    if (shouldBeFailedAlloc_())
        return NULL;
    return TestMemoryAllocator::alloc_memory(size, file, line);
}

bool FailableMemoryAllocator::shouldBeFailedAlloc_()
{
    for (int i = 0; i < toFailCount_; i++)
        if (currentAllocNumber_ == allocsToFail_[i])
            return true;
    return false;
}

char* FailableMemoryAllocator::allocMemoryLeakNode(size_t size)
{
    return (char*)PlatformSpecificMalloc(size);
}

void FailableMemoryAllocator::clearFailedAllocations()
{
    toFailCount_ = 0;
    currentAllocNumber_ = 0;
    memset(allocsToFail_, 0, sizeof(allocsToFail_));
}

