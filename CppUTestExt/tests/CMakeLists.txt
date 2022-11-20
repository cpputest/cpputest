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
