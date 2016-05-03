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

set(CppUTest_headers
        ${CppUTestRootDirectory}/include/CppUTest/CommandLineArguments.h
        ${CppUTestRootDirectory}/include/CppUTest/PlatformSpecificFunctions.h
        ${CppUTestRootDirectory}/include/CppUTest/TestMemoryAllocator.h
        ${CppUTestRootDirectory}/include/CppUTest/CommandLineTestRunner.h
        ${CppUTestRootDirectory}/include/CppUTest/PlatformSpecificFunctions_c.h
        ${CppUTestRootDirectory}/include/CppUTest/TestOutput.h
        ${CppUTestRootDirectory}/include/CppUTest/CppUTestConfig.h
        ${CppUTestRootDirectory}/include/CppUTest/SimpleString.h
        ${CppUTestRootDirectory}/include/CppUTest/TestPlugin.h
        ${CppUTestRootDirectory}/include/CppUTest/JUnitTestOutput.h
        ${CppUTestRootDirectory}/include/CppUTest/TeamCityTestOutput.h
        ${CppUTestRootDirectory}/include/CppUTest/StandardCLibrary.h
        ${CppUTestRootDirectory}/include/CppUTest/TestRegistry.h
        ${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetector.h
        ${CppUTestRootDirectory}/include/CppUTest/TestFailure.h
        ${CppUTestRootDirectory}/include/CppUTest/TestResult.h
        ${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetectorMallocMacros.h
        ${CppUTestRootDirectory}/include/CppUTest/TestFilter.h
        ${CppUTestRootDirectory}/include/CppUTest/TestTestingFixture.h
        ${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetectorNewMacros.h
        ${CppUTestRootDirectory}/include/CppUTest/TestHarness.h
        ${CppUTestRootDirectory}/include/CppUTest/Utest.h
        ${CppUTestRootDirectory}/include/CppUTest/MemoryLeakWarningPlugin.h
        ${CppUTestRootDirectory}/include/CppUTest/TestHarness_c.h
        ${CppUTestRootDirectory}/include/CppUTest/UtestMacros.h
)

add_library(CppUTest STATIC ${CppUTest_src} ${CppUTest_headers})
if (WIN32)
    target_link_libraries(CppUTest winmm.lib)
endif (WIN32)
install(FILES ${CppUTest_headers} DESTINATION include/CppUTest)
install(TARGETS CppUTest
    EXPORT CppUTestTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
