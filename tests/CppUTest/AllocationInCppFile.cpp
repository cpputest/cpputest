/* This file is for emulating allocations in a C++ file.
 * It is used simulating the use of the memory leak detector on production code in C++
 */
#undef new
#include "CppUTest/MemoryLeakDetectorNewMacros.h"
#include "AllocationInCppFile.h"

char* newAllocation()
{
    return new char;
}

char* newArrayAllocation()
{
    return new char[100];
}

#undef new

char* newAllocationWithoutMacro()
{
    return new char;
}

char* newArrayAllocationWithoutMacro()
{
    return new char[100];
}

int* newIntAllocationWithoutMacro()
{
    return new int;
}

int* newIntNoThowAllocationWithoutMacro()
{
    return new (std::nothrow) int;
}

char* newCharArrayAllocationWithoutMacro()
{
    return new char[20];
}

char* newCharArrayNoThrowAllocationWithoutMacro()
{
    return new (std::nothrow) char[20];
}

#if CPPUTEST_HAVE_EXCEPTIONS

ClassThatThrowsAnExceptionInTheConstructor::ClassThatThrowsAnExceptionInTheConstructor()
{
  throw 1;
}

#endif
