include_directories(../ApplicationLib)

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
target_link_libraries(ExampleTests ApplicationLib CppUTest CppUTestExt)
cpputest_buildtime_discover_tests(ExampleTests)
