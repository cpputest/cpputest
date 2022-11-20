add_library(${CppUTestLibName}
    src/CommandLineArguments.cpp
    src/MemoryLeakWarningPlugin.cpp
    src/TestHarness_c.cpp
    src/TestRegistry.cpp
    src/CommandLineTestRunner.cpp
    src/SimpleString.cpp
    src/SimpleStringInternalCache.cpp
    src/TestMemoryAllocator.cpp
    src/TestResult.cpp
    src/JUnitTestOutput.cpp
    src/TeamCityTestOutput.cpp
    src/TestFailure.cpp
    src/TestOutput.cpp
    src/MemoryLeakDetector.cpp
    src/TestFilter.cpp
    src/TestPlugin.cpp
    src/TestTestingFixture.cpp
    src/SimpleMutex.cpp
    src/Utest.cpp
    include/CppUTest/CommandLineArguments.h
    include/CppUTest/PlatformSpecificFunctions.h
    include/CppUTest/TestMemoryAllocator.h
    include/CppUTest/CommandLineTestRunner.h
    include/CppUTest/PlatformSpecificFunctions_c.h
    include/CppUTest/TestOutput.h
    include/CppUTest/CppUTestConfig.h
    include/CppUTest/SimpleString.h
    include/CppUTest/SimpleStringInternalCache.h
    include/CppUTest/TestPlugin.h
    include/CppUTest/JUnitTestOutput.h
    include/CppUTest/TeamCityTestOutput.h
    include/CppUTest/StandardCLibrary.h
    include/CppUTest/TestRegistry.h
    include/CppUTest/MemoryLeakDetector.h
    include/CppUTest/TestFailure.h
    include/CppUTest/TestResult.h
    include/CppUTest/MemoryLeakDetectorMallocMacros.h
    include/CppUTest/TestFilter.h
    include/CppUTest/TestTestingFixture.h
    include/CppUTest/MemoryLeakDetectorNewMacros.h
    include/CppUTest/TestHarness.h
    include/CppUTest/Utest.h
    include/CppUTest/MemoryLeakWarningPlugin.h
    include/CppUTest/TestHarness_c.h
    include/CppUTest/UtestMacros.h
    include/CppUTest/SimpleMutex.h
)

if(CPPUTEST_PLATFORM)
    target_sources(${CppUTestLibName}
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/Platforms/${CPPUTEST_PLATFORM}/UtestPlatform.cpp
    )
    target_include_directories(${CppUTestLibName}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Platforms/${CPPUTEST_PLATFORM}/include>
    )
endif()

if (MINGW)
    include(CheckStructHasMember)
    check_struct_has_member("struct timespec" tv_sec time.h HAVE_STRUCT_TIMESPEC LANGUAGE CXX)
endif()

#[[Arrange for the include directory to be added to the include paths of any CMake target depending on CppUTest.]]
target_include_directories(${CppUTestLibName}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_definitions(${CppUTestLibName}
    PRIVATE
        $<$<BOOL:${CPPUTEST_USE_SECURE_STDLIB}>:STDC_WANT_SECURE_LIB>
        # Apply workaround for MinGW timespec redefinition (pthread.h / time.h).
        $<$<BOOL:${HAVE_STRUCT_TIMESPEC}>:_TIMESPEC_DEFINED>
)

if (WIN32)
    target_link_libraries(${CppUTestLibName} winmm)
endif (WIN32)

add_library(CppUTest::CppUTest ALIAS ${CppUTestLibName})

if(NOT CPPUTEST_MEM_LEAK_DETECTION_DISABLED)
    if(MSVC)
        set(force_include "/FI")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "IAR")
        set(force_include "--preinclude ")
    else()
        set(force_include "-include")
    endif()
    target_compile_options(${CppUTestLibName}
        PUBLIC
            "$<$<COMPILE_LANGUAGE:C,CXX>:${force_include}CppUTest/MemoryLeakDetectorMallocMacros.h>"
            "$<$<COMPILE_LANGUAGE:CXX>:${force_include}CppUTest/MemoryLeakDetectorNewMacros.h>"
    )
endif()

if(CPPUTEST_BUILD_TESTING)
    add_subdirectory(tests)
endif()

# Installation
if(PROJECT_IS_TOP_LEVEL)
    install(
        TARGETS ${CppUTestLibName}
        EXPORT CppUTestTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
    install(
        DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/CppUTest"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    )
endif()
