add_executable(ExampleTests
    AllTests.cpp
    CircularBufferTest.cpp
    EventDispatcherTest.cpp
    FEDemoTest.cpp
    HelloTest.cpp
    MockDocumentationTest.cpp
    PrinterTest.cpp
)

target_include_directories(ExampleTests
    PRIVATE
        ../ApplicationLib
)

cpputest_normalize_test_output_location(ExampleTests)
target_link_libraries(ExampleTests ApplicationLib ${CppUTestLibName} ${CppUTestExtLibName})
cpputest_buildtime_discover_tests(ExampleTests)
