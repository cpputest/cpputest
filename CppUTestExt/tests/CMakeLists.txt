add_executable(CppUTestExtTests
    src/AllTests.cpp
    src/CodeMemoryReporterTest.cpp
    src/GMockTest.cpp
    src/GTest1Test.cpp
    src/IEEE754PluginTest.cpp
    src/IEEE754PluginTest_c.c
    src/MemoryReportAllocatorTest.cpp
    src/MemoryReporterPluginTest.cpp
    src/MemoryReportFormatterTest.cpp
    src/MockActualCallTest.cpp
    src/MockCheatSheetTest.cpp
    src/MockCallTest.cpp
    src/MockComparatorCopierTest.cpp
    src/MockExpectedCallTest.cpp
    src/ExpectedFunctionsListTest.cpp
    src/MockFailureReporterForTest.cpp
    src/MockFailureTest.cpp
    src/MockHierarchyTest.cpp
    src/MockNamedValueTest.cpp
    src/MockParameterTest.cpp
    src/MockPluginTest.cpp
    src/MockSupportTest.cpp
    src/MockSupport_cTestCFile.c
    src/MockSupport_cTest.cpp
    src/MockStrictOrderTest.cpp
    src/MockReturnValueTest.cpp
    src/OrderedTestTest_c.c
    src/OrderedTestTest.cpp
)

if(CPPUTEST_STD_C_LIB_DISABLED)
    target_link_libraries(CppUTestExtTests PRIVATE DummyUTestPlatform)
endif()

target_include_directories(CppUTestExtTests
    PRIVATE include
)

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

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.13")
    target_link_options(CppUTestExtTests
        PRIVATE
            $<$<CXX_COMPILER_ID:GNU,Clang>:LINKER:-Map=$<TARGET_FILE:CppUTestExtTests>.map>
            "$<$<CXX_COMPILER_ID:MSVC>:/MAP>"
            $<$<CXX_COMPILER_ID:IAR>:"SHELL:--map $<TARGET_FILE:CppUTestExtTests>.map">
    )
endif()

if(CPPUTEST_TEST_DISCOVERY)
    include(CppUTest)
    cpputest_discover_tests(CppUTestExtTests
        DETAILED FALSE
    )
endif()
