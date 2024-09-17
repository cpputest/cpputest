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
        ${PROJECT_SOURCE_DIR}/include/CppUTest/CommandLineArguments.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/PlatformSpecificFunctions.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/TestMemoryAllocator.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/CommandLineTestRunner.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/PlatformSpecificFunctions_c.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/TestOutput.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/CppUTestConfig.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/SimpleString.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/SimpleStringInternalCache.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/TestPlugin.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/JUnitTestOutput.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/TeamCityTestOutput.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/StandardCLibrary.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/TestRegistry.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/MemoryLeakDetector.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/TestFailure.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/TestResult.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/MemoryLeakDetectorMallocMacros.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/TestFilter.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/TestTestingFixture.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/MemoryLeakDetectorNewMacros.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/TestHarness.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/Utest.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/MemoryLeakWarningPlugin.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/TestHarness_c.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/UtestMacros.h
        ${PROJECT_SOURCE_DIR}/include/CppUTest/SimpleMutex.h
)

#[[Set CPPUTEST_PLATFORM in a parent CMakeLists.txt if reusing one of the provided platforms, else supply the missing definitions]]
if(CPPUTEST_PLATFORM)
    target_sources(CppUTest
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/../Platforms/${CPPUTEST_PLATFORM}/UtestPlatform.cpp
    )
endif()

if(CPPUTEST_PLATFORM STREQUAL "c2000")
    target_include_directories(CppUTest
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../include/Platforms/c2000>
    )
elseif(CPPUTEST_PLATFORM STREQUAL "Iar")
    target_compile_options(CppUTest
        PRIVATE
            --dlib_config full
    )
endif()

if (MINGW)
    include(CheckStructHasMember)
    check_struct_has_member("struct timespec" tv_sec time.h HAVE_STRUCT_TIMESPEC LANGUAGE CXX)
endif()

#[[Arrange for the include directory to be added to the include paths of any CMake target depending on CppUTest.]]
target_include_directories(CppUTest
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_definitions(CppUTest
    PRIVATE
        $<$<BOOL:${CPPUTEST_USE_SECURE_STDLIB}>:STDC_WANT_SECURE_LIB>
        # Apply workaround for MinGW timespec redefinition (pthread.h / time.h).
        $<$<BOOL:${HAVE_STRUCT_TIMESPEC}>:_TIMESPEC_DEFINED>
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)

target_link_libraries(CppUTest
    PRIVATE
        $<$<BOOL:${WIN32}>:winmm>
        $<$<BOOL:${CMAKE_USE_PTHREADS_INIT}>:${CMAKE_THREAD_LIBS_INIT}>
)

add_library(CppUTest::CppUTest ALIAS CppUTest)

if(NOT CPPUTEST_MEM_LEAK_DETECTION_DISABLED)
    if(MSVC)
        set(force_include "/FI")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "IAR")
        set(force_include "--preinclude=")
    else()
        set(force_include "-include")
    endif()
    target_compile_options(CppUTest
        PUBLIC
            # Not all toolchains support multiple force includes (namely IAR),
            # so C and C++ each get their own.
            "$<$<COMPILE_LANGUAGE:C>:${force_include}CppUTest/MemoryLeakDetectorMallocMacros.h>"
            "$<$<COMPILE_LANGUAGE:CXX>:${force_include}CppUTest/MemoryLeakDetectorNewMacros.h>"
    )
endif()

# Installation
if(PROJECT_IS_TOP_LEVEL)
    install(
        TARGETS CppUTest
        EXPORT CppUTestTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
    install(
        DIRECTORY "${PROJECT_SOURCE_DIR}/include/CppUTest"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    )
endif()
