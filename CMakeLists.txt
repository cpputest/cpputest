# 2.6.3 is needed for ctest support
# 3.1 is needed for target_sources
cmake_minimum_required(VERSION 3.1)

project(CppUTest)

set(CppUTest_version_major 4)
set(CppUTest_version_minor 0)

###############
# Conan support
###############
if (EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    message(STATUS "conan_basic_setup()")
    conan_basic_setup()
endif()

option(STD_C "Use the standard C library" ON)
option(STD_CPP "Use the standard C++ library" ON)
option(CPPUTEST_FLAGS "Use the CFLAGS/CXXFLAGS/LDFLAGS set by CppUTest" ON)
option(MEMORY_LEAK_DETECTION "Enable memory leak detection" ON)
option(EXTENSIONS "Use the CppUTest extension library" ON)
option(LONGLONG "Support long long" OFF)
option(MAP_FILE "Enable the creation of a map file" OFF)
option(COVERAGE "Enable running with coverage" OFF)
option(C++11 "Compile with C++11 support" OFF)
option(WERROR "Compile with warnings as errors" OFF)

option(TESTS "Compile and make tests for the code?" ON)
option(TESTS_DETAILED "Run each test separately instead of grouped?" OFF)
option(TESTS_BUILD_DISCOVER "Build time test discover" ON)

option(EXAMPLES "Compile and make examples?" OFF)

option(VERBOSE_CONFIG "Print configuration to stdout during generation" ON)

option(LIBNAME_POSTFIX_BITSIZE "Add architecture bitsize (32/64) to the library name?" OFF)
option(LIBNAME_POSTFIX_DEBUG "Add indication of debug compilation to the library name?" OFF)

option(HAS_INF "Compiler has Inf value for float" ON)
option(HAS_NAN "Compiler has NaN value for float" ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "What kind of build this is" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set( CppUTestLibName "CppUTest" )
set( CppUTestExtLibName "CppUTestExt" )

if(LIBNAME_POSTFIX_BITSIZE)
  if( "${CMAKE_SIZEOF_VOID_P}" STREQUAL "8" )
      set( CppUTestLibName "${CppUTestLibName}64" )
      set( CppUTestExtLibName "${CppUTestExtLibName}64" )
  elseif( "${CMAKE_SIZEOF_VOID_P}" STREQUAL "4" )
      set( CppUTestLibName "${CppUTestLibName}32" )
      set( CppUTestExtLibName "${CppUTestExtLibName}32" )
  endif()
endif(LIBNAME_POSTFIX_BITSIZE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/Modules)

include("${PROJECT_SOURCE_DIR}/cmake/Modules/CppUTestConfigurationOptions.cmake")
include(CTest)
#include("${PROJECT_SOURCE_DIR}/cmake/Modules/CheckFunctionExists.cmake")
include("${PROJECT_SOURCE_DIR}/cmake/Modules/CppUTestBuildTimeDiscoverTests.cmake")
include("${PROJECT_SOURCE_DIR}/cmake/Modules/CppUTestNormalizeTestOutputLocation.cmake")

enable_testing()

add_subdirectory(src/CppUTest)

include(CheckIncludeFileCXX)
check_include_file_cxx("fenv.h" CPPUTEST_HAVE_FENV)

# Check for functions before setting a lot of stuff
include(CheckFunctionExists)
include(CheckSymbolExists)
set (CMAKE_REQUIRED_INCLUDES "unistd.h")
check_symbol_exists(fork "unistd.h" HAVE_FORK)
if(HAVE_FORK)
  target_compile_definitions(${CppUTestLibName}
    PUBLIC
      CPPUTEST_HAVE_FORK
  )
endif(HAVE_FORK)

check_symbol_exists(waitpid "sys/wait.h" HAVE_WAITPID)
if(HAVE_WAITPID)
  target_compile_definitions(${CppUTestLibName}
    PUBLIC
      CPPUTEST_HAVE_WAITPID
  )
endif(HAVE_WAITPID)

check_symbol_exists(gettimeofday "sys/time.h" HAVE_GETTIMEOFDAY)
if(HAVE_GETTIMEOFDAY)
  target_compile_definitions(${CppUTestLibName}
    PUBLIC
      CPPUTEST_HAVE_GETTIMEOFDAY=1
  )
endif(HAVE_GETTIMEOFDAY)

check_symbol_exists(pthread_mutex_lock "pthread.h" HAVE_PTHREAD_MUTEX_LOCK)
if(HAVE_PTHREAD_MUTEX_LOCK)
  target_compile_definitions(${CppUTestLibName}
    PUBLIC
      CPPUTEST_HAVE_PTHREAD_MUTEX_LOCK=1
  )
endif(HAVE_PTHREAD_MUTEX_LOCK)

if (NOT IAR)
  check_function_exists(strdup HAVE_STRDUP)
  if(HAVE_STRDUP)
    target_compile_definitions(${CppUTestLibName}
      PUBLIC
        CPPUTEST_HAVE_STRDUP=1
    )
  endif(HAVE_STRDUP)
endif(NOT IAR)

if (MINGW)
    # Apply workaround for MinGW timespec redefinition (pthread.h / time.h)
    include(CheckStructHasMember)
    check_struct_has_member("struct timespec" tv_sec time.h HAVE_STRUCT_TIMESPEC)
    if (HAVE_STRUCT_TIMESPEC)
      target_compile_definitions(${CppUTestLibName}
        PUBLIC
          _TIMESPEC_DEFINED=1
      )
    endif()

    if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        # Apply workaround for static/shared libraries on MinGW C/C++ compiler
        # Issue occurs with CMake >= 3.9.0, it doesn't filter out gcc,gcc_s,gcc_eh from
        # the implicit library list anymore, so the C++ linker is getting passed the static
        # gcc_eh library since that's what the C linker uses by default. Only solution appears
        # to be to force static linkage.
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    endif()
endif()

include(CheckTypeSize)
check_type_size("long long" SIZEOF_LONGLONG)
if(HAVE_SIZEOF_LONGLONG)
    set(CPPUTEST_HAVE_LONG_LONG_INT ON)
endif()

configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.cmake"
    "${PROJECT_BINARY_DIR}/generated/CppUTestGeneratedConfig.h"
    )
target_include_directories(${CppUTestLibName}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
)
target_compile_definitions(${CppUTestLibName}
  PUBLIC
    $<BUILD_INTERFACE:HAVE_CONFIG_H>
)

if (EXTENSIONS)
    add_subdirectory(src/CppUTestExt)
endif (EXTENSIONS)

if (TESTS)
    add_subdirectory(tests/CppUTest)
    if (EXTENSIONS)
        add_subdirectory(tests/CppUTestExt)
    endif (EXTENSIONS)
endif (TESTS)

if (EXAMPLES)
  add_subdirectory(examples)
endif(EXAMPLES)

if(VERBOSE_CONFIG)
  message("
-------------------------------------------------------
CppUTest Version ${CppUTest_version_major}.${CppUTest_version_minor}

Current compiler options:
    CC:                                 ${CMAKE_C_COMPILER}
    CXX:                                ${CMAKE_CXX_COMPILER}
    CppUTest CFLAGS:                    ${CPPUTEST_C_FLAGS}
    CppUTest CXXFLAGS:                  ${CPPUTEST_CXX_FLAGS}
    CppUTest LDFLAGS:                   ${CPPUTEST_LD_FLAGS}

Features configured in CppUTest:
    Memory Leak Detection:              ${MEMORY_LEAK_DETECTION}
    Compiling Extensions:               ${EXTENSIONS}
    Support Long Long:                  ${LONGLONG}
    Use CppUTest flags:                 ${CPPUTEST_FLAGS}

    Using Standard C library:           ${STD_C}
    Using Standard C++ library:         ${STD_CPP}
    Using C++11 library:                ${C++11}

    Generating map file:                ${MAP_FILE}
    Compiling with coverage:            ${COVERAGE}

    Compile and run self-tests          ${TESTS}
    Run self-tests separately           ${TESTS_DETAILED}

Library name options:
    Add architecture bitsize (32/64)    ${LIBNAME_POSTFIX_BITSIZE}
    Add debug compilation indicator     ${LIBNAME_POSTFIX_DEBUG}

-------------------------------------------------------
")
endif()
