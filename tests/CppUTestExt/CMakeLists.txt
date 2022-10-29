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

if(MINGW)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads)
    if(CMAKE_USE_PTHREADS_INIT)
        target_link_libraries(CppUTestExtTests PRIVATE Threads::Threads)
    endif()
endif()

cpputest_normalize_test_output_location(CppUTestExtTests)
target_link_libraries(CppUTestExtTests
    PRIVATE
        ${CppUTestLibName}
        ${CppUTestExtLibName}
)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.13")
    target_link_options(CppUTestExtTests
        PRIVATE
            $<$<CXX_COMPILER_ID:GNU,Clang>:LINKER:-Map=$<TARGET_FILE:CppUTestExtTests>.map>
            "$<$<CXX_COMPILER_ID:MSVC>:/MAP>"
            $<$<CXX_COMPILER_ID:IAR>:"SHELL:--map $<TARGET_FILE:CppUTestExtTests>.map">
    )
endif()

if (CPPUTEST_TEST_DISCOVERY)
    cpputest_buildtime_discover_tests(CppUTestExtTests)
endif()
