add_executable(ExampleTests
    AllTests.cpp
    CircularBufferTest.cpp
    EventDispatcherTest.cpp
    FEDemoTest.cpp
    HelloTest.cpp
    MockDocumentationTest.cpp
    PrinterTest.cpp
)

target_link_libraries(ExampleTests
    PRIVATE
        ApplicationLib
        CppUTest::CppUTestExt
)

include(CppUTestNormalizeTestOutputLocation)
cpputest_normalize_test_output_location(ExampleTests)

include(CppUTestBuildTimeDiscoverTests)
cpputest_buildtime_discover_tests(ExampleTests)
