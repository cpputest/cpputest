project(CppUTest)

set(CppUTest_version_major 3)
set(CppUTest_version_minor 4)

# 2.6.3 is needed for ctest support
cmake_minimum_required(VERSION 2.6.3)

option(STD_C "Use the standard C library" ON)
option(STD_CPP "Use the standard C++ library" ON)
option(CPPUTEST_FLAGS "Use the CFLAGS/CXXFLAGS/LDFLAGS set by CppUTest" ON)
option(MEMORY_LEAK_DETECTION "Enable memory leak detection" ON)
option(EXTENSIONS "Use the CppUTest extenstion library" ON)
option(MAP_FILE "Enable the creation of a map file" OFF)
option(COVERAGE "Enable running with coverage" OFF)
option(REAL_GTEST "Enable using real GTest rather than faking it" OFF)
option(GMOCK "Enable using GMock" OFF)

option(TESTS "Compile and make tests for the code?" ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "What kind of build this is" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(CppUTestRootDirectory ${PROJECT_SOURCE_DIR})

include("${CppUTestRootDirectory}/cmake/Modules/CppUTestConfigurationOptions.cmake")
include(CTest)

configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.cmake"
    "${PROJECT_BINARY_DIR}/config.h"
    )
include_directories(${PROJECT_BINARY_DIR})
add_definitions(-DHAVE_CONFIG_H)

include_directories(${CppUTestRootDirectory}/include)
add_subdirectory(src/CppUTest)

if (EXTENSIONS)
    add_subdirectory(src/CppUTestExt)
endif (EXTENSIONS)

if (TESTS)
    add_subdirectory(tests)
endif (TESTS)

message("
-------------------------------------------------------
CppUTest Version ${CppUTest_version_major}.${CppUTest_version_minor}

Current compiler options:
    CC:                                 ${CMAKE_C_COMPILER}
    CXX:                                ${CMAKE_CXX_COMPILER}
    CppUTest CFLAGS:                    ${CPPUTEST_C_FLAGS}
    CppUTest CXXFLAGS:                  ${CPPUTEST_CXX_FLAGS}
    CppUTest LDFLAGS:                   ${CPPUTEST_LD_FLAGS}

Features configure in CppUTest:
    Memory Leak Detection:              ${MEMORY_LEAK_DETECTION}
    Compiling Extensions:               ${EXTENSIONS}
    Use CppUTest flags:                 ${CPPUTEST_FLAGS}

    Using Standard C library:           ${STD_C}
    Using Standard C++ library:         ${STD_CPP}

    Generating map file:                ${MAP_FILE}
    Compiling with coverage:            ${COVERAGE}

    Use 'real' GTest:                   ${REAL_GTEST}
    Able to use GMock:                  ${GMOCK}
-------------------------------------------------------
")
