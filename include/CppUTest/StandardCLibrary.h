
#ifndef STANDARDCLIBRARY_H_
#define STANDARDCLIBRARY_H_

#ifndef CPPUTEST_USE_STD_C_LIB
#ifdef CPPUTEST_STD_C_LIB_DISABLED
#define CPPUTEST_USE_STD_C_LIB 0
#else
#define CPPUTEST_USE_STD_C_LIB 1
#endif
#endif

#if CPPUTEST_USE_STD_C_LIB == 0

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
typedef unsigned int size_t;
#endif

typedef char* va_list;
#define NULL (0)
extern void*	malloc(size_t);
extern void     free(void *);

#define _bnd(X, bnd)            (((sizeof (X)) + (bnd)) & (~(bnd)))
#define va_start(ap, A)         (void) ((ap) = (((char *) &(A)) + (_bnd (A,sizeof(int)-1))))
#define va_end(ap)              (void) 0

#endif

#else

/* Needed for size_t */
#include <stddef.h>

/* Sometimes the C++ library does an #undef in stdlib of malloc and free. We want to prevent that */
#ifdef __cplusplus
#if CPPUTEST_USE_STD_CPP_LIB
#include <cstdlib>
#endif
#endif

/* Needed for malloc */
#include <stdlib.h>

/* Needed for ... */
#include <stdarg.h>

#endif

#endif
