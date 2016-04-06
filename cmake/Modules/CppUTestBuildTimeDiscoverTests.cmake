# Create target to discover tests
function (cpputest_buildtime_discover_tests EXECUTABLE)
  # The path to the discover script depends on execution mode:
  # - internal (building CppUTest it self).
  # - imported (installed, imported, and executed by a client of the CppUTest lib)
  if (PROJECT_NAME STREQUAL "CppUTest") # internal - (path is relative to source dir)
    SET(DISCOVER_SCRIPT ${PROJECT_SOURCE_DIR}/cmake/Scripts/CppUTestBuildTimeDiscoverTests.cmake)
  else (PROJECT_NAME STREQUAL "CppUTest") # Installed (path is relative to install directory)
    SET(DISCOVER_SCRIPT ${CppUTest_DIR}/Scripts/CppUTestBuildTimeDiscoverTests.cmake)
  endif (PROJECT_NAME STREQUAL "CppUTest")

  add_custom_command (TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DTESTS_DETAILED:BOOL=${TESTS_DETAILED} -DEXECUTABLE=$<TARGET_FILE:${EXECUTABLE}> -P ${DISCOVER_SCRIPT}
    VERBATIM
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Discovering Tests in ${EXECUTABLE}"
    VERBATIM)
endfunction ()
