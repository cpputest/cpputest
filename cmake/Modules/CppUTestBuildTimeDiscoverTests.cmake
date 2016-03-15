# Create target to discover tests
function (cpputest_buildtime_discover_tests EXECUTABLE)
    add_custom_command (TARGET ${EXECUTABLE}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -DTESTS_DETAILED:BOOL=${TESTS_DETAILED} -DEXECUTABLE=${EXECUTABLE} -P ${PROJECT_SOURCE_DIR}/cmake/Scripts/CppUTestBuildTimeDiscoverTests.cmake
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Discovering Tests in ${EXECUTABLE}"
			VERBATIM)
endfunction ()
