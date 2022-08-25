set(_DISCOVER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/../Scripts/CppUTestBuildTimeDiscoverTests.cmake")

# Create target to discover tests
function (cpputest_buildtime_discover_tests tgt)
  if(NOT TARGET ${tgt})
    message(FATAL_ERROR
      "Cannot discover tests for target \"${tgt}\" "
      "which is not built by this project."
    )
  endif()

  get_property(target_type
    TARGET ${tgt}
    PROPERTY TYPE
  )
  if(NOT target_type STREQUAL "EXECUTABLE")
    message(FATAL_ERROR
      "Cannot discover tests for target \"${tgt}\" "
      "which is not an executable."
    )
  endif()

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
