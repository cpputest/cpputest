set(CppUTestExtTests_src
        AllTests.cpp
        TestMockActualFunctionCall.cpp
        TestMockSupport.cpp
        TestCodeMemoryReportFormatter.cpp
        TestMockCheatSheet.cpp
        TestMockSupport_c.cpp
        TestMockExpectedFunctionCall.cpp
        TestMockSupport_cCFile.c
        TestMockExpectedFunctionsList.cpp
        TestMemoryReportAllocator.cpp
        TestMockFailure.cpp
        TestOrderedTest.cpp
        TestMemoryReportFormatter.cpp
        TestMemoryReporterPlugin.cpp
        TestMockPlugin.cpp
)
if (GMOCK)
  list(APPEND CppUTestExtTests_src TestGMock.cpp TestGTest.cpp)
endif (GMOCK)
if (REAL_GTEST)
  list(APPEND CppUTestExtTests_src TestGTest.cpp)
endif (REAL_GTEST)

add_executable(CppUTestExtTests ${CppUTestExtTests_src})
target_link_libraries(CppUTestExtTests CppUTest CppUTestExt ${CPPUNIT_EXTERNAL_LIBRARIES})
add_test(CppUTestExtTests CppUTestExtTests)
