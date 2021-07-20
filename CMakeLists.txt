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

# Check for functions before setting a lot of stuff
include(CheckFunctionExists)
set (CMAKE_REQUIRED_INCLUDES "unistd.h")
check_function_exists(fork HAVE_FORK)
if(HAVE_FORK)
  add_definitions(-DCPPUTEST_HAVE_FORK)
endif(HAVE_FORK)

check_function_exists(waitpid HAVE_WAITPID)
if(HAVE_WAITPID)
  add_definitions(-DCPPUTEST_HAVE_WAITPID)
endif(HAVE_WAITPID)

check_function_exists(gettimeofday HAVE_GETTIMEOFDAY)
if(HAVE_GETTIMEOFDAY)
  add_definitions(-DCPPUTEST_HAVE_GETTIMEOFDAY=1)
endif(HAVE_GETTIMEOFDAY)

check_function_exists(pthread_mutex_lock HAVE_PTHREAD_MUTEX_LOCK)
if(HAVE_PTHREAD_MUTEX_LOCK)
  add_definitions(-DCPPUTEST_HAVE_PTHREAD_MUTEX_LOCK=1)
endif(HAVE_PTHREAD_MUTEX_LOCK)

if (NOT IAR)
  check_function_exists(strdup HAVE_STRDUP)
  if(HAVE_STRDUP)
    add_definitions(-DCPPUTEST_HAVE_STRDUP=1)
  endif(HAVE_STRDUP)
endif(NOT IAR)

if (MINGW)
    # Apply workaround for MinGW timespec redefinition (pthread.h / time.h)
    include(CheckStructHasMember)
    check_struct_has_member("struct timespec" tv_sec time.h HAVE_STRUCT_TIMESPEC)
    if (HAVE_STRUCT_TIMESPEC)
        add_definitions(-D_TIMESPEC_DEFINED=1)
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
include(GNUInstallDirs)

enable_testing()

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

if (EXAMPLES)
  add_subdirectory(examples)
endif(EXAMPLES)

set (INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
set (LIB_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
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
  install(FILES  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Scripts/CppUTestBuildTimeDiscoverTests.cmake
    DESTINATION ${LIB_INSTALL_DIR}/CppUTest/cmake/Scripts)
  install(FILES  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/CppUTestBuildTimeDiscoverTests.cmake
    DESTINATION ${LIB_INSTALL_DIR}/CppUTest/cmake/Modules)
  configure_package_config_file(CppUTestConfig.cmake.build.in
    ${CMAKE_CURRENT_BINARY_DIR}/CppUTestConfig.cmake
    INSTALL_DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    PATH_VARS INCLUDE_DIR CMAKE_CURRENT_BINARY_DIR)
  if (EXTENSIONS)
    export(TARGETS CppUTest CppUTestExt
      FILE "${CMAKE_CURRENT_BINARY_DIR}/CppUTestTargets.cmake")
  else()
    export(TARGETS CppUTest
      FILE "${CMAKE_CURRENT_BINARY_DIR}/CppUTestTargets.cmake")
  endif()
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

-------------------------------------------------------
")
endif()
