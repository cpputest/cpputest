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

if(LIBNAME_POSTFIX_DEBUG)
    set_target_properties(CppUTestExt PROPERTIES DEBUG_POSTFIX "d")
endif()

#[[Arrange for the include directory to be added to the include paths of any CMake target depending on CppUTestExt.]]
target_include_directories(CppUTestExt
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
