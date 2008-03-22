#ifndef FINDLEAKSMALLOC_H_
#define FINDLEAKSMALLOC_H_
#include <stdlib.h>

void* find_leaks_malloc(size_t size, char* file, int line);
void find_leaks_free(void* mem);

#define FIND_LEAKS_MALLOC(size) find_leaks_malloc(size, __FILE__, __LINE__)
#define FIND_LEAKS_FREE(mem) find_leaks_free(mem)

#endif /*FINDLEAKSMALLOC_H_*/
