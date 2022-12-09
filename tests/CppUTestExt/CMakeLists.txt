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

if(CPPUTEST_TEST_GTEST)
    find_package(GTest)
    if(NOT GTest_FOUND)
        if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.11")
            include(FetchContent)
            FetchContent_Declare(googletest
                URL https://github.com/google/googletest/archive/release-1.8.0.zip
            )
            if(NOT googletest_POPULATED)
                FetchContent_Populate(googletest)
                add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
            endif()
            set(GTEST_LIBRARIES gmock gtest)
        else()
            # FetchContent is not supported.
            message(FATAL_ERROR
                "GTest not found"
            )
        endif()
    endif()
    target_link_libraries(CppUTestExtTests PRIVATE ${GTEST_LIBRARIES})
    target_compile_definitions(CppUTestExtTests PRIVATE CPPUTEST_INCLUDE_GTEST_TESTS)
endif()

target_link_libraries(CppUTestExtTests
    PRIVATE
        CppUTest
        CppUTestExt
)

add_mapfile(CppUTestExtTests)

if(CPPUTEST_TEST_DISCOVERY)
    include(CppUTest)
    cpputest_discover_tests(CppUTestExtTests
        DETAILED FALSE
    )
endif()
