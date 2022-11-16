add_executable(CppUTestExtTests
    AllTests.cpp
    CodeMemoryReporterTest.cpp
    GMockTest.cpp
    GTest1Test.cpp
    IEEE754PluginTest.cpp
    IEEE754PluginTest_c.c
    MemoryReportAllocatorTest.cpp
    MemoryReporterPluginTest.cpp
    MemoryReportFormatterTest.cpp
    MockActualCallTest.cpp
    MockCheatSheetTest.cpp
    MockCallTest.cpp
    MockComparatorCopierTest.cpp
    MockExpectedCallTest.cpp
    ExpectedFunctionsListTest.cpp
    MockFailureReporterForTest.cpp
    MockFailureTest.cpp
    MockHierarchyTest.cpp
    MockNamedValueTest.cpp
    MockParameterTest.cpp
    MockPluginTest.cpp
    MockSupportTest.cpp
    MockSupport_cTestCFile.c
    MockSupport_cTest.cpp
    MockStrictOrderTest.cpp
    MockReturnValueTest.cpp
    OrderedTestTest_c.c
    OrderedTestTest.cpp
)

if(CPPUTEST_STD_C_LIB_DISABLED)
    target_sources(CppUTestExtTests
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/../DummyUTestPlatform/DummyUTestPlatform.cpp
    )
endif()

if(MINGW)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads)
    if(CMAKE_USE_PTHREADS_INIT)
        target_link_libraries(CppUTestExtTests PRIVATE Threads::Threads)
    endif()
endif()

target_link_libraries(CppUTestExtTests
    PRIVATE
        ${CppUTestLibName}
        ${CppUTestExtLibName}
)

add_mapfile(CppUTestExtTests)

if(CPPUTEST_TEST_DISCOVERY)
    include(CppUTest)
    cpputest_discover_tests(CppUTestExtTests
        DETAILED FALSE
    )
endif()
