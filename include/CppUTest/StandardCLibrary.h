
#ifndef STANDARDCLIBRARY_H_
#define STANDARDCLIBRARY_H_

#if CPPUTEST_STD_C_LIB_DISABLED

typedef unsigned long size_t;
typedef void* va_list;
#define NULL (0)
extern void    *malloc(size_t);
extern void     free(void *);
#define va_start(arguments, format) /* arguments format */
#define va_end(arguments) /* arguments */

#else

/* Needed for size_t */
#include <stddef.h>

/* Needed for malloc */
#include <stdlib.h>

/* Needed for ... */
#include <stdarg.h>

#endif

#endif
