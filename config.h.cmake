#ifndef CONFIG_H_
#define CONFIG_H_

#cmakedefine CPPUTEST_MEM_LEAK_DETECTION_DISABLED
#cmakedefine01 CPPUTEST_USE_LONG_LONG
#cmakedefine CPPUTEST_HAVE_STRDUP

#cmakedefine CPPUTEST_HAVE_FORK
#cmakedefine CPPUTEST_HAVE_WAITPID
#cmakedefine CPPUTEST_HAVE_PTHREAD_MUTEX_LOCK

#cmakedefine CPPUTEST_HAVE_GETTIMEOFDAY

#cmakedefine01 CPPUTEST_HAS_INF
#cmakedefine01 CPPUTEST_HAS_NAN
#cmakedefine01 CPPUTEST_HAVE_FENV

#cmakedefine CPPUTEST_HAVE_STRUCT_TIMESPEC
#ifdef CPPUTEST_HAVE_STRUCT_TIMESPEC
/* Apply workaround for MinGW timespec redefinition (pthread.h / time.h) */
#define _TIMESPEC_DEFINED 1
#endif

#cmakedefine CPPUTEST_STD_C_LIB_DISABLED
#cmakedefine CPPUTEST_STD_CPP_LIB_DISABLED

#cmakedefine INCLUDE_GTEST_TESTS

#endif
