add_library(CppUTestExtTests_main OBJECT
    AllTests.cpp
)

if(CPPUTEST_STD_C_LIB_DISABLED)
    target_sources(CppUTestExtTests_main
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
    target_link_libraries(CppUTestExtTests_main PUBLIC ${GTEST_LIBRARIES})
    target_compile_definitions(CppUTestExtTests_main PUBLIC CPPUTEST_INCLUDE_GTEST_TESTS)
endif()

target_link_libraries(CppUTestExtTests_main
    PUBLIC
        CppUTest
        CppUTestExt
)

if(NOT CPPUTEST_SPLIT_TESTS)
    add_executable(CppUTestExtTests)

    add_mapfile(CppUTestExtTests)

    if(CPPUTEST_TEST_DISCOVERY)
        include(CppUTest)
        cpputest_discover_tests(CppUTestExtTests
            DETAILED FALSE
        )
    endif()
endif()

function(add_cpputestext_test number)
    set(name CppUTestExtTests)

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
        PRIVATE CppUTestExtTests_main
    )
endfunction()

add_cpputestext_test(1
    MockFailureReporterForTest.cpp
    ExpectedFunctionsListTest.cpp
    GMockTest.cpp
    GTest1Test.cpp
    GTest2ConvertorTest.cpp
)

add_cpputestext_test(2
    MockFailureReporterForTest.cpp
    MemoryReportAllocatorTest.cpp
    MemoryReportFormatterTest.cpp
    MemoryReporterPluginTest.cpp
    MockActualCallTest.cpp
    MockCheatSheetTest.cpp
    MockComparatorCopierTest.cpp
    MockExpectedCallTest.cpp
    MockHierarchyTest.cpp
)

add_cpputestext_test(3
    MockFailureReporterForTest.cpp
    CodeMemoryReporterTest.cpp
    OrderedTestTest.cpp
    OrderedTestTest_c.c
)

add_cpputestext_test(4
    MockFailureReporterForTest.cpp
    MockReturnValueTest.cpp
    MockNamedValueTest.cpp
)

add_cpputestext_test(5
    MockFailureReporterForTest.cpp
    MockPluginTest.cpp
    MockSupport_cTest.cpp
    MockSupport_cTestCFile.c
)

add_cpputestext_test(6
    MockFailureReporterForTest.cpp
    ExpectedFunctionsListTest.cpp
    MockCallTest.cpp
)

add_cpputestext_test(7
    MockFailureReporterForTest.cpp
    MockComparatorCopierTest.cpp
    MockHierarchyTest.cpp
    MockParameterTest.cpp
)

add_cpputestext_test(8
    MockFailureReporterForTest.cpp
    IEEE754PluginTest.cpp
    IEEE754PluginTest_c.c
    MockComparatorCopierTest.cpp
)

add_cpputestext_test(9
    MockFailureReporterForTest.cpp
    MockFailureTest.cpp
    MockHierarchyTest.cpp
    MockPluginTest.cpp
    MockReturnValueTest.cpp
    MockStrictOrderTest.cpp
    MockSupportTest.cpp
)
