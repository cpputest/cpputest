add_library(CppUTestTests_main OBJECT
    AllTests.cpp
)

if(CPPUTEST_STD_C_LIB_DISABLED)
    target_sources(CppUTestTests_main
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/../DummyUTestPlatform/DummyUTestPlatform.cpp
    )
endif()

target_link_libraries(CppUTestTests_main
    PUBLIC CppUTest
)

include(CppUTest)

if(NOT CPPUTEST_SPLIT_TESTS)
    add_executable(CppUTestTests)

    add_mapfile(CppUTestTests)

    if(CPPUTEST_TEST_DISCOVERY)
        cpputest_discover_tests(CppUTestTests)
    endif()
endif()

function(add_cpputest_test number)
    set(name CppUTestTests)
    
    if(CPPUTEST_SPLIT_TESTS)
        string(APPEND name ${number})
        add_executable(${name})
        add_mapfile(${name})
        if(CPPUTEST_TEST_DISCOVERY)
            cpputest_discover_tests(${name})
        endif()
    endif()

    target_sources(${name}
        PRIVATE ${ARGN}
    )
    target_link_libraries(${name}
        PRIVATE CppUTestTests_main
    )
endfunction()

add_cpputest_test(1
    AllocLetTestFree.c
    AllocLetTestFreeTest.cpp
    CheatSheetTest.cpp
    CompatabilityTests.cpp
    CommandLineArgumentsTest.cpp
    CommandLineTestRunnerTest.cpp
    JUnitOutputTest.cpp
)

add_cpputest_test(2
    DummyMemoryLeakDetector.cpp
    MemoryLeakWarningTest.cpp
    PluginTest.cpp
    PreprocessorTest.cpp
    SetPluginTest.cpp
    SimpleMutexTest.cpp
    TeamCityOutputTest.cpp
    TestFailureNaNTest.cpp
    TestFailureTest.cpp
    TestResultTest.cpp
)

add_cpputest_test(3
    MemoryLeakDetectorTest.cpp
    SimpleStringTest.cpp
    SimpleStringCacheTest.cpp
)

add_cpputest_test(4
    TestOutputTest.cpp
    TestRegistryTest.cpp
)

add_cpputest_test(5
    AllocationInCFile.c
    AllocationInCppFile.cpp
    MemoryOperatorOverloadTest.cpp
    TeamCityOutputTest.cpp
)

add_cpputest_test(6
    TestFilterTest.cpp
    TestHarness_cTest.cpp
    TestHarness_cTestCFile.c
    TestInstallerTest.cpp
)

add_cpputest_test(7
    TestMemoryAllocatorTest.cpp
    TestUTestMacro.cpp
)

add_cpputest_test(8
    UtestPlatformTest.cpp
    UtestTest.cpp
    TestUTestStringMacro.cpp
)
