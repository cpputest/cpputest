add_executable(CppUTestTests
    src/AllTests.cpp
    src/SetPluginTest.cpp
    src/CheatSheetTest.cpp
    src/SimpleStringTest.cpp
    src/SimpleStringCacheTest.cpp
    src/CompatabilityTests.cpp
    src/CommandLineArgumentsTest.cpp
    src/TestFailureTest.cpp
    src/TestFailureNaNTest.cpp
    src/CommandLineTestRunnerTest.cpp
    src/TestFilterTest.cpp
    src/TestHarness_cTest.cpp
    src/JUnitOutputTest.cpp
    src/TestHarness_cTestCFile.c
    src/DummyMemoryLeakDetector.cpp
    src/MemoryLeakDetectorTest.cpp
    src/TestInstallerTest.cpp
    src/AllocLetTestFree.c
    src/MemoryOperatorOverloadTest.cpp
    src/TestMemoryAllocatorTest.cpp
    src/MemoryLeakWarningTest.cpp
    src/TestOutputTest.cpp
    src/AllocLetTestFreeTest.cpp
    src/TestRegistryTest.cpp
    src/AllocationInCFile.c
    src/PluginTest.cpp
    src/TestResultTest.cpp
    src/PreprocessorTest.cpp
    src/TestUTestMacro.cpp
    src/TestUTestStringMacro.cpp
    src/AllocationInCppFile.cpp
    src/UtestTest.cpp
    src/SimpleMutexTest.cpp
    src/UtestPlatformTest.cpp
    src/TeamCityOutputTest.cpp
)

if(CPPUTEST_STD_C_LIB_DISABLED)
    add_library(DummyUTestPlatform INTERFACE)
    target_sources(DummyUTestPlatform
        INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/DummyUTestPlatform/DummyUTestPlatform.cpp
    )
    target_link_libraries(CppUTestTests PRIVATE DummyUTestPlatform)
endif()

target_include_directories(CppUTestTests
    PRIVATE include
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
