#ifndef D_FailableMemoryAllocator_H
#define D_FailableMemoryAllocator_H


#include "CppUTest/TestMemoryAllocator.h"
#include <string.h>


class FailableMallocAllocator: public TestMemoryAllocator
{
public:
    enum {MAX_NUMBER_OF_FAILED_ALLOCS = 10};
    FailableMallocAllocator(const char* name_str = "generic", const char* alloc_name_str = "alloc", const char* free_name_str = "free");
    virtual ~FailableMallocAllocator() {};
    virtual void failMallocNumber(int number);
    virtual char* alloc_memory(size_t size, const char* file, int line);
    virtual char* allocMemoryLeakNode(size_t size);
protected:
    virtual bool shouldBeFailedAlloc_();
    int allocsToFail_[MAX_NUMBER_OF_FAILED_ALLOCS];
    int toFailCount_;
    int currentAllocNumber_;
};


#endif
