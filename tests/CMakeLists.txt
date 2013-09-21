enable_testing()

set(CppUTestTests_src
        AllTests.cpp
        SetPluginTest.cpp
        CheatSheetTest.cpp
        SimpleStringTest.cpp
        CommandLineArgumentsTest.cpp
        TestFailureTest.cpp
        TestFailureNaNTest.cpp
        CommandLineTestRunnerTest.cpp
        TestFilterTest.cpp
        TestHarness_cTest.cpp
        JUnitOutputTest.cpp
        TestHarness_cTestCFile.c
        MemoryLeakDetectorTest.cpp
        TestInstallerTest.cpp
        AllocLetTestFree.c
        MemoryLeakOperatorOverloadsTest.cpp
        TestMemoryAllocatorTest.cpp
        MemoryLeakWarningTest.cpp
        TestOutputTest.cpp
        AllocLetTestFreeTest.cpp
        NullTestTest.cpp
        TestRegistryTest.cpp
        AllocationInCFile.c
        PluginTest.cpp
        TestResultTest.cpp
        PreprocessorTest.cpp
        TestUTestMacro.cpp
        AllocationInCppFile.cpp
        UtestTest.cpp
)

if (MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4723")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4723")
endif (MSVC)

add_executable(CppUTestTests ${CppUTestTests_src})
target_link_libraries(CppUTestTests CppUTest)
add_test(CppUTestTests CppUTestTests)

if (TESTS)
    add_subdirectory(CppUTestExt)
endif (TESTS)
