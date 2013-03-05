set(CppUTestExt_src
        CodeMemoryReportFormatter.cpp
        MemoryReporterPlugin.cpp
        MockFailure.cpp
        MockSupportPlugin.cpp
        GTestConvertor.cpp
        MockActualFunctionCall.cpp
        MockFunctionCall.cpp
        MockSupport_c.cpp
        MemoryReportAllocator.cpp
        MockExpectedFunctionCall.cpp
        MockNamedValue.cpp
        OrderedTest.cpp
        MemoryReportFormatter.cpp
        MockExpectedFunctionsList.cpp
        MockSupport.cpp
)

set(CppUTestExt_headers
        ${CppUTestRootDirectory}/include/CppUTestExt/MemoryReportAllocator.h
        ${CppUTestRootDirectory}/include/CppUTestExt/MockExpectedFunctionsList.h
        ${CppUTestRootDirectory}/include/CppUTestExt/MockSupportPlugin.h
        ${CppUTestRootDirectory}/include/CppUTestExt/MemoryReportFormatter.h
        ${CppUTestRootDirectory}/include/CppUTestExt/MockFailure.h
        ${CppUTestRootDirectory}/include/CppUTestExt/MockSupport_c.h
        ${CppUTestRootDirectory}/include/CppUTestExt/GMock.h
        ${CppUTestRootDirectory}/include/CppUTestExt/MemoryReporterPlugin.h
        ${CppUTestRootDirectory}/include/CppUTestExt/MockFunctionCall.h
        ${CppUTestRootDirectory}/include/CppUTestExt/OrderedTest.h
        ${CppUTestRootDirectory}/include/CppUTestExt/GTestConvertor.h
        ${CppUTestRootDirectory}/include/CppUTestExt/MockActualFunctionCall.h
        ${CppUTestRootDirectory}/include/CppUTestExt/MockNamedValue.h
)

add_library(CppUTestExt STATIC ${CppUTestExt_src})
target_link_libraries(CppUTestExt ${CPPUNIT_EXTERNAL_LIBRARIES})
install(FILES ${CppUTestExt_headers} DESTINATION include/CppUTestExt)
install(TARGETS CppUTestExt
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
