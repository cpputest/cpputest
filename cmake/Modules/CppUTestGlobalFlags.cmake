if (CMAKE_CXX_COMPILER_ID STREQUAL "IAR")
    # Set up the CMake variables for the linker
    set(LINKER_SCRIPT "${CppUTestRootDirectory}/platforms/iar/CppUTestTest.icf")
    set(CPPUTEST_LD_FLAGS "--semihosting --config ${LINKER_SCRIPT} --map mapfile.map")
elseif (NOT STD_C)
    set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} -nostdinc")
    set(CPPUTEST_LD_FLAGS "${CPPUTEST_LD_FLAGS} -nostdinc")
endif ()

if(STDC_WANT_SECURE_LIB)
    add_definitions(-DSTDC_WANT_SECURE_LIB)
endif()

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include("${CppUTestRootDirectory}/cmake/Modules/CppUTestWarningFlags.cmake")
endif ()

if(
    CPPUTEST_STD_CPP_LIB_DISABLED
    AND NOT CPPUTEST_STD_C_LIB_DISABLED
    AND NOT MSVC
)
    set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} -nostdinc++")
endif()

if (MINGW AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Apply workaround for static/shared libraries on MinGW C/C++ compiler
    # Issue occurs with CMake >= 3.9.0, it doesn't filter out gcc,gcc_s,gcc_eh from
    # the implicit library list anymore, so the C++ linker is getting passed the static
    # gcc_eh library since that's what the C linker uses by default. Only solution appears
    # to be to force static linkage.
    set(CPPUTEST_LD_FLAGS "${CPPUTEST_LD_FLAGS} -static")
endif()

if(MEMORY_LEAK_DETECTION)
    if(MSVC)
        set(CPPUTEST_C_FLAGS "${CPPUTEST_C_FLAGS} /FI \"${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetectorMallocMacros.h\"")
        set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} /FI \"${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetectorMallocMacros.h\"")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "IAR")
        set(CPPUTEST_C_FLAGS "${CPPUTEST_C_FLAGS} --preinclude \"${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetectorMallocMacros.h\"")
        set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} --preinclude \"${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetectorNewMacros.h\"")
        set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} --preinclude \"${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetectorMallocMacros.h\"")
    else()
        set(CPPUTEST_C_FLAGS "${CPPUTEST_C_FLAGS} -include \"${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetectorMallocMacros.h\"")
        set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} -include \"${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetectorNewMacros.h\"")
        set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} -include \"${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetectorMallocMacros.h\"")
    endif()
endif()

if(MAP_FILE AND NOT MSVC)
    set(CPPUTEST_LD_FLAGS "${CPPUTEST_LD_FLAGS} -Wl,-Map,$<.map.txt")
endif()

if(COVERAGE AND NOT MSVC)
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
else()
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "What kind of build this is" FORCE)
endif()

if (CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_EXTENSIONS OFF)
elseif (CPPUTEST_USE_CXX11)
    find_package(CXX11 REQUIRED)
    set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} ${CXX11_FLAGS}")
else()
    # No standard specified
endif ()

set(GMOCK_HOME $ENV{GMOCK_HOME})
if (DEFINED ENV{GMOCK_HOME})
    # GMock pulls in gtest.
    set(CPPUTEST_INCLUDE_GTEST_TESTS 1)
    set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} -DGTEST_USE_OWN_TR1_TUPLE=1")
    include_directories(${GMOCK_HOME}/include ${GMOCK_HOME}/gtest ${GMOCK_HOME}/gtest/include)
    add_subdirectory(${GMOCK_HOME} "${CMAKE_CURRENT_BINARY_DIR}/gmock")

    set(CPPUTEST_C_WARNING_FLAGS "")
    set(CPPUTEST_CXX_WARNING_FLAGS "")
endif (DEFINED ENV{GMOCK_HOME})

set(CPPUTEST_C_FLAGS "${CPPUTEST_C_FLAGS} ${CPPUTEST_C_WARNING_FLAGS}")
set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} ${CPPUTEST_CXX_WARNING_FLAGS}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CPPUTEST_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPPUTEST_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CPPUTEST_LD_FLAGS}")
