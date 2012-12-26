
AC_PREREQ([2.69])
AC_INIT([CppUTest], [3.3], [https://github.com/cpputest/cpputest])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/CppUTest/Utest.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([cpputest.pc])

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
saved_cxxflags="$CXXFLAGS"

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

# FLag -Wswitch-default
CFLAGS=-Wswitch-default
AC_MSG_CHECKING([whether CC and CXX supports -Wswitch-default])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wswitch-default"; CPPUTEST_CXXWARNINGFLAGS+=" -Wswitch-default" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wswitch-enum
CFLAGS=-Wswitch-enum
AC_MSG_CHECKING([whether CC and CXX supports -Wswitch-enum])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wswitch-enum"; CPPUTEST_CXXWARNINGFLAGS+=" -Wswitch-enum" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wconversion 
CFLAGS=-Wconversion
AC_MSG_CHECKING([whether CC and CXX supports -Wconversion])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wconversion"; CPPUTEST_CXXWARNINGFLAGS+=" -Wconversion" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -pedantic-errors 
CFLAGS=-pedantic-errors
AC_MSG_CHECKING([whether CC and CXX supports -pedantic-errors])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -pedantic-errors"; CPPUTEST_CXXWARNINGFLAGS+=" -pedantic-errors" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wsign-conversion 
CFLAGS=-Wsign-conversion
AC_MSG_CHECKING([whether CC and CXX supports -Wsign-conversion])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wsign-conversion"; CPPUTEST_CXXWARNINGFLAGS+=" -Wsign-conversion" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Woverloaded-virtual 
AC_LANG_PUSH([C++])
CXXFLAGS=-Woverloaded-virtual
AC_MSG_CHECKING([whether CXX supports -Woverloaded-virtual])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CXXWARNINGFLAGS+=" -Woverloaded-virtual" ], [AC_MSG_RESULT([no])])
CXXFLAGS="$saved_cxxflags"
AC_LANG_POP

# FLag -Wstrict-prototypes 
CFLAGS=-Wstrict-prototypes
AC_MSG_CHECKING([whether CC supports -Wstrict-prototypes])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wstrict-prototypes" ], [AC_MSG_RESULT([no])])
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

AC_CHECK_PROG([CPPUTEST_HAS_GCC], [gcc], [yes], [no])


# Different features
AC_ARG_ENABLE([std-c], [AC_HELP_STRING([--disable-std-c], [disable the use of Standard C Library (warning: requires implementing Platforms/GccNoStdC) ])], 
   [use_std_c=${enableval}], [use_std_c=yes])

AC_ARG_ENABLE([std-cpp], [AC_HELP_STRING([--disable-std-cpp], [disable the use of Standard C++ Library])], 
   [use_std_cpp=${enableval}], [use_std_cpp=yes])

AC_ARG_ENABLE([memory-leak-detection], [AC_HELP_STRING([--disable-memory-leak-detection], [disable memory leak detection])], 
   [memory_leak_detection=${enableval}], [memory_leak_detection=yes])

AC_ARG_ENABLE([extensions], [AC_HELP_STRING([--disable-extensions], [disable CppUTest extension library])], 
   [cpputest_ext=${enableval}], [cpputest_ext=yes])

AC_ARG_ENABLE([generate-map-file], [AC_HELP_STRING([--enable-generate-map-file], [enable the creation of a map file])], 
   [generate_map_file=${enableval}], [generate_map_file=no])

AC_ARG_ENABLE([coverage], [AC_HELP_STRING([--enable-coverage], [enable running with coverage])], 
   [coverage=${enableval}], [coverage=no])

############################## Values ########################################
CPPUTEST_CFLAGS+="${CPPUTEST_CWARNINGFLAGS}"
CPPUTEST_CXXFLAGS+="${CPPUTEST_CXXWARNINGFLAGS}"
CPP_PLATFORM="Gcc"

### Setting include #####

CPPUTEST_CPPFLAGS="${CPPUTEST_CPPFLAGS} -I ${srcdir}/include"
CPPUTEST_LDFLAGS+=" -lstdc++"

### All files in git
ALL_FILES_IN_GIT="`git --git-dir=${srcdir}/.git ls-files | tr '[ \n]' ' '`"

############################## Setting options ###############################

AM_CONDITIONAL([INCLUDE_CPPUTEST_EXT], [test "x${cpputest_ext}" = xyes]) 

# Dealing with not having a Standard C library... (usually for Kernel development)
if test "x${use_std_c}" = xno; then
	use_std_cpp=no
	memory_leak_detection=no
    CPPUTEST_CPPFLAGS+=" -nostdinc"
    CPPUTEST_LDFLAGS+=" -nodefaultlibs"
    AC_DEFINE([CPPUTEST_STD_C_LIB_DISABLED], [1], [Standard C library disabled])
    CPP_PLATFORM="GccNoStdC"
fi

# Using standard C++
if test "x${use_std_cpp}" = xno; then
	AC_DEFINE([CPPUTEST_STD_CPP_LIB_DISABLED], 1, [Standard C++ library disabled])
	if test "x${use_std_c}" = xyes; then
    	CPPUTEST_CPPFLAGS+=" -nostdinc++"
    fi
fi

# Dealing with memory leak detection
if test "x${memory_leak_detection}" = xno; then
	AC_DEFINE([CPPUTEST_MEM_LEAK_DETECTION_DISABLED], 1, [memory leak detection disabled])
else
	CPPUTEST_CXXFLAGS+="${CPPUTEST_CXXFLAGS} -include ${srcdir}/include/CppUTest/MemoryLeakDetectorNewMacros.h"
	CPPUTEST_CPPFLAGS+="${CPPUTEST_CPPFLAGS} -include ${srcdir}/include/CppUTest/MemoryLeakDetectorMallocMacros.h"
fi

# Generating map files.
if test "x${generate_map_file}" = xyes; then
	CPPUTEST_LDFLAGS+=" -Wl,-map,$<.map.txt"
	MOSTLYCLEANFILES+=" *.map.txt"
fi

if test "x${coverage}" = xyes; then
	CPPUTEST_CXXFLAGS+=" -fprofile-arcs -ftest-coverage"
	CPPUTEST_CFLAGS+=" -fprofile-arcs -ftest-coverage"
	MOSTLYCLEANFILES+=" *.gcda *.gcno"
fi

# Variables to substitute

AC_SUBST([CPP_PLATFORM])

AC_SUBST([INCLUDE_CPPUTEST_EXT])

# Replacement of tool flags
AC_SUBST([CPPUTEST_CFLAGS])
AC_SUBST([CPPUTEST_ADDITIONAL_CFLAGS])
AC_SUBST([CPPUTEST_CXXFLAGS])
AC_SUBST([CPPUTEST_ADDITIONAL_CXXFLAGS])
AC_SUBST([CPPUTEST_CPPFLAGS])
AC_SUBST([CPPUTEST_ADDITIONAL_CPPFLAGS])
AC_SUBST([CPPUTEST_LDFLAGS])
AC_SUBST([CPPUTEST_ADDITIONAL_LDFLAGS])

AC_SUBST([MOSTLYCLEANFILES])
AC_SUBST([CPPUTEST_HAS_GCC])
AC_SUBST([ALL_FILES_IN_GIT])

AC_DEFINE([CPPUTEST_COMPILATION], [1], [Compiling CppUTest itself])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo \
"----------------------------------------------------------------

${PACKAGE_NAME} Version ${PACKAGE_VERSION}

Features configured in ${PACKAGE_NAME}:
   Memory Leak Detection:       ${memory_leak_detection}
   Using Standard C++ Library:  ${use_std_cpp}
   Using Standard C Library:    ${use_std_c}
   Compiling extensions:        ${cpputest_ext}
   Generating map file:         ${generate_map_file}
   Compiling w coverage info:   ${coverage}

----------------------------------------------------------------"