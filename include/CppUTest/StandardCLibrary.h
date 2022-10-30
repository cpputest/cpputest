
/* Must include this first to ensure the StandardC include in CppUTestConfig still happens at the right moment */
#include "CppUTestConfig.h"

#ifndef STANDARDCLIBRARY_H_
#define STANDARDCLIBRARY_H_

/* Needed for size_t */
#include <stddef.h>

/* Sometimes the C++ library does an #undef in stdlib of malloc and free. We want to prevent that */
#ifdef __cplusplus
 #if CPPUTEST_USE_STD_CPP_LIB
  #include <cstdlib>
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

#endif
