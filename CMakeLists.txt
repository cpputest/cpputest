# 2.6.3 is needed for ctest support
# 3.1 is needed for target_sources
# 3.8 is needed for try_compile improvements (CMP0067)
cmake_minimum_required(VERSION 3.8)

project(CppUTest
  VERSION 4.0
)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.21")
  # PROJECT_IS_TOP_LEVEL is defined automatically
elseif (CMAKE_CURRENT_BINARY_DIR STREQUAL CMAKE_BINARY_DIR)
  set(PROJECT_IS_TOP_LEVEL TRUE)
else()
  set(PROJECT_IS_TOP_LEVEL FALSE)
endif()

###############
# Conan support
###############
if (EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    message(STATUS "conan_basic_setup()")
    conan_basic_setup()
endif()

include(CTest)

include(CMakeDependentOption)
option(CPPUTEST_STD_C_LIB_DISABLED "Disable the standard C library")

cmake_dependent_option(CPPUTEST_STD_CPP_LIB_DISABLED "Use the standard C++ library"
  OFF "NOT CPPUTEST_STD_C_LIB_DISABLED" ON)
cmake_dependent_option(CPPUTEST_MEM_LEAK_DETECTION_DISABLED "Enable memory leak detection"
  OFF "NOT BORLAND;NOT CPPUTEST_STD_C_LIB_DISABLED" ON)
option(CPPUTEST_EXTENSIONS "Use the CppUTest extension library" ON)
include(CheckTypeSize)
check_type_size("long long" SIZEOF_LONGLONG)
cmake_dependent_option(CPPUTEST_USE_LONG_LONG "Support long long"
  YES "HAVE_SIZEOF_LONGLONG" OFF)

cmake_dependent_option(CPPUTEST_BUILD_TESTING "Compile and make tests for CppUTest"
  ${PROJECT_IS_TOP_LEVEL} "BUILD_TESTING" OFF)
cmake_dependent_option(CPPUTEST_SPLIT_TESTS "Split tests into small executables"
  OFF "CPPUTEST_BUILD_TESTING" OFF)
cmake_dependent_option(CPPUTEST_TEST_DISCOVERY "Build time test discover"
  ON "CPPUTEST_BUILD_TESTING;CMAKE_CROSSCOMPILING_EMULATOR OR NOT CMAKE_CROSSCOMPILING" OFF)
cmake_dependent_option(CPPUTEST_TEST_GTEST "Test GoogleTest integration"
  OFF "CPPUTEST_BUILD_TESTING" OFF)
cmake_dependent_option(CPPUTEST_EXAMPLES "Compile and make examples?"
  ${PROJECT_IS_TOP_LEVEL} "CPPUTEST_EXTENSIONS;NOT CPPUTEST_STD_CPP_LIB_DISABLED" OFF)

if(NOT DEFINED CPPUTEST_PLATFORM)
  if(DEFINED CPP_PLATFORM)
    message(DEPRECATION
      "The CPP_PLATFORM variable has been deprecated. "
      "Use CPPUTEST_PLATFORM instead."
    )
    set(CPPUTEST_PLATFORM ${CPP_PLATFORM})
  else()
    if(CPPUTEST_STD_C_LIB_DISABLED)
      set(CPPUTEST_PLATFORM OFF)
    elseif(MSVC)
      set(CPPUTEST_PLATFORM VisualCpp)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "IAR")
      set(CPPUTEST_PLATFORM Iar)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "ARMCC")
      set(CPPUTEST_PLATFORM armcc)
    elseif(DOS)
      set(CPPUTEST_PLATFORM Dos)
    elseif(BORLAND)
      set(CPPUTEST_PLATFORM Borland)
    else()
      set(CPPUTEST_PLATFORM Gcc)
    endif()
  endif()
  message(STATUS "Setting CPPUTEST_PLATFORM: ${CPPUTEST_PLATFORM}")
endif()
set(CPPUTEST_PLATFORM "${CPPUTEST_PLATFORM}" CACHE STRING "Platform implementation")
set_property(
  CACHE CPPUTEST_PLATFORM
  PROPERTY STRINGS
    armcc Borland C2000 Dos Gcc Iar Keil Symbian VisualCpp OFF
)

include(CheckCXXSymbolExists)
check_cxx_symbol_exists(fopen_s "stdio.h" CPPUTEST_HAVE_SECURE_STDLIB)
cmake_dependent_option(CPPUTEST_USE_SECURE_STDLIB "Use MSVC safe functions"
  ON "WIN32;CPPUTEST_HAVE_SECURE_STDLIB" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
if(NOT PROJECT_IS_TOP_LEVEL)
  set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" PARENT_SCOPE)
endif()

if(PROJECT_IS_TOP_LEVEL)
  include(cmake/warnings.cmake)
endif()
include(GNUInstallDirs)

# Check for symbols before setting a lot of stuff
include(CheckTypeSize)
include(CheckStructHasMember)
set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS})
check_cxx_symbol_exists(fork "unistd.h" CPPUTEST_HAVE_FORK)
check_cxx_symbol_exists(waitpid "sys/wait.h" CPPUTEST_HAVE_WAITPID)
check_cxx_symbol_exists(gettimeofday "sys/time.h" CPPUTEST_HAVE_GETTIMEOFDAY)
check_cxx_symbol_exists(pthread_mutex_lock "pthread.h" CPPUTEST_HAVE_PTHREAD_MUTEX_LOCK)

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "IAR")
  check_cxx_symbol_exists(strdup "string.h" CPPUTEST_HAVE_STRDUP)
endif()
check_type_size("long long" SIZEOF_LONGLONG LANGUAGE CXX)
if(HAVE_SIZEOF_LONGLONG)
    set(CPPUTEST_HAVE_LONG_LONG_INT ON)
endif()

configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.cmake"
    "${PROJECT_BINARY_DIR}/generated/CppUTestGeneratedConfig.h"
)

add_subdirectory(src)

target_include_directories(CppUTest
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
)
target_compile_definitions(CppUTest
  PUBLIC
    HAVE_CONFIG_H
)

if (CPPUTEST_BUILD_TESTING)
    add_subdirectory(tests)
endif ()

if (CPPUTEST_EXAMPLES)
  add_subdirectory(examples)
endif()

if(PROJECT_IS_TOP_LEVEL)
  include(cmake/install.cmake)
endif()
