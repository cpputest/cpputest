
AC_PREREQ([2.68])
AC_INIT([CppUTest], [3.5], [https://github.com/cpputest/cpputest])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/CppUTest/Utest.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([cpputest.pc])
AC_CONFIG_MACRO_DIR([m4])

AC_CANONICAL_HOST
AM_MAINTAINER_MODE([enable])

case "x$build_os" in
	*darwin*)
		AC_SUBST([CPPUTEST_ON_MACOSX], [yes])
   ;;
esac


AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_LN_S
AC_PROG_MAKE_SET
AM_PROG_CC_C_O
AM_SILENT_RULES

ACX_PTHREAD([LIBS="$PTHREAD_LIBS $LIBS"]) 


# This additional -lpthread was added due to a bug on gcc for MacOSX: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=42159
# According to the bug report, a workaround is to link -lpthread. Even the ACX_PTHREAD doesn't do that, so we add an
# additional check if that it possible, and if it is, then we link pthread

saved_libs="$LIBS"
LIBS=-lpthread
AC_MSG_CHECKING([if we can link -lpthread to work around a gcc bug (on MacOSX)])
AC_LINK_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); HACK_TO_USE_PTHREAD_LIBS+=" -lpthread"], [AC_MSG_RESULT([no])])
LIBS="$saved_libs $HACK_TO_USE_PTHREAD_LIBS"

AC_CHECK_HEADERS([stddef.h stdint.h stdlib.h string.h sys/time.h unistd.h])

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
AC_CHECK_PROG([CPPUTEST_HAS_CLANG], [clang], [yes], [no])
AC_CHECK_PROG([CPPUTEST_HAS_LCOV], [lcov], [yes], [no])
AC_CHECK_PROG([CPPUTEST_HAS_VALGRIND], [valgrind], [yes], [no])

# Checking for warning flags on the compiler
saved_cflags="$CFLAGS"
saved_cxxflags="$CXXFLAGS"
saved_ldflags="$LDFLAGS"

if test "x$USE_MAINTAINER_MODE" = "xyes"; then
# FLag -Werror. 
CFLAGS=-Werror
AC_MSG_CHECKING([whether CC and CXX supports -Werror])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Werror"; CPPUTEST_CXXWARNINGFLAGS+=" -Werror" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"
fi

# FLag -Weverything. 
CFLAGS="-Werror -Weverything -Wno-unused-macros"
AC_MSG_CHECKING([whether CC and CXX supports -Weverything])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Weverything"; CPPUTEST_CXXWARNINGFLAGS+=" -Weverything" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wall. 
CFLAGS="-Werror -Wall"
AC_MSG_CHECKING([whether CC and CXX supports -Wall])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wall"; CPPUTEST_CXXWARNINGFLAGS+=" -Wall" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wextra. 
CFLAGS="-Werror -Wextra"
AC_MSG_CHECKING([whether CC and CXX supports -Wextra])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wextra"; CPPUTEST_CXXWARNINGFLAGS+=" -Wextra" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wshadow. 
CFLAGS="-Werror -Wshadow"
AC_MSG_CHECKING([whether CC and CXX supports -Wshadow])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wshadow"; CPPUTEST_CXXWARNINGFLAGS+=" -Wshadow" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wswitch-default
CFLAGS="-Werror -Wswitch-default"
AC_MSG_CHECKING([whether CC and CXX supports -Wswitch-default])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wswitch-default"; CPPUTEST_CXXWARNINGFLAGS+=" -Wswitch-default" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wswitch-enum
CFLAGS="-Werror -Wswitch-enum"
AC_MSG_CHECKING([whether CC and CXX supports -Wswitch-enum])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wswitch-enum"; CPPUTEST_CXXWARNINGFLAGS+=" -Wswitch-enum" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wconversion 
CFLAGS="-Werror -Wconversion"
AC_MSG_CHECKING([whether CC and CXX supports -Wconversion])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wconversion"; CPPUTEST_CXXWARNINGFLAGS+=" -Wconversion" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -pedantic-errors 
CFLAGS="-Werror -pedantic-errors"
AC_MSG_CHECKING([whether CC and CXX supports -pedantic-errors])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -pedantic-errors"; CPPUTEST_CXXWARNINGFLAGS+=" -pedantic-errors" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wsign-conversion 
CFLAGS="-Werror -Wsign-conversion"
AC_MSG_CHECKING([whether CC supports -Wsign-conversion])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wsign-conversion"], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wsign-conversion (for CXX)
AC_LANG_PUSH([C++])
CXXFLAGS="-Werror -Wsign-conversion"
AC_MSG_CHECKING([whether CXX supports -Wsign-conversion])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CXXWARNINGFLAGS+=" -Wsign-conversion"], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cxxflags"
AC_LANG_POP

# FLag -Woverloaded-virtual 
AC_LANG_PUSH([C++])
CXXFLAGS="-Werror -Woverloaded-virtual"
AC_MSG_CHECKING([whether CXX supports -Woverloaded-virtual])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CXXWARNINGFLAGS+=" -Woverloaded-virtual" ], [AC_MSG_RESULT([no])])
CXXFLAGS="$saved_cxxflags"
AC_LANG_POP

# FLag -Wstrict-prototypes 
CFLAGS="-Werror -Wstrict-prototypes"
AC_MSG_CHECKING([whether CC supports -Wstrict-prototypes])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wstrict-prototypes" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# Disable some warnings as CppUTest has this and can't be prevented at the moment.
# FLag -Wno-disabled-macro-expansion. 
CFLAGS="-Werror -Wno-disabled-macro-expansion"
AC_MSG_CHECKING([whether CC and CXX supports -Wno-disabled-macro-expansion])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wno-disabled-macro-expansion"; CPPUTEST_CXXWARNINGFLAGS+=" -Wno-disabled-macro-expansion" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wno-padded. 
CFLAGS="-Werror -Wno-padded"
AC_MSG_CHECKING([whether CC and CXX supports -Wno-padded])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CWARNINGFLAGS+=" -Wno-padded"; CPPUTEST_CXXWARNINGFLAGS+=" -Wno-padded" ], [AC_MSG_RESULT([no])])
CFLAGS="$saved_cflags"

# FLag -Wno-global-constructors. 
AC_LANG_PUSH([C++])
CXXFLAGS="-Werror -Wno-global-constructors"
AC_MSG_CHECKING([whether CXX supports -Wno-global-constructors])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CXXWARNINGFLAGS+=" -Wno-global-constructors" ], [AC_MSG_RESULT([no])])
CXXFLAGS="$saved_cxxflags"
AC_LANG_POP

# FLag -Wno-exit-time-destructors. 
AC_LANG_PUSH([C++])
CXXFLAGS="-Werror -Wno-exit-time-destructors"
AC_MSG_CHECKING([whether CXX supports -Wno-exit-time-destructors])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CXXWARNINGFLAGS+=" -Wno-exit-time-destructors" ], [AC_MSG_RESULT([no])])
CXXFLAGS="$saved_cxxflags"
AC_LANG_POP

# FLag -Wno-weak-vtables. 
AC_LANG_PUSH([C++])
CXXFLAGS="-Werror -Wno-weak-vtables"
AC_MSG_CHECKING([whether CXX supports -Wno-weak-vtables])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CXXWARNINGFLAGS+=" -Wno-weak-vtables" ], [AC_MSG_RESULT([no])])
CXXFLAGS="$saved_cxxflags"
AC_LANG_POP

### Checking for Standard C++ 11 compiler flags
AC_LANG_PUSH([C++])

# FLag -std=c++0x 
CXXFLAGS="-std=c++0x"
AC_MSG_CHECKING([whether CXX supports -std=c++0x])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CPP11FLAG="-std=c++0x" ], [AC_MSG_RESULT([no])])
CXXFLAGS="$saved_cxxflags"

CXXFLAGS="-std=c++11"
AC_MSG_CHECKING([whether CXX supports -std=c++11])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_CPP11FLAG="-std=c++11" ], [AC_MSG_RESULT([no])])
CXXFLAGS="$saved_cxxflags"

CXXFLAGS="-stdlib=libc++"
AC_MSG_CHECKING([whether CXX supports -stdlib=libc++])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <cstdio>]])], [AC_MSG_RESULT([yes]); CPPUTEST_CPP11FLAG+=" -stdlib=libc++" ], [AC_MSG_RESULT([no])])
CXXFLAGS="$saved_cxxflags"

AC_LANG_POP

### Checking for strange exception handling behavior, related to bugs in compilers...
### TODO: This part isn't finished yet. It will need to generate different CppUTestConfig.h files based on the result of these checks.
AC_LANG_PUSH([C++])

# Can we use operator delete without exception handling specifier? (clang warns on this!)
CXXFLAGS="-Werror"
AC_MSG_CHECKING([whether CXX supports operator delete without exception handling specifier])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([#include <new>\nvoid operator delete(void* mem);])], [AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no]); UT_DELETE_MUST_HAVE_EXCEPTION_SPECIFIER="yes"])
CXXFLAGS="$saved_cxxflags"

# Can we use operator new with exception specifier (g++4.7 on MacOSX is broken here)
CXXFLAGS="-Werror"
AC_MSG_CHECKING([whether CXX supports operator new with exception handling specifier])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([#include <new>\nvoid* operator new(size_t size) throw(std::bad_alloc);;])], [AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no]); UT_NEW_CANT_HAVE_EXCEPTION_SPECIFIER="yes"])
CXXFLAGS="$saved_cxxflags"

AC_LANG_POP

##### Linker checking. 
# 
# TBD!
# Things that need to be fixed!
#
# The below code is checking for the -Qunused-arguments which is a linker flag. However, it says gcc supports it, while in fact, it doesn't.
# As a workaround, we'll just check whether it is clang hardcoded, this is not in the automake spirit and will need to be fixed.
#
# LDFLAGS=
# AC_MSG_CHECKING([whether LD supports -Qunused-arguments])
# AC_LINK_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_NO_UNUSED_ARGUMENT_WARNING+=" -Qunused-arguments" ], [AC_MSG_RESULT([no])])
# LDFLAGS="$saved_ldflags"

AC_MSG_CHECKING([whether CXXLD supports -Qunused-arguments linker option])
OUTPUT_WHEN_CLANG_COMPILER=`${CXX} --version | grep clang`
AM_CONDITIONAL([TEST_COMPILER_IS_CLANG], [ ! test -z "$OUTPUT_WHEN_CLANG_COMPILER]")
AM_COND_IF([TEST_COMPILER_IS_CLANG],
                [AC_MSG_RESULT([yes]); CPPUTEST_NO_UNUSED_ARGUMENT_WARNING+=" -Qunused-arguments"], 
                [AC_MSG_RESULT([no])]; CPPUTEST_NO_UNUSED_ARGUMENT_WARNING+=" ")

# Checking for options for creating map files
LDFLAGS=" -Wl,-map,filename.map.txt"
AC_MSG_CHECKING([whether LD supports -Wl,-map])
AC_LINK_IFELSE([AC_LANG_PROGRAM([])], [AC_MSG_RESULT([yes]); CPPUTEST_LD_MAP_GENERATION+=" -Wl,-map,filename.map.txt" ], [AC_MSG_RESULT([no])])
LDFLAGS="$saved_ldflags"


# Different features
AC_ARG_ENABLE([std-c], [AC_HELP_STRING([--disable-std-c], [disable the use of Standard C Library (warning: requires implementing Platforms/GccNoStdC) ])], 
   [use_std_c=${enableval}], [use_std_c=yes])

AC_ARG_ENABLE([std-cpp], [AC_HELP_STRING([--disable-std-cpp], [disable the use of Standard C++ Library])], 
   [use_std_cpp=${enableval}], [use_std_cpp=yes])

AC_ARG_ENABLE([std-cpp11], [AC_HELP_STRING([--enable-std-cpp11], [enable the use of Standard C++ 11 (if the compiler supports that)])], 
   [use_std_cpp11=${enableval}], [coverage=no])

AC_ARG_ENABLE([cpputest-flags], [AC_HELP_STRING([--disable-cpputest-flags], [disable CFLAGS/CPPFLAGS/CXXFLAGS set by CppUTest])], 
   [cpputest_flags=${enableval}], [cpputest_flags=yes])

AC_ARG_ENABLE([memory-leak-detection], [AC_HELP_STRING([--disable-memory-leak-detection], [disable memory leak detection])], 
   [memory_leak_detection=${enableval}], [memory_leak_detection=yes])

AC_ARG_ENABLE([extensions], [AC_HELP_STRING([--disable-extensions], [disable CppUTest extension library])], 
   [cpputest_ext=${enableval}], [cpputest_ext=yes])

AC_ARG_ENABLE([generate-map-file], [AC_HELP_STRING([--enable-generate-map-file], [enable the creation of a map file])], 
   [generate_map_file=${enableval}], [generate_map_file=no])

AC_ARG_ENABLE([coverage], [AC_HELP_STRING([--enable-coverage], [enable running with coverage])], 
   [coverage=${enableval}], [coverage=no])

AC_ARG_ENABLE([real-gtest], [AC_HELP_STRING([--enable-real-gtest], [enable using real GTest rather than faking it. Requires GTEST_HOME to be set])], 
   [real_gtest=${enableval}], [real_gtest=no])

AC_ARG_ENABLE([gmock], [AC_HELP_STRING([--enable-gmock], [enable using GMock. Requires GMOCK_HOME to be set])], 
   [use_gmock=${enableval}], [use_gmock=no])

############################## Setting options ###############################

AM_CONDITIONAL([INCLUDE_CPPUTEST_EXT], [test "x${cpputest_ext}" = xyes]) 

# Dealing with not having a Standard C library... (usually for Kernel development)
if test "x${use_std_c}" = xno; then
	use_std_cpp=no
	memory_leak_detection=no
    CPPUTEST_CPPFLAGS+=" -nostdinc"
    CPPUTEST_LDFLAGS+=" -nostdlib"
    AC_DEFINE([CPPUTEST_STD_C_LIB_DISABLED], [1], [Standard C library disabled])
    CPP_PLATFORM="GccNoStdC"
else
	CPP_PLATFORM="Gcc"
fi

# Using standard C++
if test "x${use_std_cpp}" = xno; then
	AC_DEFINE([CPPUTEST_STD_CPP_LIB_DISABLED], 1, [Standard C++ library disabled])
	if test "x${use_std_c}" = xyes; then
    	CPPUTEST_CXXFLAGS+=" -nostdinc++"
    	
    	# Since automake passes the CXXFLAGS to the linker, this will cause warnings with clang 3.2 (which become errors)
    	CPPUTEST_LDFLAGS+=$CPPUTEST_NO_UNUSED_ARGUMENT_WARNING
    	
    fi
fi

# Using the C++11 standard?
if test "x${use_std_cpp11}" = xyes; then
	CPPUTEST_CXXFLAGS+=" ${CPPUTEST_CPP11FLAG}"
fi


# Dealing with memory leak detection
if test "x${memory_leak_detection}" = xno; then
	AC_DEFINE([CPPUTEST_MEM_LEAK_DETECTION_DISABLED], 1, [memory leak detection disabled])
else
	CPPUTEST_CXXFLAGS+=" -include ${srcdir}/include/CppUTest/MemoryLeakDetectorNewMacros.h"
	CPPUTEST_CPPFLAGS+=" -include ${srcdir}/include/CppUTest/MemoryLeakDetectorMallocMacros.h"
fi

# Generating map files.
if test "x${generate_map_file}" = xyes; then
	CPPUTEST_LDFLAGS+=$CPPUTEST_LD_MAP_GENERATION
	MOSTLYCLEANFILES+=" *.map.txt"
fi

if test "x${coverage}" = xyes; then
	CPPUTEST_CXXFLAGS+=" --coverage"
	CPPUTEST_CFLAGS+=" --coverage"
	MOSTLYCLEANFILES+=" *.gcda *.gcno"
fi

if test "x${use_gmock}" = xyes; then
	AC_ARG_VAR([GMOCK_HOME], [Location of the GMock])
	if test -z ${GMOCK_HOME}; then 
		AC_MSG_ERROR([
-------------------------------------
Compiling with --enable-gmock will require the GMOCK_HOME
to be set.
-------------------------------------])
	fi
	real_gtest=yes
	GTEST_HOME=${GMOCK_HOME}/gtest
	CPPUTEST_CPPFLAGS+=" -I${GMOCK_HOME}/include"
	if test -e ${GMOCK_HOME}/lib/libgmock.la; then \
		CPPUTEST_LDADD+=" ${GMOCK_HOME}/lib/libgmock.la"; \
	elif test -e ${GMOCK_HOME}/libgmock.a; then \
		CPPUTEST_LDADD+=" ${GMOCK_HOME}/libgmock.a"; \
	else \
		AC_MSG_ERROR([
-------------------------------------
GMOCK_HOME was set, but couldn't find the compiled library.
Did you compile it?
-------------------------------------]);
	fi	
	AC_DEFINE([CPPUTEST_USE_REAL_GMOCK], [1], [Use GMock])
else
	CPPUTEST_CPPFLAGS+=" -I${srcdir}/include/CppUTestExt/CppUTestGMock"
fi

if test "x${real_gtest}" = xyes; then
	AC_ARG_VAR([GTEST_HOME], [Location of the GTest source])
	if test -z ${GTEST_HOME}; then 
		AC_MSG_ERROR([
-------------------------------------
Compiling with --enable-real-gtest requires you to 
set the GTEST_HOME to the GTest source directory
-------------------------------------])
	fi
	CPPUTEST_CPPFLAGS+=" -I${GTEST_HOME}/include -I${GTEST_HOME} -Isomething"

	if test -e ${GTEST_HOME}/lib/libgtest.la; then \
		CPPUTEST_LDADD+=" ${GTEST_HOME}/lib/libgtest.la"; \
	elif test -e ${GTEST_HOME}/libgtest.a; then \
		CPPUTEST_LDADD+=" ${GTEST_HOME}/libgtest.a"; \
	else \
		AC_MSG_ERROR([
-------------------------------------
GTEST_HOME was set, but couldn't find the compiled library.
Did you compile it?
-------------------------------------]);
	fi	
	AC_DEFINE([CPPUTEST_USE_REAL_GTEST], [1], [Using the real gtest])
	
	# Turn warnings off. Gtest doesn't like it!
	CPPUTEST_CWARNINGFLAGS=""
	CPPUTEST_CXXWARNINGFLAGS=""
else
	CPPUTEST_CPPFLAGS+=" -I${srcdir}/include/CppUTestExt/CppUTestGTest"
fi

CPPUTEST_CFLAGS+=" ${CPPUTEST_CWARNINGFLAGS}"
CPPUTEST_CXXFLAGS+=" ${CPPUTEST_CXXWARNINGFLAGS}"
CPPUTEST_CPPFLAGS+=" -I ${srcdir}/include ${CPPUTEST_CPPWARNINGFLAGS}"

if test "x${cpputest_flags}" = xno; then
	CPPUTEST_CFLAGS=""
	CPPUTEST_CXXFLAGS=""
	CPPUTEST_CPPFLAGS=""
fi


############################## Values ########################################

### All files in git
if test -e "${srcdir}/.git"; then
	ALL_FILES_IN_GIT="`git --git-dir=${srcdir}/.git ls-files | tr '[ \n]' ' '`"
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
AC_SUBST([CPPUTEST_LDADD])
AC_SUBST([CPPUTEST_LDFLAGS])

AC_SUBST([MOSTLYCLEANFILES])
AC_SUBST([CPPUTEST_HAS_GCC])
AC_SUBST([CPPUTEST_HAS_CLANG])
AC_SUBST([ALL_FILES_IN_GIT])

AC_DEFINE([CPPUTEST_COMPILATION], [1], [Compiling CppUTest itself])

LT_INIT
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo \
"----------------------------------------------------------------

${PACKAGE_NAME} Version ${PACKAGE_VERSION}

Current compiler options:
   CC:                                  ${CC}
   CXX:                                 ${CXX}
   LD:                                  ${LD}
   Default CFLAGS:                      ${CFLAGS}
   Default CXXFLAGS:                    ${CXXFLAGS}
   CppUTest CFLAGS:                     ${CPPUTEST_CFLAGS}
   CppUTest CXXFLAGS:                   ${CPPUTEST_CXXFLAGS}
   CppUTest CPPFLAGS:                   ${CPPUTEST_CPPFLAGS}
   CppUTest LDFLAGS:                    ${CPPUTEST_LDFLAGS}
   CppUTest LIB:                        ${LIBS}

Features configured in ${PACKAGE_NAME}:
   Memory Leak Detection:               ${memory_leak_detection}
   Compiling extensions:                ${cpputest_ext}
   Disable CppUTest compile/link flags: ${cpputest_flags}

   Using Standard C++ Library:          ${use_std_cpp}
   Using Standard C Library:            ${use_std_c}

   Generating map file:                 ${generate_map_file}
   Compiling w coverage info:           ${coverage}

   Use 'real' GTest:                    ${real_gtest}
   Able to use GMock:                   ${use_gmock}

      
----------------------------------------------------------------"
