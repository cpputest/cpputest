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

include(CheckCXXSourceCompiles)
check_cxx_source_compiles(
  "int main(int argc, char ** argv) { throw 20; }"
  CPPUTEST_HAVE_EXCEPTIONS
)

if((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") AND (CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC"))
  set(is_clang_cl TRUE)
endif()

cmake_dependent_option(CPPUTEST_STD_CPP_LIB_DISABLED "Use the standard C++ library"
  OFF "NOT CPPUTEST_STD_C_LIB_DISABLED;CPPUTEST_HAVE_EXCEPTIONS" ON)
option(CPPUTEST_FLAGS "Use the CFLAGS/CXXFLAGS/LDFLAGS set by CppUTest" ON)
cmake_dependent_option(CPPUTEST_MEM_LEAK_DETECTION_DISABLED "Enable memory leak detection"
  OFF "NOT BORLAND;NOT CPPUTEST_STD_C_LIB_DISABLED;NOT is_clang_cl" ON)
option(CPPUTEST_EXTENSIONS "Use the CppUTest extension library" ON)

include(CheckTypeSize)
check_type_size("long long" SIZEOF_LONGLONG)
cmake_dependent_option(CPPUTEST_USE_LONG_LONG "Support long long"
  YES "HAVE_SIZEOF_LONGLONG" OFF)

cmake_dependent_option(CPPUTEST_MAP_FILE "Enable the creation of a map file"
  OFF "NOT MSVC" OFF)
cmake_dependent_option(CPPUTEST_COVERAGE "Enable running with coverage"
  OFF "NOT MSVC" OFF)
cmake_dependent_option(CPPUTEST_WERROR
  "Compile with warnings as errors"
  ON "PROJECT_IS_TOP_LEVEL" OFF
)
cmake_dependent_option(CPPUTEST_BUILD_TESTING "Compile and make tests for CppUTest"
  ${PROJECT_IS_TOP_LEVEL} "BUILD_TESTING" OFF)
option(CPPUTEST_TESTS_DETAILED "Run each test separately instead of grouped?" OFF)
cmake_dependent_option(CPPUTEST_TEST_DISCOVERY "Build time test discover"
  ON "CPPUTEST_BUILD_TESTING;CMAKE_CROSSCOMPILING_EMULATOR OR NOT CMAKE_CROSSCOMPILING" OFF)
cmake_dependent_option(CPPUTEST_EXAMPLES "Compile and make examples?"
  ${PROJECT_IS_TOP_LEVEL} "NOT CPPUTEST_STD_CPP_LIB_DISABLED" OFF)
option(CPPUTEST_VERBOSE_CONFIG "Print configuration to stdout during generation" ${PROJECT_IS_TOP_LEVEL})

cmake_dependent_option(CPPUTEST_LIBNAME_POSTFIX_BITSIZE "Add architecture bitsize (32/64) to the library name?"
  OFF "PROJECT_IS_TOP_LEVEL" OFF)

if(DEFINED CPP_PLATFORM)
  message(DEPRECATION
    "The CPP_PLATFORM variable has been deprecated. "
    "Use CPPUTEST_PLATFORM instead."
  )
else()
  if(MSVC)
    set(CPP_PLATFORM VisualCpp)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "IAR")
    set(CPP_PLATFORM Iar)
  elseif(BORLAND)
    set(CPP_PLATFORM Borland)
  elseif(CPPUTEST_STD_C_LIB_DISABLED)
    set(CPP_PLATFORM GccNoStdC)
  else()
    set(CPP_PLATFORM Gcc)
  endif()
endif()
set(CPPUTEST_PLATFORM "${CPP_PLATFORM}" CACHE STRING "Platform implementation")
set_property(
  CACHE CPPUTEST_PLATFORM
  PROPERTY STRINGS
    armcc
    Borland
    C2000
    Dos
    Gcc
    GccNoStdC
    Iar
    Keil
    Symbian
    VisualCpp
    OFF
)

include(CheckCXXSymbolExists)
check_cxx_symbol_exists(fopen_s "stdio.h" CPPUTEST_HAVE_SECURE_STDLIB)
cmake_dependent_option(CPPUTEST_USE_SECURE_STDLIB "Use MSVC safe functions"
  ON "WIN32;CPPUTEST_HAVE_SECURE_STDLIB" OFF)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) # Don't change users' build type.
  # Multi-configuration generators don't have a single build type.
  get_property(
    GENERATOR_IS_MULTI_CONFIG GLOBAL
    PROPERTY GENERATOR_IS_MULTI_CONFIG
  )
  if(NOT GENERATOR_IS_MULTICONFIG AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "What kind of build this is" FORCE)
    message("No CMAKE_BUILD_TYPE set. Assigning: ${CMAKE_BUILD_TYPE}")
  endif()
endif()

set( CppUTestLibName "CppUTest" )
set( CppUTestExtLibName "CppUTestExt" )

if(CPPUTEST_LIBNAME_POSTFIX_BITSIZE)
  if( "${CMAKE_SIZEOF_VOID_P}" STREQUAL "8" )
      set( CppUTestLibName "${CppUTestLibName}64" )
      set( CppUTestExtLibName "${CppUTestExtLibName}64" )
  elseif( "${CMAKE_SIZEOF_VOID_P}" STREQUAL "4" )
      set( CppUTestLibName "${CppUTestLibName}32" )
      set( CppUTestExtLibName "${CppUTestExtLibName}32" )
  endif()
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
if(NOT PROJECT_IS_TOP_LEVEL)
  set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" PARENT_SCOPE)
endif()

include(CppUTestConfigurationOptions)
include(CppUTestBuildTimeDiscoverTests)
include(CppUTestNormalizeTestOutputLocation)
include(GNUInstallDirs)

add_subdirectory(src/CppUTest)

# Check for symbols before setting a lot of stuff
include(CheckTypeSize)
include(CheckStructHasMember)
set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS})
check_cxx_symbol_exists(FE_DIVBYZERO "fenv.h" CPPUTEST_HAVE_FENV)
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

if (MINGW)
    check_struct_has_member("struct timespec" tv_sec time.h CPPUTEST_HAVE_STRUCT_TIMESPEC LANGUAGE CXX)
    if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        # Apply workaround for static/shared libraries on MinGW C/C++ compiler
        # Issue occurs with CMake >= 3.9.0, it doesn't filter out gcc,gcc_s,gcc_eh from
        # the implicit library list anymore, so the C++ linker is getting passed the static
        # gcc_eh library since that's what the C linker uses by default. Only solution appears
        # to be to force static linkage.
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    endif()
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
    HAVE_CONFIG_H
)

if (CPPUTEST_EXTENSIONS)
    add_subdirectory(src/CppUTestExt)
endif ()

if (CPPUTEST_BUILD_TESTING)
    add_subdirectory(tests/CppUTest)
    if (CPPUTEST_EXTENSIONS)
        add_subdirectory(tests/CppUTestExt)
    endif ()
endif ()

if (CPPUTEST_EXAMPLES)
  add_subdirectory(examples)
endif()

if(PROJECT_IS_TOP_LEVEL)
  set (INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
  set (LIB_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
  set (INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

  # Pkg-config file
  set(CppUTest_PKGCONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/cpputest.pc)

  # Pkg-config file.
  set (prefix "${CMAKE_INSTALL_PREFIX}")
  set (exec_prefix "\${prefix}")
  set (libdir "\${exec_prefix}/${LIB_INSTALL_DIR}")
  set (includedir "\${prefix}/${INCLUDE_INSTALL_DIR}")
  set (PACKAGE_VERSION "${PROJECT_VERSION}")

  configure_file (cpputest.pc.in "${CppUTest_PKGCONFIG_FILE}" @ONLY)
  install(
    FILES "${CppUTest_PKGCONFIG_FILE}"
    DESTINATION ${LIB_INSTALL_DIR}/pkgconfig
  )

  install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/generated/CppUTestGeneratedConfig.h"
    DESTINATION "${INCLUDE_INSTALL_DIR}/generated"
  )

  # Try to include helper module
  include(CMakePackageConfigHelpers OPTIONAL
    RESULT_VARIABLE PkgHelpers_AVAILABLE)
  # guard against older versions of cmake which do not provide it
  if(PkgHelpers_AVAILABLE)
    set(CPPUTEST_CONFIG_DEST "${LIB_INSTALL_DIR}/CppUTest/cmake")

    configure_package_config_file(CppUTestConfig.cmake.install.in
      ${CMAKE_CURRENT_BINARY_DIR}/install/CppUTestConfig.cmake
      INSTALL_DESTINATION "${CPPUTEST_CONFIG_DEST}"
      PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR)
    write_basic_package_version_file(
      ${CMAKE_CURRENT_BINARY_DIR}/install/CppUTestConfigVersion.cmake
      VERSION ${PROJECT_VERSION}
      COMPATIBILITY SameMajorVersion )
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/install/CppUTestConfig.cmake
      ${CMAKE_CURRENT_BINARY_DIR}/install/CppUTestConfigVersion.cmake
      DESTINATION "${CPPUTEST_CONFIG_DEST}" )
    install(
      EXPORT CppUTestTargets
      NAMESPACE CppUTest::
      DESTINATION "${CPPUTEST_CONFIG_DEST}"
    )
    install(FILES  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Scripts/CppUTestBuildTimeDiscoverTests.cmake
      DESTINATION "${CPPUTEST_CONFIG_DEST}/Scripts")
    install(FILES  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/CppUTestBuildTimeDiscoverTests.cmake
      DESTINATION "${CPPUTEST_CONFIG_DEST}/Modules")
    configure_package_config_file(CppUTestConfig.cmake.build.in
      ${CMAKE_CURRENT_BINARY_DIR}/CppUTestConfig.cmake
      INSTALL_DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
      PATH_VARS INCLUDE_DIR CMAKE_CURRENT_BINARY_DIR)
    write_basic_package_version_file(
      ${CMAKE_CURRENT_BINARY_DIR}/CppUTestConfigVersion.cmake
      VERSION ${PROJECT_VERSION}
      COMPATIBILITY SameMajorVersion )
    set(CppUTest_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "The directory containing a CMake configuration file for CppUTest.")
  else()
    message("If you wish to use find_package(CppUTest) in your own project to find CppUTest library"
      " please update cmake to version which provides CMakePackageConfighelpers module"
      " or write generators for CppUTestConfig.cmake by yourself.")
  endif()
endif()

if(CPPUTEST_VERBOSE_CONFIG)
  message("
-------------------------------------------------------
CppUTest Version ${PROJECT_VERSION}

Current compiler options:
    CC:                                 ${CMAKE_C_COMPILER}
    CXX:                                ${CMAKE_CXX_COMPILER}
    CppUTest CFLAGS:                    ${CPPUTEST_C_FLAGS}
    CppUTest CXXFLAGS:                  ${CPPUTEST_CXX_FLAGS}
    CppUTest LDFLAGS:                   ${CPPUTEST_LD_FLAGS}

Features configured in CppUTest:
    Memory Leak Detection Disabled:     ${CPPUTEST_MEM_LEAK_DETECTION_DISABLED}
    Compiling Extensions:               ${CPPUTEST_EXTENSIONS}
    Support Long Long:                  ${CPPUTEST_USE_LONG_LONG}
    Use CppUTest flags:                 ${CPPUTEST_FLAGS}

    Disable Standard C library:         ${CPPUTEST_STD_C_LIB_DISABLED}
    Disable Standard C++ library:       ${CPPUTEST_STD_CPP_LIB_DISABLED}

    Generating map file:                ${CPPUTEST_MAP_FILE}
    Compiling with coverage:            ${CPPUTEST_COVERAGE}

    Compile and run self-tests          ${CPPUTEST_BUILD_TESTING}
    Run self-tests separately           ${CPPUTEST_TESTS_DETAILED}

Library name options:
    Add architecture bitsize (32/64)    ${CPPUTEST_LIBNAME_POSTFIX_BITSIZE}

-------------------------------------------------------
")
endif()
