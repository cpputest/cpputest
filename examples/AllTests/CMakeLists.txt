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
        .
)

target_compile_options(ExampleTests
    PRIVATE $<$<BOOL:${MSVC}>:/wd4723>
)

target_link_libraries(ExampleTests
    PRIVATE
        ApplicationLib
        CppUTest::CppUTestExt
)

include(CppUTestBuildTimeDiscoverTests)
if(CPPUTEST_TEST_DISCOVERY OR NOT DEFINED CPPUTEST_TEST_DISCOVERY)
    cpputest_buildtime_discover_tests(ExampleTests
        DETAILED TRUE
    )
endif()
