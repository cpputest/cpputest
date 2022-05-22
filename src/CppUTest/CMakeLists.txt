add_library(${CppUTestLibName}
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

if(LIBNAME_POSTFIX_DEBUG)
    set_target_properties(${CppUTestLibName} PROPERTIES DEBUG_POSTFIX "d")
endif()

#[[Set CPP_PLATFORM in a parent CMakeLists.txt if reusing one of the provided platforms, else supply the missing definitions]]
if (CPP_PLATFORM)
    target_sources(${CppUTestLibName}
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/../Platforms/${CPP_PLATFORM}/UtestPlatform.cpp
    )
    target_include_directories(${CppUTestLibName}
        PUBLIC
            ${PROJECT_SOURCE_DIR}/include/Platforms/${CPP_PLATFORM}
    )
endif(CPP_PLATFORM)

#[[Arrange for the include directory to be added to the include paths of any CMake target depending on CppUTest.]]
target_include_directories(${CppUTestLibName}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

if (WIN32)
    target_link_libraries(${CppUTestLibName} winmm)
endif (WIN32)
