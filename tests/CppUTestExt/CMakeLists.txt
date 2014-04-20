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
add_test(CppUTestExtTests ${EXECUTABLE_OUTPUT_PATH}/CppUTestExtTests)
