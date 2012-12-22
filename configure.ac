
AC_PREREQ([2.69])
AC_INIT([CppUTest], [3.3], [https://github.com/cpputest/cpputest])
AC_CONFIG_SRCDIR([src/CppUTest/Utest.cpp])
AC_CONFIG_HEADERS([config.h])

AC_PROG_CXX([clang g++])
AC_PROG_CC([clang gcc])
AC_PROG_CPP([clang gcc])
AC_PROG_LN_S
AC_PROG_MAKE_SET

AC_CHECK_HEADERS([stddef.h stdint.h stdlib.h string.h sys/time.h unistd.h])

AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([gettimeofday memset strstr])

# Different features
AC_ARG_ENABLE([std-c], [  --disable-std-c   disable the use of Standard C Library (warning: requires implementing Platforms/GccNoStdC) ], 
   [use_std_c=${enableval}], [use_std_c=yes])

AC_ARG_ENABLE([std-cpp], [  --disable-std-cpp   disable the use of Standard C++ Library], 
   [use_std_cpp=${enableval}], [use_std_cpp=yes])

AC_ARG_ENABLE([memory-leak-detection], [  --disable-memory-leak-detection   disable memory leak detection], 
   [memory_leak_detection=${enableval}], [memory_leak_detection=yes])

AC_ARG_ENABLE([verbose], [  --enable-verbose   be verbose during compilation], 
   [verbose=${disableval}], [verbose=no])

############################## Values ########################################

CPP_PLATFORM="Gcc"
CPPUTEST_MEMLEAK_DETECTOR_NEW_MACRO_FILE="${srcdir}/include/CppUTest/MemoryLeakDetectorNewMacros.h"
CPPUTEST_MEMLEAK_DETECTOR_MALLOC_MACRO_FILE="${srcdir}/include/CppUTest/MemoryLeakDetectorMallocMacros.h"


###### TO BE CHANGED #############
CPPUTEST_CXXFLAGS="${CPPUTEST_CXXFLAGS} -Weverything -Wno-disabled-macro-expansion -Wno-padded -Wno-global-constructors -Wno-exit-time-destructors -Wno-weak-vtables"
CPPUTEST_CPPFLAGS="${CPPUTEST_CPPFLAGS} -Weverything"


### Setting include #####

CPPUTEST_CPPFLAGS="${CPPUTEST_CPPFLAGS} -I ${srcdir}/include"

############################## Setting options ###############################
if test "x${verbose}" = xno; then
	SILENCE=@
fi
	
# Dealing with not having a Standard C library... (usually for Kernel development)
if test "x${use_std_c}" = xno; then
    CPP_PLATFORM="GccNoStdC"
    AC_SUBST([CPPUTEST_USE_STD_C_LIB], [N])
fi

# Dealing with memory leak detection
if test "x${memory_leak_detection}" = xno; then
	AC_DEFINE([CPPUTEST_MEM_LEAK_DETECTION_DISABLED], 1, [memory leak detection disabled])
else
	CPPUTEST_CXXFLAGS="${CPPUTEST_CXXFLAGS} -include ${CPPUTEST_MEMLEAK_DETECTOR_NEW_MACRO_FILE}"
	CPPUTEST_CPPFLAGS="${CPPUTEST_CPPFLAGS} -include ${CPPUTEST_MEMLEAK_DETECTOR_MALLOC_MACRO_FILE}"
fi


# Variables to substitute

AC_SUBST([SILENCE])

AC_SUBST([CPPUTEST_OBJS_DIR], objs)
AC_SUBST([CPPUTEST_LIB_DIR], lib)
AC_SUBST([CPPUTEST_SRC_DIRS], "src/CppUTest src/Platforms/${CPP_PLATFORM}")
AC_SUBST([CPPUTEST_TEST_DIRS], tests)
AC_SUBST([CPPUTEST_LIB], ${CPPUTEST_LIB_DIR}/libCppUTest.a)
AC_SUBST([CPPUTEST_TEST_EXEC], CppUTest_tests)

# Replacement of tool flags
AC_SUBST([CPPUTEST_CFLAGS])
AC_SUBST([CPPUTEST_ADDITIONAL_CFLAGS])
AC_SUBST([CPPUTEST_CXXFLAGS])
AC_SUBST([CPPUTEST_ADDITIONAL_CXXFLAGS])
AC_SUBST([CPPUTEST_CPPFLAGS])
AC_SUBST([CPPUTEST_ADDITIONAL_CPPFLAGS])
AC_SUBST([CPPUTEST_LDFLAGS])
AC_SUBST([CPPUTEST_ADDITIONAL_LDFLAGS])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo \
"----------------------------------------------------------------

${PACKAGE_NAME} Version ${PACKAGE_VERSION}

Features configured in ${PACKAGE_NAME}:
   Memory Leak Detection:       ${memory_leak_detection}
   Using Standard C++ Library:  ${use_std_cpp}
   Using Standard C Library:    ${use_std_c}

----------------------------------------------------------------"