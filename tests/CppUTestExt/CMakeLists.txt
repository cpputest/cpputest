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

if (MINGW)
    find_package (Threads REQUIRED)
    set(THREAD_LIB "pthread")
endif (MINGW)

add_executable(CppUTestExtTests ${CppUTestExtTests_src})
target_link_libraries(CppUTestExtTests CppUTest CppUTestExt ${THREAD_LIB} ${CPPUNIT_EXTERNAL_LIBRARIES})
cpputest_buildtime_discover_tests (CppUTestExtTests)
