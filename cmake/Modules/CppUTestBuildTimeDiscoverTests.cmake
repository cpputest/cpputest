option(CPPUTEST_TESTS_DETAILED "Run discovered tests individually")

set(_DISCOVER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/../Scripts/CppUTestBuildTimeDiscoverTests.cmake")

# Create target to discover tests
function (cpputest_buildtime_discover_tests tgt)
  message(DEPRECATION
    "Use cpputest_discover_tests from the CppUTest module instead"
  )

  set(options)
  set(oneValueArgs DETAILED)
  set(multiValueArgs)
  cmake_parse_arguments(
    ""
    "${options}"
    "${oneValueArgs}"
    "${multiValueArgs}"
    ${ARGN}
  )
  if(NOT DEFINED _DETAILED)
    set(_DETAILED ${CPPUTEST_TESTS_DETAILED})
  endif()

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
      -D "TESTS_DETAILED:BOOL=${_DETAILED}"
      -D "EXECUTABLE=$<TARGET_FILE:${tgt}>"
      -D "EMULATOR=$<TARGET_PROPERTY:${tgt},CROSSCOMPILING_EMULATOR>"
      -P "${_DISCOVER_SCRIPT}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Discovering Tests in ${tgt}"
    VERBATIM
  )
endfunction ()
