add_executable(ExampleTests
    AllTests.cpp
    CircularBufferTest.cpp
    EventDispatcherTest.cpp
    FEDemoTest.cpp
    HelloTest.cpp
    MockDocumentationTest.cpp
    PrinterTest.cpp
)

cpputest_normalize_test_output_location(ExampleTests)
target_link_libraries(ExampleTests
    PRIVATE
        ApplicationLib
        CppUTest::${CppUTestLibName}
        CppUTest::${CppUTestExtLibName}
)
cpputest_buildtime_discover_tests(ExampleTests)
