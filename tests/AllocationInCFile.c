#include <stdlib.h>
#include "AllocationInCFile.h"
#include "CppUTest/MemoryLeakDetectorMallocMacros.h"

/* This file is for simulating overloads of malloc */

char* mallocAllocation()
{
	return malloc(10UL);
}

void freeAllocation(void* memory)
{
	free(memory);
}

#undef free

void freeAllocationWithoutMacro(void* memory)
{
	free(memory);
}

