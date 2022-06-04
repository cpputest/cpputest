set(_DISCOVER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/../Scripts/CppUTestBuildTimeDiscoverTests.cmake")

# Create target to discover tests
function (cpputest_buildtime_discover_tests tgt)
  add_custom_command(
    TARGET ${tgt} POST_BUILD
    COMMAND
      ${CMAKE_COMMAND}
      -D "TESTS_DETAILED:BOOL=${TESTS_DETAILED}"
      -D "EXECUTABLE=$<TARGET_FILE:${tgt}>"
      -D "EMULATOR=$<TARGET_PROPERTY:${tgt},CROSSCOMPILING_EMULATOR>"
      -P "${_DISCOVER_SCRIPT}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Discovering Tests in ${tgt}"
    VERBATIM
  )
endfunction ()
