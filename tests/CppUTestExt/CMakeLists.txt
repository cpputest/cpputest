set(CppUTestExtTests_src
    AllTests.cpp
    CodeMemoryReportFormatterTest.cpp
    GMockTest.cpp
    GTest1Test.cpp
    MemoryReportAllocatorTest.cpp
    MemoryReporterPluginTest.cpp
    MemoryReportFormatterTest.cpp
    MockActualCallTest.cpp
    MockCheatSheetTest.cpp
    MockExpectedCallTest.cpp
    MockExpectedFunctionsListTest.cpp
    MockFailureTest.cpp
    MockPluginTest.cpp
    MockSupportTest.cpp
    MockSupport_cTestCFile.c
    MockSupport_cTest.cpp
    OrderedTestTest.cpp
)

add_executable(CppUTestExtTests ${CppUTestExtTests_src})
target_link_libraries(CppUTestExtTests CppUTest CppUTestExt ${CPPUNIT_EXTERNAL_LIBRARIES})

if (TESTS)
    # get all test groups
    execute_process(COMMAND tests/CppUTestExt/CppUTestExtTests -lg OUTPUT_VARIABLE CppUTestTestsExt_Groups)
    # create separate test for each group
    separate_arguments(CppUTestTestsExt_Groups)
    foreach(group ${CppUTestTestsExt_Groups})
        message("TestGroup2: ${group}:")
        add_test(NAME CppUTestExt.${group} COMMAND ${EXECUTABLE_OUTPUT_PATH}/CppUTestExtTests -sg ${group} -c)
    endforeach()
endif (TESTS)
