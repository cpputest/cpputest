add_library(${CppUTestExtLibName} STATIC
    src/CodeMemoryReportFormatter.cpp
    src/GTest.cpp
    src/IEEE754ExceptionsPlugin.cpp
    src/MemoryReporterPlugin.cpp
    src/MockFailure.cpp
    src/MockSupportPlugin.cpp
    src/MockActualCall.cpp
    src/MockSupport_c.cpp
    src/MemoryReportAllocator.cpp
    src/MockExpectedCall.cpp
    src/MockNamedValue.cpp
    src/OrderedTest.cpp
    src/MemoryReportFormatter.cpp
    src/MockExpectedCallsList.cpp
    src/MockSupport.cpp
    include/CppUTestExt/CodeMemoryReportFormatter.h
    include/CppUTestExt/IEEE754ExceptionsPlugin.h
    include/CppUTestExt/MemoryReportAllocator.h
    include/CppUTestExt/MockExpectedCall.h
    include/CppUTestExt/MockCheckedExpectedCall.h
    include/CppUTestExt/MockExpectedCallsList.h
    include/CppUTestExt/MockSupportPlugin.h
    include/CppUTestExt/MemoryReportFormatter.h
    include/CppUTestExt/MockFailure.h
    include/CppUTestExt/MockSupport.h
    include/CppUTestExt/MockSupport_c.h
    include/CppUTestExt/GMock.h
    include/CppUTestExt/GTest.h
    include/CppUTestExt/GTestSupport.h
    include/CppUTestExt/MemoryReporterPlugin.h
    include/CppUTestExt/OrderedTest.h
    include/CppUTestExt/GTestConvertor.h
    include/CppUTestExt/MockActualCall.h
    include/CppUTestExt/MockCheckedActualCall.h
    include/CppUTestExt/MockNamedValue.h
    include/CppUTestExt/MockSupport.h
)

target_link_libraries(${CppUTestExtLibName} PUBLIC ${CppUTestLibName})

#[[Arrange for the include directory to be added to the include paths of any CMake target depending on CppUTestExt.]]
target_include_directories(${CppUTestExtLibName}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

add_library(CppUTest::CppUTestExt ALIAS ${CppUTestExtLibName})

if(PROJECT_IS_TOP_LEVEL)
    install(
        TARGETS ${CppUTestExtLibName}
        EXPORT CppUTestTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
    install(
        DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/CppUTestExt"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    )
endif()
