project(CppUTest)

set(CppUTest_version_major 3)
set(CppUTest_version_minor 5)
set(CppUTest_version_patch 0)

# 2.6.3 is needed for ctest support
cmake_minimum_required(VERSION 2.6.3)

option(CPPUTEST_STD_C "Use the standard C library" ON)
option(CPPUTEST_STD_CPP "Use the standard C++ library" ON)
option(CPPUTEST_FLAGS "Use the CFLAGS/CXXFLAGS/LDFLAGS set by CppUTest" ON)
option(CPPUTEST_MEMORY_LEAK_DETECTION "Enable memory leak detection" ON)
option(CPPUTEST_EXTENSIONS "Use the CppUTest extenstion library" ON)
option(CPPUTEST_MAP_FILE "Enable the creation of a map file" OFF)
option(CPPUTEST_COVERAGE "Enable running with coverage" OFF)
option(CPPUTEST_C++11 "Compile with C++11 support" OFF)

option(CPPUTEST_TESTS "Compile and make tests for the code?" ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "What kind of build this is" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(CppUTestRootDirectory ${PROJECT_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CppUTestRootDirectory}/cmake/Modules)

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

if (CPPUTEST_EXTENSIONS)
    add_subdirectory(src/CppUTestExt)
endif (CPPUTEST_EXTENSIONS)

if (CPPUTEST_TESTS)
    add_subdirectory(tests)
endif (CPPUTEST_TESTS)

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
    Memory Leak Detection:              ${CPPUTEST_MEMORY_LEAK_DETECTION}
    Compiling Extensions:               ${CPPUTEST_EXTENSIONS}
    Use CppUTest flags:                 ${CPPUTEST_FLAGS}

    Using Standard C library:           ${CPPUTEST_STD_C}
    Using Standard C++ library:         ${CPPUTEST_STD_CPP}
    Using C++11 library:                ${CPPUTEST_C++11}

    Generating map file:                ${CPPUTEST_MAP_FILE}
    Compiling with coverage:            ${CPPUTEST_COVERAGE}

-------------------------------------------------------
")

#
# CMake support.
#

# Add all targets to the build-tree export set
if(CPPUTEST_EXTENSIONS)
    export(TARGETS CppUTest CppUTestExt FILE "${PROJECT_BINARY_DIR}/CppUTestTargets.cmake")
else()
    export(TARGETS CppUTest FILE "${PROJECT_BINARY_DIR}/CppUTestTargets.cmake")
endif()

# Create the CppUTestConfig.cmake and CppUTestConfigVersion files
#file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}" "${INSTALL_INCLUDE_DIR}")
# ... for the build tree
get_property(CONF_INCLUDE_DIRS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
configure_file(CppUTestConfig.cmake.in "${CMAKE_BINARY_DIR}/CppUTestConfig.cmake" @ONLY)

configure_file(CppUTestConfigVersion.cmake.in "${CMAKE_BINARY_DIR}/CppUTestConfigVersion.cmake" @ONLY)
