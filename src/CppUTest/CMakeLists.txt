set(CppUTest_src
        CommandLineArguments.cpp
        MemoryLeakWarningPlugin.cpp
        TestHarness_c.cpp
        TestRegistry.cpp
        CommandLineTestRunner.cpp
        SimpleString.cpp
        TestMemoryAllocator.cpp
        TestResult.cpp
        JUnitTestOutput.cpp
        TeamCityTestOutput.cpp
        TestFailure.cpp
        TestOutput.cpp
        MemoryLeakDetector.cpp
        TestFilter.cpp
        TestPlugin.cpp
        TestTestingFixture.cpp
        SimpleMutex.cpp
        Utest.cpp
        ../Platforms/${CPP_PLATFORM}/UtestPlatform.cpp
)

add_library(CppUTest STATIC ${CppUTest_src})
if (WIN32)
    target_link_libraries(CppUTest winmm.lib)
endif (WIN32)

target_include_directories(CppUTest PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include/CppUTest>
    $<INSTALL_INTERFACE:include>
    )

install(DIRECTORY ${CppUTestRootDirectory}/include/CppUTest DESTINATION include)
install(TARGETS CppUTest
    EXPORT CppUTestTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
