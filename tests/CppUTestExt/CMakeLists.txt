set(CppUTestExtTests_src
        AllTests.cpp
        TestMockActualFunctionCall.cpp
        TestMockSupport.cpp
        TestCodeMemoryReportFormatter.cpp
        TestMockCheatSheet.cpp
        TestMockSupport_c.cpp
        TestGMock.cpp
        TestMockExpectedFunctionCall.cpp
        TestMockSupport_cCFile.c
        TestGTest.cpp
        TestMockExpectedFunctionsList.cpp
        TestMemoryReportAllocator.cpp
        TestMockFailure.cpp
        TestOrderedTest.cpp
        TestMemoryReportFormatter.cpp
        TestMemoryReporterPlugin.cpp
        TestMockPlugin.cpp
)

add_executable(CppUTestExtTests ${CppUTestExtTests_src})
target_link_libraries(CppUTestExtTests CppUTest CppUTestExt ${CPPUNIT_EXTERNAL_LIBRARIES})
add_test(CppUTestExtTests CppUTestExtTests)
