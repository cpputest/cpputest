set(CppUTestTests_src
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

if (MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4723")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4723")
endif (MSVC)

if (MINGW)
    find_package (Threads REQUIRED)
    set(THREAD_LIB "pthread")
endif (MINGW)

if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "OpenBSD")
   set (THREADS_HAVE_PTHREAD_ARG 1)
   find_package (Threads REQUIRED)
   set (THREAD_LIB "pthread")
endif ()

add_executable(CppUTestTests ${CppUTestTests_src})
cpputest_normalize_test_output_location(CppUTestTests)
target_link_libraries(CppUTestTests CppUTest ${THREAD_LIB})

if (TESTS_BUILD_DISCOVER)
    cpputest_buildtime_discover_tests(CppUTestTests)
endif()
