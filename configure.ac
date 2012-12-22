
AC_PREREQ([2.69])
AC_INIT([CppUTest], [3.3], [https://github.com/cpputest/cpputest])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/CppUTest/Utest.cpp])
AC_CONFIG_HEADERS([config.h])

AC_PROG_CXX([clang g++])
AC_PROG_CC([clang gcc])
AC_PROG_CPP([clang gcc])
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AM_PROG_CC_C_O
AM_SILENT_RULES([yes])

# Checking for warning flags on the compiler
saved_cflags="$CFLAGS"

# FLag -Werror. 
CFLAGS=-Werror
AC_MSG_CHECKING([whether CC and CXX supports -Werror])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Werror"; CPPUTEST_CXXWARNINGFLAGS+=" -Werror" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Weverything. 
CFLAGS=-Weverything
AC_MSG_CHECKING([whether CC and CXX supports -Weverything])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Weverything"; CPPUTEST_CXXWARNINGFLAGS+=" -Weverything" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wall. 
CFLAGS=-Wall
AC_MSG_CHECKING([whether CC and CXX supports -Wall])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wall"; CPPUTEST_CXXWARNINGFLAGS+=" -Wall" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wextra. 
CFLAGS=-Wextra
AC_MSG_CHECKING([whether CC and CXX supports -Wextra])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wextra"; CPPUTEST_CXXWARNINGFLAGS+=" -Wextra" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wshadow. 
CFLAGS=-Wshadow
AC_MSG_CHECKING([whether CC and CXX supports -Wshadow])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wshadow"; CPPUTEST_CXXWARNINGFLAGS+=" -Wshadow" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wswitch-default. 
CFLAGS=-Wswitch-default
AC_MSG_CHECKING([whether CC and CXX supports -Wswitch-default])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wswitch-default"; CPPUTEST_CXXWARNINGFLAGS+=" -Wswitch-default" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wswitch-enum. 
CFLAGS=-Wswitch-enum
AC_MSG_CHECKING([whether CC and CXX supports -Wswitch-enum])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wswitch-enum"; CPPUTEST_CXXWARNINGFLAGS+=" -Wswitch-enum" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wconversion. 
CFLAGS=-Wconversion
AC_MSG_CHECKING([whether CC and CXX supports -Wconversion])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wconversion"; CPPUTEST_CXXWARNINGFLAGS+=" -Wconversion" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# Disable some warnings as CppUTest has this and can't be prevented at the moment.
# FLag -Wno-disabled-macro-expansion. 
CFLAGS=-Wno-disabled-macro-expansion
AC_MSG_CHECKING([whether CC and CXX supports -Wno-disabled-macro-expansion])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wno-disabled-macro-expansion"; CPPUTEST_CXXWARNINGFLAGS+=" -Wno-disabled-macro-expansion" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wno-padded. 
CFLAGS=-Wno-padded
AC_MSG_CHECKING([whether CC and CXX supports -Wno-padded])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wno-padded"; CPPUTEST_CXXWARNINGFLAGS+=" -Wno-padded" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wno-global-constructors. 
CFLAGS=-Wno-global-constructors
AC_MSG_CHECKING([whether CC and CXX supports -Wno-global-constructors])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wno-global-constructors"; CPPUTEST_CXXWARNINGFLAGS+=" -Wno-global-constructors" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wno-exit-time-destructors. 
CFLAGS=-Wno-exit-time-destructors
AC_MSG_CHECKING([whether CC and CXX supports -Wno-exit-time-destructors])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wno-exit-time-destructors"; CPPUTEST_CXXWARNINGFLAGS+=" -Wno-exit-time-destructors" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wno-weak-vtables. 
CFLAGS=-Wno-weak-vtables
AC_MSG_CHECKING([whether CC and CXX supports -Wno-weak-vtables])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wno-weak-vtables"; CPPUTEST_CXXWARNINGFLAGS+=" -Wno-weak-vtables" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

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

############################## Values ########################################
CPPUTEST_CFLAGS+="${CPPUTEST_CWARNINGFLAGS}"
CPPUTEST_CXXFLAGS+="${CPPUTEST_CXXWARNINGFLAGS}"


CPP_PLATFORM="Gcc"
CPPUTEST_MEMLEAK_DETECTOR_NEW_MACRO_FILE="${srcdir}/include/CppUTest/MemoryLeakDetectorNewMacros.h"
CPPUTEST_MEMLEAK_DETECTOR_MALLOC_MACRO_FILE="${srcdir}/include/CppUTest/MemoryLeakDetectorMallocMacros.h"


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

AC_SUBST([CPPUTEST_CFLAGS])
AC_SUBST([CPPUTEST_CPPFLAGS])
AC_SUBST([CPPUTEST_CXXFLAGS])

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