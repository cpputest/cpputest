add_library(CppUTestExt STATIC
    CodeMemoryReportFormatter.cpp
    GTest.cpp
    IEEE754ExceptionsPlugin.cpp
    MemoryReporterPlugin.cpp
    MockFailure.cpp
    MockSupportPlugin.cpp
    MockActualCall.cpp
    MockSupport_c.cpp
    MemoryReportAllocator.cpp
    MockExpectedCall.cpp
    MockNamedValue.cpp
    OrderedTest.cpp
    MemoryReportFormatter.cpp
    MockExpectedCallsList.cpp
    MockSupport.cpp
    ${PROJECT_SOURCE_DIR}/include/CppUTestExt/CodeMemoryReportFormatter.h
    ${PROJECT_SOURCE_DIR}/include/CppUTestExt/IEEE754ExceptionsPlugin.h
    ${PROJECT_SOURCE_DIR}/include/CppUTestExt/MemoryReportAllocator.h
    ${PROJECT_SOURCE_DIR}/include/CppUTestExt/MockExpectedCall.h
    ${PROJECT_SOURCE_DIR}/include/CppUTestExt/MockCheckedExpectedCall.h
    ${PROJECT_SOURCE_DIR}/include/CppUTestExt/MockExpectedCallsList.h
    ${PROJECT_SOURCE_DIR}/include/CppUTestExt/MockSupportPlugin.h
    ${PROJECT_SOURCE_DIR}/include/CppUTestExt/MemoryReportFormatter.h
    ${PROJECT_SOURCE_DIR}/include/CppUTestExt/MockFailure.h
    ${PROJECT_SOURCE_DIR}/include/CppUTestExt/MockSupport.h
    ${PROJECT_SOURCE_DIR}/include/CppUTestExt/MockSupport_c.h
    ${PROJECT_SOURCE_DIR}/include/CppUTestExt/GMock.h
    ${PROJECT_SOURCE_DIR}/include/CppUTestExt/GTest.h
    ${PROJECT_SOURCE_DIR}/include/CppUTestExt/GTestSupport.h
    ${PROJECT_SOURCE_DIR}/include/CppUTestExt/MemoryReporterPlugin.h
    ${PROJECT_SOURCE_DIR}/include/CppUTestExt/OrderedTest.h
    ${PROJECT_SOURCE_DIR}/include/CppUTestExt/GTestConvertor.h
    ${PROJECT_SOURCE_DIR}/include/CppUTestExt/MockActualCall.h
    ${PROJECT_SOURCE_DIR}/include/CppUTestExt/MockCheckedActualCall.h
    ${PROJECT_SOURCE_DIR}/include/CppUTestExt/MockNamedValue.h
    ${PROJECT_SOURCE_DIR}/include/CppUTestExt/MockSupport.h
)

target_link_libraries(CppUTestExt PUBLIC CppUTest)

#[[Arrange for the include directory to be added to the include paths of any CMake target depending on CppUTestExt.]]
target_include_directories(CppUTestExt
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

add_library(CppUTest::CppUTestExt ALIAS CppUTestExt)

if(PROJECT_IS_TOP_LEVEL)
    install(
        TARGETS CppUTestExt
        EXPORT CppUTestTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
    install(
        DIRECTORY "${PROJECT_SOURCE_DIR}/include/CppUTestExt"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    )
endif()
