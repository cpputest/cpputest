
/*
 * This file can be used to get extra debugging information about memory leaks in your production code.
 * It defines a preprocessor macro for operator new. This will pass additional information to the
 * operator new and this will give the line/file information of the memory leaks in your code.
 *
 * You can use this by including this file to all your production code. When using gcc, you can use
 * the -include file to do this for you.
 *
 * Warning: Using the new macro can cause a conflict with newly declared operator news. This can be
 * resolved by:
 * 1. #undef operator new before including this file
 * 2. Including the files that override operator new before this file.
 *    This can be done by creating your own NewMacros.h file that includes your operator new overrides
 *    and THEN this file.
 *
 * STL (or StdC++ lib) also does overrides for operator new. Therefore, you'd need to include the STL
 * files *before* this file too.
 *
 */

#include "CppUTestConfig.h"

/* Make sure that mem leak detection is on and that this is being included from a C++ file */
#if CPPUTEST_USE_MEM_LEAK_DETECTION && defined(__cplusplus)

/* This #ifndef prevents <new> from being included twice and enables the file to be included anywhere */
#ifndef CPPUTEST_USE_NEW_MACROS

    #if CPPUTEST_USE_STD_CPP_LIB
        #ifdef CPPUTEST_USE_STRDUP_MACROS
            #if CPPUTEST_USE_STRDUP_MACROS == 1
            /*
             * Some platforms (OSx, i.e.) will get <string.h> or <cstring> included when using <memory> header,
             *  in order to avoid conflicts with strdup and strndup macros defined by MemoryLeakDetectorMallocMacros.h
             *  we will undefined those macros, include the C++ headers and then reinclude MemoryLeakDetectorMallocMacros.h.
             * The check '#if CPPUTEST_USE_STRDUP_MACROS' will ensure we only include MemoryLeakDetectorMallocMacros.h if
             *  it has already been includeded earlier.
             */
                #undef strdup
                #undef strndup
                #undef CPPUTEST_USE_STRDUP_MACROS
                #define __CPPUTEST_REINCLUDE_MALLOC_MEMORY_LEAK_DETECTOR
            #endif
        #endif
        #include <new>
        #include <memory>
        #include <string>
        #ifdef __CPPUTEST_REINCLUDE_MALLOC_MEMORY_LEAK_DETECTOR
            #include "MemoryLeakDetectorMallocMacros.h"
        #endif
    #endif

    /* Some toolkits, e.g. MFC, provide their own new overloads with signature (size_t, const char *, int).
     * If we don't provide them, in addition to the (size_t, const char *, size_t) version, we don't get to
     * know about all allocations and report freeing of unallocated blocks. Hence, provide both overloads.
     */

    void* operator new(size_t size, const char* file, int line) UT_THROW (CPPUTEST_BAD_ALLOC);
    void* operator new(size_t size, const char* file, size_t line) UT_THROW (CPPUTEST_BAD_ALLOC);
    void* operator new[](size_t size, const char* file, int line) UT_THROW (CPPUTEST_BAD_ALLOC);
    void* operator new[](size_t size, const char* file, size_t line) UT_THROW (CPPUTEST_BAD_ALLOC);
    void* operator new(size_t size) UT_THROW(CPPUTEST_BAD_ALLOC);
    void* operator new[](size_t size) UT_THROW(CPPUTEST_BAD_ALLOC);

    void operator delete(void* mem, const char* file, int line) UT_NOTHROW;
    void operator delete(void* mem, const char* file, size_t line) UT_NOTHROW;
    void operator delete[](void* mem, const char* file, int line) UT_NOTHROW;
    void operator delete[](void* mem, const char* file, size_t line) UT_NOTHROW;
    void operator delete(void* mem) UT_NOTHROW;
    void operator delete[](void* mem) UT_NOTHROW;
#if __cplusplus >= 201402L
    void operator delete (void* mem, size_t size) UT_NOTHROW;
    void operator delete[] (void* mem, size_t size) UT_NOTHROW;
#endif

#endif


#ifdef __clang__
 #pragma clang diagnostic push
 #if (__clang_major__ == 3 && __clang_minor__ >= 6) || __clang_major__ >= 4
  #pragma clang diagnostic ignored "-Wkeyword-macro"
 #endif
#endif

#define new new(__FILE__, __LINE__)

#ifdef __clang__
 #pragma clang diagnostic pop
#endif

#define CPPUTEST_USE_NEW_MACROS 1

#endif
