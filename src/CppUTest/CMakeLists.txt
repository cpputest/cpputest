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

set(CppUTest_headers
    ${PROJECT_SOURCE_DIR}/include/CppUTest/CommandLineArguments.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/CommandLineTestRunner.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/CppUTestConfig.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/CppUTestGeneratedConfig.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/JUnitTestOutput.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/MemoryLeakDetector.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/MemoryLeakDetectorMallocMacros.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/MemoryLeakDetectorNewMacros.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/MemoryLeakWarningPlugin.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/PlatformSpecificFunctions_c.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/PlatformSpecificFunctions.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/SimpleMutex.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/SimpleString.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/SimpleStringInternalCache.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/StandardCLibrary.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/TeamCityTestOutput.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/TestFailure.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/TestFilter.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/TestHarness_c.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/TestHarness.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/TestMemoryAllocator.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/TestOutput.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/TestPlugin.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/TestRegistry.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/TestResult.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/TestTestingFixture.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/Utest.h
    ${PROJECT_SOURCE_DIR}/include/CppUTest/UtestMacros.h
)

target_sources(CppUTest
    PRIVATE ${CppUTest_headers}
)

set_target_properties(CppUTest PROPERTIES
    PUBLIC_HEADER "${CppUTest_headers}"
)

#[[Set CPP_PLATFORM in a parent CMakeLists.txt if reusing one of the provided platforms, else supply the missing definitions]]
if (CPP_PLATFORM)
    target_sources(CppUTest
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/../Platforms/${CPP_PLATFORM}/UtestPlatform.cpp
    )
    target_include_directories(CppUTest
        PUBLIC
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/Platforms/${CPP_PLATFORM}>
    )
endif(CPP_PLATFORM)

#[[Arrange for the include directory to be added to the include paths of any CMake target depending on CppUTest.]]
target_include_directories(CppUTest
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/CppUTest>
)

if (WIN32)
    target_link_libraries(CppUTest winmm)
endif (WIN32)

add_library(CppUTest::CppUTest ALIAS CppUTest)

if(LIBNAME_POSTFIX_DEBUG)
    set_target_properties(CppUTest PROPERTIES DEBUG_POSTFIX "d")
endif()

if(LIBNAME_POSTFIX_BITSIZE)
    if(CMAKE_SIZEOF_VOID_P STREQUAL "8")
        set_target_properties(CppUTest PROPERTIES EXPORT_NAME "CppUTest64")
    elseif(CMAKE_SIZEOF_VOID_P STREQUAL "4")
        set_target_properties(CppUTest PROPERTIES EXPORT_NAME "CppUTest32")
    endif()
endif()

if(PROJECT_IS_TOP_LEVEL)
    install(
        TARGETS CppUTest
        EXPORT CppUTestTargets
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/CppUTest
    )
endif()
