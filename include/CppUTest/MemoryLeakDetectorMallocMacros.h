
/*
 * This file can be used to get extra debugging information about memory leaks in your production code.
 * It defines a preprocessor macro for malloc. This will pass additional information to the
 * malloc and this will give the line/file information of the memory leaks in your code.
 *
 * You can use this by including this file to all your production code. When using gcc, you can use
 * the -include file to do this for you.
 *
 */

#include "CppUTestConfig.h"

#if CPPUTEST_USE_MEM_LEAK_DETECTION

/* This prevents the declaration from done twice and makes sure the file only #defines malloc, so it can be included anywhere */
#ifndef CPPUTEST_USE_MALLOC_MACROS

#ifdef __cplusplus
extern "C"
{
#endif

extern void* cpputest_malloc_location(size_t size, const char* file, int line);
extern void* cpputest_calloc_location(size_t count, size_t size, const char* file, int line);
extern void* cpputest_realloc_location(void *, size_t, const char* file, int line);
extern void cpputest_free_location(void* buffer, const char* file, int line);

#ifdef __cplusplus
}
#endif

extern void crash_on_allocation_number(unsigned number);


#define malloc(a) cpputest_malloc_location(a, __FILE__, __LINE__)
#define calloc(a, b) cpputest_calloc_location(a, b, __FILE__, __LINE__)
#define realloc(a, b) cpputest_realloc_location(a, b, __FILE__, __LINE__)
#define free(a) cpputest_free_location(a, __FILE__, __LINE__)

#define CPPUTEST_USE_MALLOC_MACROS 1
#endif /* CPPUTEST_USE_MALLOC_MACROS */

/* This prevents strdup macros to get defined, unless it has been enabled by the user or generated config */
#ifdef CPPUTEST_HAVE_STRDUP

/* This prevents the declaration from done twice and makes sure the file only #defines strdup, so it can be included anywhere */
#ifndef CPPUTEST_USE_STRDUP_MACROS

#ifdef __cplusplus
extern "C"
{
#endif

extern char* cpputest_strdup_location(const char* str, const char* file, int line);
extern char* cpputest_strndup_location(const char* str, size_t n, const char* file, int line);

#ifdef __cplusplus
}
#endif

#define strdup(str) cpputest_strdup_location(str, __FILE__, __LINE__)
#define strndup(str, n) cpputest_strndup_location(str, n, __FILE__, __LINE__)

#define CPPUTEST_USE_STRDUP_MACROS 1
#endif /* CPPUTEST_USE_STRDUP_MACROS */
#endif /* CPPUTEST_HAVE_STRDUP */
#endif /* CPPUTEST_USE_MEM_LEAK_DETECTION */
