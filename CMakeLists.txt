project(CppUTest)

set(CppUTest_version_major 3)
set(CppUTest_version_minor 7.1)

# 2.6.3 is needed for ctest support
cmake_minimum_required(VERSION 2.6.3)

option(STD_C "Use the standard C library" ON)
option(STD_CPP "Use the standard C++ library" ON)
option(CPPUTEST_FLAGS "Use the CFLAGS/CXXFLAGS/LDFLAGS set by CppUTest" ON)
option(MEMORY_LEAK_DETECTION "Enable memory leak detection" ON)
option(EXTENSIONS "Use the CppUTest extenstion library" ON)
option(MAP_FILE "Enable the creation of a map file" OFF)
option(COVERAGE "Enable running with coverage" OFF)
option(C++11 "Compile with C++11 support" OFF)

option(TESTS "Compile and make tests for the code?" ON)
option(TESTS_DETAILED "Run each test separately instead of grouped?" OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "What kind of build this is" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# Pkg-config file
include(FindPkgConfig)
set(CppUTest_PKGCONFIG_FILE cpputest.pc)

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

if (EXTENSIONS)
    add_subdirectory(src/CppUTestExt)
endif (EXTENSIONS)

if (TESTS)
    add_subdirectory(tests)
endif (TESTS)

# Pkg-config file.
set (prefix "${CMAKE_INSTALL_PREFIX}")
set (exec_prefix "${CMAKE_INSTALL_PREFIX}")
set (libdir "${LIB_INSTALL_DIR}")
set (includedir "${INCLUDE_INSTALL_DIR}")
set (PACKAGE_VERSION "${CppUTest_version_major}.${CppUTest_version_minor}")

configure_file (cpputest.pc.in
    ${CMAKE_CURRENT_SOURCE_DIR}/${CppUTest_PKGCONFIG_FILE} @ONLY)

install(FILES  ${CMAKE_CURRENT_SOURCE_DIR}/${CppUTest_PKGCONFIG_FILE}
    DESTINATION ${LIB_INSTALL_DIR}/pkgconfig
    )

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
    Using C++11 library:                ${C++11}

    Generating map file:                ${MAP_FILE}
    Compiling with coverage:            ${COVERAGE}

-------------------------------------------------------
")
