if (MSVC)
    set(CPP_PLATFORM VisualCpp)
    include_directories(${CppUTestRootDirectory}/include/Platforms/${CPP_PLATFORM})
    option(STDC_WANT_SECURE_LIB "Use MSVC safe functions" ON)
    if(STDC_WANT_SECURE_LIB)
      ADD_DEFINITIONS(-DSTDC_WANT_SECURE_LIB)
    endif(STDC_WANT_SECURE_LIB)
elseif (IAR)
    set(CPP_PLATFORM Iar)
    unset(CMAKE_CXX_EXTENSION_COMPILE_OPTION)
    set(TESTS_BUILD_DISCOVER OFF)
    # Set up the CMake variables for the linker
    set(LINKER_SCRIPT "${CppUTestRootDirectory}/platforms/iar/CppUTestTest.icf")
    set(CMAKE_C_LINK_FLAGS "--semihosting --config ${LINKER_SCRIPT} --map mapfile.map")
    set(CMAKE_CXX_LINK_FLAGS "--semihosting --config ${LINKER_SCRIPT} --map mapfile.map")
elseif (STD_C)
    if(NOT CPP_PLATFORM)
        set(CPP_PLATFORM Gcc)
    endif(NOT CPP_PLATFORM)
else (MSVC)
    set(STD_CPP False)
    set(MEMORY_LEAK_DETECTION False)
    set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} -nostdinc")
    set(CPPUTEST_LD_FLAGS "${CPPUTEST_LD_FLAGS} -nostdinc")
    set(CPPUTEST_STD_C_LIB_DISABLED 1)
    set(CPP_PLATFORM GccNoStdC)
endif (MSVC)

include("${CppUTestRootDirectory}/cmake/Modules/CppUTestWarningFlags.cmake")

if (NOT STD_CPP)
    set(CPPUTEST_STD_CPP_LIB_DISABLED 1)
    if (STD_C AND NOT MSVC)
        set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} -nostdinc++")
    endif (STD_C AND NOT MSVC)
endif (NOT STD_CPP)

if (MEMORY_LEAK_DETECTION)
    if (MSVC)
        set(CPPUTEST_C_FLAGS "${CPPUTEST_C_FLAGS} /FI \"${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetectorMallocMacros.h\"")
        set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} /FI \"${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetectorMallocMacros.h\"")
    elseif (IAR)
        set(CPPUTEST_C_FLAGS "${CPPUTEST_C_FLAGS} --preinclude \"${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetectorMallocMacros.h\"")
        set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} --preinclude \"${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetectorNewMacros.h\"")
        set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} --preinclude \"${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetectorMallocMacros.h\"")
    else (MSVC)
        set(CPPUTEST_C_FLAGS "${CPPUTEST_C_FLAGS} -include \"${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetectorMallocMacros.h\"")
        set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} -include \"${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetectorNewMacros.h\"")
        set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} -include \"${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetectorMallocMacros.h\"")
    endif (MSVC)
else (MEMORY_LEAK_DETECTION)
    set(CPPUTEST_MEM_LEAK_DETECTION_DISABLED 1)
endif (MEMORY_LEAK_DETECTION)

if (LONGLONG)
    set(CPPUTEST_USE_LONG_LONG 1)
endif (LONGLONG)

if (MAP_FILE AND NOT MSVC)
    set(CPPUTEST_LD_FLAGS "${CPPUTEST_LD_FLAGS} -Wl,-Map,$<.map.txt")
endif (MAP_FILE AND NOT MSVC)

if (COVERAGE AND NOT MSVC)
    set(CPPUTEST_C_FLAGS "${CPPUTEST_C_FLAGS} --coverage")
    set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} --coverage")
    set(CMAKE_BUILD_TYPE "Debug")
    find_program(GCOVR gcovr DOC "gcovr executable")

    if (NOT GCOVR)
        message(SEND_ERROR "gcovr not found")
    endif()

    add_custom_target(coverage ${GCOVR}
        --root ${PROJECT_SOURCE_DIR}
        --output "${CMAKE_BINARY_DIR}/coverage/coverage.html"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Generate coverage data"
        VERBATIM
        )
endif()

if (CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_EXTENSIONS OFF)
elseif (C++11)
    find_package(CXX11 REQUIRED)
    set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} ${CXX11_FLAGS}")
else()
    # No standard specified
endif ()

set(GMOCK_HOME $ENV{GMOCK_HOME})
if (DEFINED ENV{GMOCK_HOME})
    # GMock pulls in gtest.
    set(CPPUTEST_INCLUDE_GTEST_TESTS 1)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTEST_USE_OWN_TR1_TUPLE=1")
    include_directories(${GMOCK_HOME}/include ${GMOCK_HOME}/gtest ${GMOCK_HOME}/gtest/include)
    add_subdirectory(${GMOCK_HOME} "${CMAKE_CURRENT_BINARY_DIR}/gmock")

    set(CPPUTEST_C_WARNING_FLAGS "")
    set(CPPUTEST_CXX_WARNING_FLAGS "")
endif (DEFINED ENV{GMOCK_HOME})

set(CPPUTEST_C_FLAGS "${CPPUTEST_C_FLAGS} ${CPPUTEST_C_WARNING_FLAGS}")
set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} ${CPPUTEST_CXX_WARNING_FLAGS}")

if (CPPUTEST_FLAGS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CPPUTEST_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPPUTEST_CXX_FLAGS}")
    set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} ${CPPUTEST_LD_FLAGS}")
endif (CPPUTEST_FLAGS)
