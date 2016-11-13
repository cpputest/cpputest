project(CppUTest)

set(CppUTest_version_major 3)
set(CppUTest_version_minor 8)

# 2.6.3 is needed for ctest support
cmake_minimum_required(VERSION 2.8.7)

# Check for functions before setting a lot of stuff
include(CheckFunctionExists)
set (CMAKE_REQUIRED_INCLUDES "unistd.h")
check_function_exists(fork HAVE_FORK)
if(HAVE_FORK)
  add_definitions(-DCPPUTEST_HAVE_FORK)
endif(HAVE_FORK)

check_function_exists(strdup HAVE_STRDUP)
if(HAVE_STRDUP)
  add_definitions(-DCPPUTEST_HAVE_STRDUP=1)
endif(HAVE_STRDUP)

# Apply workaround for MinGW timespec redefinition (pthread.h / time.h)
if (MINGW)
    include(CheckStructHasMember)
    check_struct_has_member("struct timespec" tv_sec time.h HAVE_STRUCT_TIMESPEC)
    if (HAVE_STRUCT_TIMESPEC)
        add_definitions(-D_TIMESPEC_DEFINED=1)
    endif()
endif()

option(STD_C "Use the standard C library" ON)
option(STD_CPP "Use the standard C++ library" ON)
option(CPPUTEST_FLAGS "Use the CFLAGS/CXXFLAGS/LDFLAGS set by CppUTest" ON)
option(MEMORY_LEAK_DETECTION "Enable memory leak detection" ON)
option(EXTENSIONS "Use the CppUTest extenstion library" ON)
option(LONGLONG "Support long long" OFF)
option(MAP_FILE "Enable the creation of a map file" OFF)
option(COVERAGE "Enable running with coverage" OFF)
option(C++11 "Compile with C++11 support" OFF)
option(WERROR "Compile with warnings as errors" OFF)

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
#include("${CppUTestRootDirectory}/cmake/Modules/CheckFunctionExists.cmake")
include("${CppUTestRootDirectory}/cmake/Modules/CppUTestBuildTimeDiscoverTests.cmake")
include("${CppUTestRootDirectory}/cmake/Modules/CppUTestNormalizeTestOutputLocation.cmake")

configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.cmake"
    "${PROJECT_BINARY_DIR}/generated/CppUTestGeneratedConfig.h"
    )
include_directories(${PROJECT_BINARY_DIR})
add_definitions(-DHAVE_CONFIG_H)

include_directories(${CppUTestRootDirectory}/include)
add_subdirectory(src/CppUTest)

if (EXTENSIONS)
    add_subdirectory(src/CppUTestExt)
endif (EXTENSIONS)

if (TESTS)
    add_subdirectory(tests/CppUTest)
    if (EXTENSIONS)
        add_subdirectory(tests/CppUTestExt)
    endif (EXTENSIONS)
endif (TESTS)

set (INCLUDE_INSTALL_DIR "include")
set (LIB_INSTALL_DIR "lib")
set (INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Pkg-config file.
set (prefix "${CMAKE_INSTALL_PREFIX}")
set (exec_prefix "\${prefix}")
set (libdir "\${exec_prefix}/${LIB_INSTALL_DIR}")
set (includedir "\${prefix}/${INCLUDE_INSTALL_DIR}")
set (PACKAGE_VERSION "${CppUTest_version_major}.${CppUTest_version_minor}")

configure_file (cpputest.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/${CppUTest_PKGCONFIG_FILE} @ONLY)

install(FILES  ${CMAKE_CURRENT_BINARY_DIR}/${CppUTest_PKGCONFIG_FILE}
    DESTINATION ${LIB_INSTALL_DIR}/pkgconfig
    )

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/generated/CppUTestGeneratedConfig.h"
    DESTINATION "${INCLUDE_INSTALL_DIR}/CppUTest"
    )

# Try to include helper module
include(CMakePackageConfigHelpers OPTIONAL
  RESULT_VARIABLE PkgHelpers_AVAILABLE)
# guard against older versions of cmake which do not provide it
if(PkgHelpers_AVAILABLE)
  configure_package_config_file(CppUTestConfig.cmake.install.in
    ${CMAKE_CURRENT_BINARY_DIR}/install/CppUTestConfig.cmake
    INSTALL_DESTINATION ${LIB_INSTALL_DIR}/CppUTest/cmake
    PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR)
  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/install/CppUTestConfigVersion.cmake
    VERSION ${CppUTest_version_major}.${CppUTest_version_minor}
    COMPATIBILITY SameMajorVersion )
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/install/CppUTestConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/install/CppUTestConfigVersion.cmake
    DESTINATION ${LIB_INSTALL_DIR}/CppUTest/cmake )
  install(EXPORT CppUTestTargets
    DESTINATION ${LIB_INSTALL_DIR}/CppUTest/cmake)
  configure_package_config_file(CppUTestConfig.cmake.build.in
    ${CMAKE_CURRENT_BINARY_DIR}/CppUTestConfig.cmake
    INSTALL_DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    PATH_VARS INCLUDE_DIR CMAKE_CURRENT_BINARY_DIR)
  export(TARGETS CppUTest CppUTestExt
    FILE "${CMAKE_CURRENT_BINARY_DIR}/CppUTestTargets.cmake")
  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/CppUTestConfigVersion.cmake
    VERSION ${CppUTest_version_major}.${CppUTest_version_minor}
    COMPATIBILITY SameMajorVersion )
  set(CppUTest_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "The directory containing a CMake configuration file for CppUTest.")
else()
  message("If you wish to use find_package(CppUTest) in your own project to find CppUTest library"
    " please update cmake to version which provides CMakePackageConfighelpers module"
    " or write generators for CppUTestConfig.cmake by yourself.")
endif()

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

-------------------------------------------------------
")
