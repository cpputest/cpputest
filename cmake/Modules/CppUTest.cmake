set(_CPPUTEST_DISCOVERY_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/_CppUTestDiscovery.cmake
    CACHE INTERNAL "CppUTest discovery scripts"
)

option(CPPUTEST_TESTS_DETAILED "Run discovered tests individually")
option(CPPUTEST_JUNIT_REPORT "Output JUnit test reports")

function(cpputest_discover_tests target)
    set(options JUNIT)
    set(oneValueArgs DETAILED)
    set(multiValueArgs EXTRA_ARGS)
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

    if(NOT DEFINED _EXTRA_ARGS)
        set(_EXTRA_ARGS -v)
    endif()

    if(NOT DEFINED _DETAILED)
        set(_DETAILED ${CPPUTEST_TESTS_DETAILED})
    endif()

    if(_JUNIT OR CPPUTEST_JUNIT_REPORT)
        list(APPEND _EXTRA_ARGS -ojunit)
    endif()

    set(CTEST_INCLUDE_FILE "${CMAKE_CURRENT_BINARY_DIR}/${target}_include.cmake")
    if(CMAKE_CONFIGURATION_TYPES)
        set(CTEST_GENERATED_FILE "${CMAKE_CURRENT_BINARY_DIR}/${target}.$<CONFIG>.cmake")
        file(WRITE "${CTEST_INCLUDE_FILE}"
            "if(EXISTS \"${CMAKE_CURRENT_BINARY_DIR}/${target}.\${CTEST_CONFIGURATION_TYPE}.cmake\")\n"
            "  include(\"${CMAKE_CURRENT_BINARY_DIR}/${target}.\${CTEST_CONFIGURATION_TYPE}.cmake\")\n"
            "endif()\n"
        )
    else()
        set(CTEST_GENERATED_FILE "${CMAKE_CURRENT_BINARY_DIR}/${target}.cmake")
        file(WRITE "${CTEST_INCLUDE_FILE}"
            "if(EXISTS \"${CTEST_GENERATED_FILE}\")\n"
            "  include(\"${CTEST_GENERATED_FILE}\")\n"
            "endif()\n"
        )
    endif()

    add_custom_command(
        TARGET ${target} POST_BUILD
        BYPRODUCTS "${CTEST_GENERATED_FILE}"
        COMMAND
            "${CMAKE_COMMAND}"
            -D "TESTS_DETAILED:BOOL=${_DETAILED}"
            -D "EXECUTABLE=$<TARGET_FILE:${target}>"
            -D "EMULATOR=$<TARGET_PROPERTY:${target},CROSSCOMPILING_EMULATOR>"
            -D "ARGS=${_EXTRA_ARGS}"
            -D "CTEST_FILE=${CTEST_GENERATED_FILE}"
            -P "${_CPPUTEST_DISCOVERY_SCRIPT}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        DEPENDS "${_CPPUTEST_DISCOVERY_SCRIPT}"
        VERBATIM
    )

    if(${CMAKE_VERSION} VERSION_LESS "3.10")
        # We can only set one.
        get_property(already_set
            DIRECTORY
            PROPERTY TEST_INCLUDE_FILE
            SET
        )
        if(${already_set})
            message(FATAL_ERROR
                "Cannot discovery multiple tests from the same file"
            )
        endif()
        set_property(
            DIRECTORY
            PROPERTY TEST_INCLUDE_FILE "${CTEST_INCLUDE_FILE}"
        )
    else()
        set_property(
            DIRECTORY APPEND
            PROPERTY TEST_INCLUDE_FILES "${CTEST_INCLUDE_FILE}"
        )
    endif()

endfunction()
