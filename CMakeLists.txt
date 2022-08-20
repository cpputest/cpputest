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

include(CMakeDependentOption)
option(CPPUTEST_STD_C_LIB_DISABLED "Disable the standard C library")
cmake_dependent_option(CPPUTEST_STD_CPP_LIB_DISABLED "Use the standard C++ library"
  OFF "NOT CPPUTEST_STD_C_LIB_DISABLED" ON)
option(CPPUTEST_FLAGS "Use the CFLAGS/CXXFLAGS/LDFLAGS set by CppUTest" ON)
option(MEMORY_LEAK_DETECTION "Enable memory leak detection" ON)
option(EXTENSIONS "Use the CppUTest extension library" ON)
option(LONGLONG "Support long long" OFF)
option(MAP_FILE "Enable the creation of a map file" OFF)
option(COVERAGE "Enable running with coverage" OFF)
cmake_dependent_option(CPPUTEST_WERROR
  "Compile with warnings as errors"
  ON "PROJECT_IS_TOP_LEVEL" OFF
)

option(TESTS "Compile and make tests for the code?" ON)
option(TESTS_DETAILED "Run each test separately instead of grouped?" OFF)
option(TESTS_BUILD_DISCOVER "Build time test discover" ON)

option(EXAMPLES "Compile and make examples?" OFF)

option(VERBOSE_CONFIG "Print configuration to stdout during generation" ON)

option(LIBNAME_POSTFIX_BITSIZE "Add architecture bitsize (32/64) to the library name?" OFF)
option(LIBNAME_POSTFIX_DEBUG "Add indication of debug compilation to the library name?" OFF)

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

# Pkg-config file
include(FindPkgConfig)
set(CppUTest_PKGCONFIG_FILE cpputest.pc)

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

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
if(NOT PROJECT_IS_TOP_LEVEL)
  set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" PARENT_SCOPE)
endif()

include(CppUTestConfigurationOptions)
include(CTest)
include(CppUTestBuildTimeDiscoverTests)
include(CppUTestNormalizeTestOutputLocation)
include(GNUInstallDirs)

enable_testing()

add_subdirectory(src/CppUTest)

# Check for symbols before setting a lot of stuff
include(CheckCXXSymbolExists)
include(CheckTypeSize)
include(CheckStructHasMember)
set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS})
check_cxx_symbol_exists(FE_DIVBYZERO "fenv.h" CPPUTEST_HAVE_FENV)
check_cxx_symbol_exists(fork "unistd.h" CPPUTEST_HAVE_FORK)
check_cxx_symbol_exists(waitpid "sys/wait.h" CPPUTEST_HAVE_WAITPID)
check_cxx_symbol_exists(gettimeofday "sys/time.h" CPPUTEST_HAVE_GETTIMEOFDAY)
check_cxx_symbol_exists(pthread_mutex_lock "pthread.h" CPPUTEST_HAVE_PTHREAD_MUTEX_LOCK)

if(DEFINED HAS_NAN)
  message(DEPRECATION
    "The HAS_NAN cache variable has been deprecated. "
    "CPPUTEST_HAS_NAN is assessed automatically, "
    "but can be assigned manually."
  )
  set(CPPUTEST_HAS_NAN ${HAS_NAN})
endif()
if(NOT DEFINED CPPUTEST_HAS_NAN)
  check_cxx_symbol_exists(NAN "math.h" CPPUTEST_HAS_NAN)
endif()

if(DEFINED HAS_INF)
  message(DEPRECATION
    "The HAS_INF cache variable has been deprecated. "
    "CPPUTEST_HAS_INF is assessed automatically, "
    "but can be assigned manually."
  )
  set(CPPUTEST_HAS_INF ${HAS_INF})
endif()
if(NOT DEFINED CPPUTEST_HAS_INF)
  include(CheckCXXSymbolExists)
  check_cxx_symbol_exists(INFINITY "math.h" CPPUTEST_HAS_INF)
endif()

if (NOT IAR)
  check_cxx_symbol_exists(strdup "string.h" CPPUTEST_HAVE_STRDUP)
endif(NOT IAR)
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

if(PROJECT_IS_TOP_LEVEL)
  set (INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
  set (LIB_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
  set (INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

  # Pkg-config file.
  set (prefix "${CMAKE_INSTALL_PREFIX}")
  set (exec_prefix "\${prefix}")
  set (libdir "\${exec_prefix}/${LIB_INSTALL_DIR}")
  set (includedir "\${prefix}/${INCLUDE_INSTALL_DIR}")
  set (PACKAGE_VERSION "${PROJECT_VERSION}")

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

if(VERBOSE_CONFIG)
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
    Memory Leak Detection:              ${MEMORY_LEAK_DETECTION}
    Compiling Extensions:               ${EXTENSIONS}
    Support Long Long:                  ${LONGLONG}
    Use CppUTest flags:                 ${CPPUTEST_FLAGS}

    Disable Standard C library:         ${CPPUTEST_STD_C_LIB_DISABLED}
    Disable Standard C++ library:       ${CPPUTEST_STD_CPP_LIB_DISABLED}

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
