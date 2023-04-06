
/* Must include this first to ensure the StandardC include in CppUTestConfig still happens at the right moment */
#include "CppUTestConfig.h"

#ifndef STANDARDCLIBRARY_H_
#define STANDARDCLIBRARY_H_

#if CPPUTEST_USE_STD_C_LIB

/* Needed for size_t */
#include <stddef.h>

/* Sometimes the C++ library does an #undef in stdlib of malloc and free. We want to prevent that */
#ifdef __cplusplus
 #if CPPUTEST_USE_STD_CPP_LIB
  #include <cstdlib>
  #include <cstring>
  #include <string>
 #endif
#endif

/* Needed for malloc */
#include <stdlib.h>

/* Needed for std::nullptr */
#ifdef __cplusplus
 #if CPPUTEST_USE_STD_CPP_LIB
  #include <cstddef>
 #endif
#endif

/* Needed for ... */
#include <stdarg.h>

/* Kludge to get a va_copy in VC++ V6 and in GCC 98 */
#ifndef va_copy
#ifdef __GNUC__
#define va_copy __va_copy
#else
#define va_copy(copy, original) copy = original;
#endif
#endif

/* Needed for some detection of long long and 64 bit */
#include <limits.h>

/* Needed to ensure that string.h is included prior to strdup redefinition */
#ifdef CPPUTEST_HAVE_STRING_H
#include <string.h>
#endif

#else

#ifdef __KERNEL__

/* Unfinished and not working! Hacking hacking hacking. Why bother make the header files C++ safe! */
#define false kernel_false
#define true kernel_true
#define bool kernel_bool
#define new kernel_new
#define _Bool int
#include <linux/acpi.h>
#include <linux/types.h>
#undef false
#undef true
#undef bool
#undef new

#else

/*
 * #warning "These definitions in StandardCLibrary.h are pure (educated, from linux kernel) guesses at the moment. Replace with your platform includes."
 * Not on as warning are as errors :P
 */

#ifdef __SIZE_TYPE__
typedef __SIZE_TYPE__ size_t;
#else
typedef long unsigned int size_t;
#endif

#define NULL (0)
#ifdef __cplusplus
extern "C" {
#endif
extern void*	malloc(size_t);
extern void     free(void *);
#ifdef __cplusplus
}
#endif


#define _bnd(X, bnd)            (((sizeof (X)) + (bnd)) & (~(bnd)))

#define va_list __builtin_va_list
#define va_copy __builtin_va_copy
#define va_start __builtin_va_start
#define va_end __builtin_va_end

#endif

#endif

#endif
