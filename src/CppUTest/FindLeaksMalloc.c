#include "CppUTest/TestHarness_c.h"
#include "CppUTest/FindLeaksMalloc.h"


void* find_leaks_malloc(size_t size, char* file, int line)
{
    return cpputest_malloc(size);
}

void find_leaks_free(void* mem)
{
    cpputest_free(mem);
}

