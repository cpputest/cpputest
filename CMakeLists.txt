# 2.6.3 is needed for ctest support
# 3.1 is needed for target_sources
# 3.10 is needed for IAR support
cmake_minimum_required(VERSION 3.10)

project(CppUTest
    LANGUAGES CXX C
    VERSION 4.0
)

###############
# Conan support
###############
if (EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    message(STATUS "conan_basic_setup()")
    conan_basic_setup()
endif()

#############
# User config
#############

include(CMakeDependentOption)
option(STD_C "Use the standard C library" ON)
option(STD_CPP "Use the standard C++ library" ON)
option(CPPUTEST_FLAGS "Use the CFLAGS/CXXFLAGS/LDFLAGS set by CppUTest" ON)
cmake_dependent_option(MEMORY_LEAK_DETECTION "Enable memory leak detection" ON "STD_C;NOT BORLAND" OFF)
option(EXTENSIONS "Use the CppUTest extension library" ON)
cmake_dependent_option(LONGLONG "Support long long" OFF "NOT BORLAND" OFF)
option(MAP_FILE "Enable the creation of a map file" OFF)
option(COVERAGE "Enable running with coverage" OFF)
option(C++11 "Compile with C++11 support" OFF)
option(WERROR "Compile with warnings as errors" OFF)

option(TESTS "Compile and make tests for the code?" ON)
option(TESTS_DETAILED "Run each test separately instead of grouped?" OFF)
cmake_dependent_option(TESTS_BUILD_DISCOVER "Build time test discover" ON "NOT CMAKE_CXX_COMPILER_ID STREQUAL IAR" OFF)

option(EXAMPLES "Compile and make examples?" OFF)

option(VERBOSE_CONFIG "Print configuration to stdout during generation" ON)

option(LIBNAME_POSTFIX_BITSIZE "Add architecture bitsize (32/64) to the library name?" OFF)
option(LIBNAME_POSTFIX_DEBUG "Add indication of debug compilation to the library name?" OFF)

cmake_dependent_option(HAS_INF "Compiler has Inf value for float" ON "NOT BORLAND" OFF)
cmake_dependent_option(HAS_NAN "Compiler has NaN value for float" ON "NOT BORLAND" OFF)

cmake_dependent_option(STDC_WANT_SECURE_LIB "Use MSVC safe functions" ON "MSVC" OFF)

# Deduce a default CPP_PLATFORM.
if(NOT DEFINED CPP_PLATFORM)
    if(MSVC)
        set(CPP_PLATFORM "VisualCpp")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Symbian")
        set(CPP_PLATFORM "Symbian")
    elseif(BORLAND)
        set(CPP_PLATFORM "Borland")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "IAR")
        set(CPP_PLATFORM "Iar")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "DOS")
        set(CPP_PLATFORM "DOS")
    elseif(CMAKE_CXX_COMPILE_FEATURES STREQUAL "TI")
        # FIXME: This is probably wrong.
        set(CPP_PLATFORM  "C2000")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "ARMCC")
        set(CPP_PLATFORM "armcc")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "ARMClang")
        set(CPP_PLATFORM "Keil")
    elseif(NOT STD_C)
        set(CPP_PLATFORM "GccNoStdC")
    else()
        set(CPP_PLATFORM "Gcc")
    endif()
endif()

set(CPP_PLATFORM "${CPP_PLATFORM}" CACHE STRING "Provided platform implementation")
set_property(CACHE CPP_PLATFORM
    PROPERTY STRINGS
        # Valid options:
        "FALSE" # User provided
        "VisualCpp"
        "Symbian"
        "Borland"
        "Iar"
        "DOS"
        "C2000"
        "armcc"
        "Keil"
        "GccNoStdC"
        "Gcc"
)

# Pkg-config file
include(FindPkgConfig)
set(CppUTest_PKGCONFIG_FILE cpputest.pc)

set(CppUTestRootDirectory ${PROJECT_SOURCE_DIR})

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

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CppUTestRootDirectory}/cmake/Modules)

if(CPPUTEST_FLAGS)
    include("${CppUTestRootDirectory}/cmake/Modules/CppUTestConfigurationOptions.cmake")
endif()
include(CTest)
#include("${CppUTestRootDirectory}/cmake/Modules/CheckFunctionExists.cmake")
include("${CppUTestRootDirectory}/cmake/Modules/CppUTestBuildTimeDiscoverTests.cmake")
include("${CppUTestRootDirectory}/cmake/Modules/CppUTestNormalizeTestOutputLocation.cmake")
include(GNUInstallDirs)

enable_testing()

add_subdirectory(src/CppUTest)

include(CheckIncludeFileCXX)
check_include_file_cxx("fenv.h" CPPUTEST_HAVE_FENV)

include(CheckSymbolExists)
check_symbol_exists(fork "unistd.h" CPPUTEST_HAVE_FORK)
check_symbol_exists(waitpid "sys/wait.h" CPPUTEST_HAVE_WAITPID)
check_symbol_exists(gettimeofday "sys/time.h" CPPUTEST_HAVE_GETTIMEOFDAY)
check_symbol_exists(pthread_mutex_lock "pthread.h" CPPUTEST_HAVE_PTHREAD_MUTEX_LOCK)

if (CMAKE_CXX_COMPILER_ID STREQUAL "IAR")
    include(CheckFunctionExists)
    check_function_exists(strdup CPPUTEST_HAVE_STRDUP)
endif()

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
endif()

include(CheckTypeSize)
check_type_size("long long" SIZEOF_LONGLONG)
if(HAVE_SIZEOF_LONGLONG)
    set(CPPUTEST_HAVE_LONG_LONG_INT ON)
endif()

if(NOT STD_C)
    set(CPPUTEST_STD_C_LIB_DISABLED ON)
endif()

if(NOT STD_CPP)
    set(CPPUTEST_STD_CPP_LIB_DISABLED ON)
endif()

if(NOT MEMORY_LEAK_DETECTION)
    set(CPPUTEST_MEM_LEAK_DETECTION_DISABLED ON)
endif()

if(LONGLONG)
    set(CPPUTEST_USE_LONG_LONG ON)
endif()

if(NOT HAS_INF)
    set(CPPUTEST_NO_INF ON)
endif()

if(NOT HAS_NAN)
    set(CPPUTEST_NO_NAN ON)
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
  configure_package_config_file(CppUTestConfig.cmake.install.in
    ${CMAKE_CURRENT_BINARY_DIR}/install/CppUTestConfig.cmake
    INSTALL_DESTINATION ${LIB_INSTALL_DIR}/CppUTest/cmake
    PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR)
  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/install/CppUTestConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
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
    export(TARGETS ${CppUTestLibName} ${CppUTestExtLibName}
      FILE "${CMAKE_CURRENT_BINARY_DIR}/CppUTestTargets.cmake")
  else()
    export(TARGETS ${CppUTestLibName}
      FILE "${CMAKE_CURRENT_BINARY_DIR}/CppUTestTargets.cmake")
  endif()
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
    Platform:                           ${CPP_PLATFORM}
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
