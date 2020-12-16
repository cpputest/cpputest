add_library(CppUTest
        CommandLineArguments.cpp
        MemoryLeakWarningPlugin.cpp
        TestHarness_c.cpp
        TestRegistry.cpp
        CommandLineTestRunner.cpp
        SimpleString.cpp
        SimpleStringInternalCache.cpp
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
)

#[[Set CPP_PLATFORM in a parent CMakeLists.txt if reusing one of the provided platforms, else supply the missing definitions]]
if (CPP_PLATFORM)
    target_sources(CppUTest
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/../Platforms/${CPP_PLATFORM}/UtestPlatform.cpp
    )
endif(CPP_PLATFORM)

#[[Arrange for the include directory to be added to the include paths of any CMake target depending on CppUTest.]]
target_include_directories(CppUTest
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
        $<INSTALL_INTERFACE:include>
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
        ${CppUTestRootDirectory}/include/CppUTest/SimpleStringInternalCache.h
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
        ${CppUTestRootDirectory}/include/CppUTest/SimpleMutex.h
)

set_target_properties(CppUTest PROPERTIES
    PUBLIC_HEADER "${CppUTest_headers}")

if (WIN32)
    target_link_libraries(CppUTest winmm.lib)
endif (WIN32)
install(TARGETS CppUTest
    EXPORT CppUTestTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/CppUTest"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/CppUTest")
