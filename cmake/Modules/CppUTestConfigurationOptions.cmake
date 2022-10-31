if (CPPUTEST_PLATFORM STREQUAL "IAR")
    unset(CMAKE_CXX_EXTENSION_COMPILE_OPTION)
    # Set up the CMake variables for the linker
    set(LINKER_SCRIPT "${CppUTest_SOURCE_DIR}/platforms/iar/CppUTestTest.icf")
    set(CMAKE_C_LINK_FLAGS "--semihosting --config ${LINKER_SCRIPT} --map mapfile.map")
    set(CMAKE_CXX_LINK_FLAGS "--semihosting --config ${LINKER_SCRIPT} --map mapfile.map")
elseif (CPPUTEST_PLATFORM STREQUAL "BORLAND")
    set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} -w-8008 -w-8066")
elseif (CPPUTEST_PLATFORM STREQUAL "GccNoStdC")
    set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} -nostdinc")
    set(CPPUTEST_LD_FLAGS "${CPPUTEST_LD_FLAGS} -nostdinc")
endif ()

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CppUTestWarningFlags)
endif ()

if (CPPUTEST_STD_CPP_LIB_DISABLED)
    if (NOT CPPUTEST_STD_C_LIB_DISABLED AND NOT MSVC)
        set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} -nostdinc++")
    endif ()
endif ()

if (NOT CPPUTEST_MEM_LEAK_DETECTION_DISABLED)
    if (MSVC)
        set(CPPUTEST_C_FLAGS "${CPPUTEST_C_FLAGS} /FI \"${CppUTest_SOURCE_DIR}/include/CppUTest/MemoryLeakDetectorMallocMacros.h\"")
        set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} /FI \"${CppUTest_SOURCE_DIR}/include/CppUTest/MemoryLeakDetectorMallocMacros.h\"")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "IAR")
        set(CPPUTEST_C_FLAGS "${CPPUTEST_C_FLAGS} --preinclude \"${CppUTest_SOURCE_DIR}/include/CppUTest/MemoryLeakDetectorMallocMacros.h\"")
        set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} --preinclude \"${CppUTest_SOURCE_DIR}/include/CppUTest/MemoryLeakDetectorNewMacros.h\"")
        set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} --preinclude \"${CppUTest_SOURCE_DIR}/include/CppUTest/MemoryLeakDetectorMallocMacros.h\"")
    else (MSVC)
        set(CPPUTEST_C_FLAGS "${CPPUTEST_C_FLAGS} -include \"${CppUTest_SOURCE_DIR}/include/CppUTest/MemoryLeakDetectorMallocMacros.h\"")
        set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} -include \"${CppUTest_SOURCE_DIR}/include/CppUTest/MemoryLeakDetectorNewMacros.h\"")
        set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} -include \"${CppUTest_SOURCE_DIR}/include/CppUTest/MemoryLeakDetectorMallocMacros.h\"")
    endif (MSVC)
endif ()

if (CPPUTEST_MAP_FILE AND NOT MSVC)
    set(CPPUTEST_LD_FLAGS "${CPPUTEST_LD_FLAGS} -Wl,-Map,$<.map.txt")
endif ()

if (CPPUTEST_COVERAGE AND NOT MSVC)
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
endif ()

set(CPPUTEST_C_FLAGS "${CPPUTEST_C_FLAGS} ${CPPUTEST_C_WARNING_FLAGS}")
set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} ${CPPUTEST_CXX_WARNING_FLAGS}")

if (CPPUTEST_FLAGS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CPPUTEST_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPPUTEST_CXX_FLAGS}")
    set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} ${CPPUTEST_LD_FLAGS}")
endif (CPPUTEST_FLAGS)
