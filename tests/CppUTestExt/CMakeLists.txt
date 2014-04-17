set(CppUTestExtTests_src
        AllTests.cpp
        MockActualCallTest.cpp
        MockSupportTest.cpp
        CodeMemoryReportFormatterTest.cpp
        MockCheatSheetTest.cpp
        MockSupport_cTest.cpp
        GMockTest.cpp
        MockExpectedCallTest.cpp
        MockSupport_cTestCFile.c
        GTest1Test.cpp
        MockExpectedFunctionsListTest.cpp
        MemoryReportAllocatorTest.cpp
        MockFailureTest.cpp
        OrderedTestTest.cpp
        MemoryReportFormatterTest.cpp
        MemoryReporterPluginTest.cpp
        MockPluginTest.cpp
)

add_executable(CppUTestExtTests ${CppUTestExtTests_src})
target_link_libraries(CppUTestExtTests CppUTest CppUTestExt ${CPPUNIT_EXTERNAL_LIBRARIES})
add_test(CppUTestExtTests ${EXECUTABLE_OUTPUT_PATH}/CppUTestExtTests)
