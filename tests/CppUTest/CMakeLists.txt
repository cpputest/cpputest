add_executable(CppUTestTests
    AllTests.cpp
    SetPluginTest.cpp
    CheatSheetTest.cpp
    SimpleStringTest.cpp
    SimpleStringCacheTest.cpp
    CompatabilityTests.cpp
    CommandLineArgumentsTest.cpp
    TestFailureTest.cpp
    TestFailureNaNTest.cpp
    CommandLineTestRunnerTest.cpp
    TestFilterTest.cpp
    TestHarness_cTest.cpp
    JUnitOutputTest.cpp
    TestHarness_cTestCFile.c
    DummyMemoryLeakDetector.cpp
    MemoryLeakDetectorTest.cpp
    TestInstallerTest.cpp
    AllocLetTestFree.c
    MemoryOperatorOverloadTest.cpp
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
    TestUTestStringMacro.cpp
    AllocationInCppFile.cpp
    UtestTest.cpp
    SimpleMutexTest.cpp
    UtestPlatformTest.cpp
    TeamCityOutputTest.cpp
)

if(CPPUTEST_STD_C_LIB_DISABLED)
    target_sources(CppUTestTests
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/../DummyUTestPlatform/DummyUTestPlatform.cpp
    )
endif()

target_link_libraries(CppUTestTests PRIVATE CppUTest)

add_mapfile(CppUTestTests)

if(CPPUTEST_TEST_DISCOVERY)
    include(CppUTest)
    cpputest_discover_tests(CppUTestTests)
endif()
