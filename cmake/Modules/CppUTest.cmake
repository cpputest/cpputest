option(CPPUTEST_TESTS_DETAILED "Run discovered tests individually")

function(cpputest_discover_tests target)
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

    # Validate arguments.
    if(NOT TARGET ${target})
        message(FATAL_ERROR
            "Cannot discover tests for target \"${target}\" "
            "which is not built by this project."
        )
    endif()

    get_property(target_type
        TARGET ${target}
        PROPERTY TYPE
    )
    if(NOT target_type STREQUAL "EXECUTABLE")
        message(FATAL_ERROR
        "Cannot discover tests for target \"${target}\" "
        "which is not an executable."
        )
    endif()

    if(NOT DEFINED _DETAILED)
        set(_DETAILED ${CPPUTEST_TESTS_DETAILED})
    endif()

    add_custom_command(
        TARGET ${target} POST_BUILD
        BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/CTestTestfile.cmake"
        COMMAND
            "${CMAKE_COMMAND}"
            -D "TESTS_DETAILED:BOOL=${_DETAILED}"
            -D "EXECUTABLE=$<TARGET_FILE:${target}>"
            -D "EMULATOR=$<TARGET_PROPERTY:${target},CROSSCOMPILING_EMULATOR>"
            -P "${_CPPUTEST_DISCOVERY_SCRIPT}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        VERBATIM
    )
endfunction()

set(_CPPUTEST_DISCOVERY_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/_CppUTestDiscovery.cmake
    CACHE INTERNAL "CppUTest discovery scripts"
)
