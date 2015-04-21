enable_testing()

set(CppUTestTests_src
    AllTests.cpp
    SetPluginTest.cpp
    CheatSheetTest.cpp
    SimpleStringTest.cpp
    CommandLineArgumentsTest.cpp
    TestFailureTest.cpp
    TestFailureNaNTest.cpp
    CommandLineTestRunnerTest.cpp
    TestFilterTest.cpp
    TestHarness_cTest.cpp
    JUnitOutputTest.cpp
    TestHarness_cTestCFile.c
    MemoryLeakDetectorTest.cpp
    TestInstallerTest.cpp
    AllocLetTestFree.c
    MemoryLeakOperatorOverloadsTest.cpp
    TestMemoryAllocatorTest.cpp
    MemoryLeakWarningTest.cpp
    TestOutputTest.cpp
    AllocLetTestFreeTest.cpp
    TestRegistryTest.cpp
    AllocationInCFile.c
    PluginTest.cpp
    TestResultTest.cpp
    PreprocessorTest.cpp
    TestUTestMacro.cpp
    AllocationInCppFile.cpp
    UtestTest.cpp
    SimpleMutexTest.cpp
    UtestPlatformTest.cpp
)

if (MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4723")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4723")
endif (MSVC)

add_executable(CppUTestTests ${CppUTestTests_src})
target_link_libraries(CppUTestTests CppUTest)

if (TESTS)
    if (TESTS_DETAILED)
        # get all test groups and names
        execute_process(COMMAND tests/CppUTestTests -ln OUTPUT_VARIABLE CppUTestTests_GroupsAndNames)
        # convert space-separated string into a list
        separate_arguments(CppUTestTests_GroupsAndNames)
        # create separate CTest test for each CppUTestTests test
        set(lastgroup "")
        foreach(testfullname ${CppUTestTests_GroupsAndNames})
            string(REGEX MATCH "^([^/.]+)" groupname ${testfullname})
            string(REGEX MATCH "([^/.]+)$" testname ${testfullname})
            if (NOT ("${groupname}" STREQUAL "${lastgroup}"))
                message("TestGroup1: ${groupname}:")
                set(lastgroup "${groupname}")
            endif (NOT ("${groupname}" STREQUAL "${lastgroup}"))
            message("... ${testname}")
            add_test(NAME CppUTest.${testfullname} COMMAND ${EXECUTABLE_OUTPUT_PATH}/CppUTestTests -sg ${groupname} -sn ${testname} -c)
        endforeach()
    else (TESTS_DETAILED)
        # get all test groups
        execute_process(COMMAND tests/CppUTestTests -lg OUTPUT_VARIABLE CppUTestTests_Groups)
        # create separate test for each group
        separate_arguments(CppUTestTests_Groups)
        foreach(group ${CppUTestTests_Groups})
            message("TestGroup1: ${group}")
            add_test(NAME CppUTest.${group} COMMAND ${EXECUTABLE_OUTPUT_PATH}/CppUTestTests -sg ${group} -c)
        endforeach()
    endif (TESTS_DETAILED)

    add_subdirectory(CppUTestExt)
endif (TESTS)

