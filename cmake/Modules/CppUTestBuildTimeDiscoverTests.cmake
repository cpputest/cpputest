# Create target to discover tests
function (cpputest_buildtime_discover_tests EXECUTABLE)
    # The that to the discover script depends execution mode:
    # - imported (installed, imported, and executed by a client of the CppUTest lib) or
    # - internal (building CppUTest it self).
    if (CppUTest_DIR) # Installed (path is relative to install directory)
        SET(DISCOVER_SCRIPT ${CppUTest_DIR}/Scripts/CppUTestBuildTimeDiscoverTests.cmake)
    else (CppUTest_DIR) # internal - (path is relative to source dir)
        SET(DISCOVER_SCRIPT ${PROJECT_SOURCE_DIR}/cmake/Scripts/CppUTestBuildTimeDiscoverTests.cmake)
    endif (CppUTest_DIR)

    add_custom_command (TARGET ${EXECUTABLE}
                        POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -DTESTS_DETAILED:BOOL=${TESTS_DETAILED} -DEXECUTABLE=${EXECUTABLE} -P ${DISCOVER_SCRIPT}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Discovering Tests in ${EXECUTABLE}"
			VERBATIM)
endfunction ()
