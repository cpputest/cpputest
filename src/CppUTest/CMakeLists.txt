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

if(NOT DEFINED CPP_PLATFORM)
    if(MSVC)
        set(CPP_PLATFORM "VisualCpp")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Symbian")
        set(CPP_PLATFORM "Symbian")
    elseif(BORLAND)
        set(CPP_PLATFORM "Borland")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "IAR")
        set(CPP_PLATFORM "Iar")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "DOS")
        set(CPP_PLATFORM "DOS")
    elseif(CMAKE_CXX_COMPILE_FEATURES STREQUAL "TI")
        # FIXME: This is probably wrong.
        set(CPP_PLATFORM STREQUAL "C2000")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "ARMCC")
        set(CPP_PLATFORM "armcc")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "ARMClang")
        set(CPP_PLATFORM "Keil")
    elseif(NOT STD_C)
        set(CPP_PLATFORM "GccNoStdC")
    else()
        set(CPP_PLATFORM "Gcc")
    endif()
endif()

if(DEFINED CPP_PLATFORM)
    target_sources(${CppUTestLibName}
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/../Platforms/${CPP_PLATFORM}/UtestPlatform.cpp
    )
    target_include_directories(${CppUTestLibName}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../include/Platforms/${CPP_PLATFORM}>
    )
endif()

#[[Arrange for the include directory to be added to the include paths of any CMake target depending on CppUTest.]]
target_include_directories(${CppUTestLibName}
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

set_target_properties(${CppUTestLibName} PROPERTIES
    PUBLIC_HEADER "${CppUTest_headers}")

if (WIN32)
    target_link_libraries(${CppUTestLibName} winmm)
endif (WIN32)
install(TARGETS ${CppUTestLibName}
    EXPORT CppUTestTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/CppUTest"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/CppUTest")
