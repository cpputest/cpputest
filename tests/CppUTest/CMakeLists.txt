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

if(MINGW OR (${CMAKE_HOST_SYSTEM_NAME} MATCHES "OpenBSD"))
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads)
    if(CMAKE_USE_PTHREADS_INIT)
        target_link_libraries(CppUTestTests PRIVATE Threads::Threads)
    endif()
endif()

target_link_libraries(CppUTestTests PRIVATE ${CppUTestLibName})

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.13")
    target_link_options(CppUTestTests
        PRIVATE
            $<$<CXX_COMPILER_ID:GNU,Clang>:LINKER:-Map=$<TARGET_FILE:CppUTestTests>.map>
            "$<$<CXX_COMPILER_ID:MSVC>:/MAP>"
            $<$<CXX_COMPILER_ID:IAR>:"SHELL:--map $<TARGET_FILE:CppUTestTests>.map">
    )
endif()

if(CPPUTEST_TEST_DISCOVERY)
    include(CppUTest)
    cpputest_discover_tests(CppUTestTests)
endif()
