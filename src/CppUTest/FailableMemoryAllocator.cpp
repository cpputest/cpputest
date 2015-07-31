#include "CppUTest/TestHarness.h"
#include "CppUTest/FailableMemoryAllocator.h"
#include "CppUTest/PlatformSpecificFunctions.h"

void FailableMallocAllocator::failMallocNumber(int number)
{
    allocsToFail_[toFailCount_++] = number; // TODO: Limit check
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

