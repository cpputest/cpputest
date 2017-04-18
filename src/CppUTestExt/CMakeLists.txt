set(CppUTestExt_src
        CodeMemoryReportFormatter.cpp
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
)

add_library(CppUTestExt STATIC ${CppUTestExt_src})
target_link_libraries(CppUTestExt ${CPPUNIT_EXTERNAL_LIBRARIES})

target_include_directories(CppUTestExt PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include/CppUTestExt>
    $<INSTALL_INTERFACE:include>
    )

install(DIRECTORY ${CppUTestRootDirectory}/include/CppUTestExt DESTINATION include)
install(TARGETS CppUTestExt
    EXPORT CppUTestTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
