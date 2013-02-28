project(CppUTest)

set(CppUTest_version_major 3)
set(CppUTest_version_minor 3)

# 2.6.3 is needed for ctest support
cmake_minimum_required(VERSION 2.6.3)

option(STD_C "Disable the use of standard C library" TRUE)
option(STD_CPP "Disable the use of standard C++ library" TRUE)
option(CPPUTEST_FLAGS "Disable CFLAGS/CPPFLAGS/CXXFLAGS set by CppUTest" TRUE)
option(MEMORY_LEAK_DETECTION "Disable memory leak detection" TRUE)
option(EXTENSIONS "Disable CppUTest extenstion library" TRUE)
option(MAP_FILE "Enable the creation of a map file" FALSE)
option(COVERAGE "Enable running with coverage" FALSE)
option(REAL_GTEST "Enable using real GTest rather than faking it" FALSE)
option(GMOCK "Enable using GMock" FALSE)

option(TESTS "Compile and make tests for the code?" TRUE)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules" CACHE STRING "Path to cmake modules")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "What kind of build this is" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(CppUTestRootDirectory ${PROJECT_SOURCE_DIR})

set(CPP_PLATFORM Gcc)

if (NOT STD_C)
    set(STD_CPP FALSE)
    set(MEMORY_LEAK_DETECTION FALSE)
    message("Standard C library disable")
    set(CPP_PLATFORM GccNoStdC)
endif (NOT STD_C)

include(CTest)

include_directories(${CppUTestRootDirectory}/include)
add_subdirectory(src/CppUTest)

if (EXTENSIONS)
    add_subdirectory(src/CppUTestExt)
endif (EXTENSIONS)

if (TESTS)
    add_subdirectory(tests)
endif (TESTS)
