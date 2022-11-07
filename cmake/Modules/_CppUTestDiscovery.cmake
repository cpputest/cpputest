set(script)

function(add_command NAME)
    set(_args "")
    # use ARGV* instead of ARGN, because ARGN splits arrays into multiple arguments
    math(EXPR _last_arg ${ARGC}-1)
    foreach(_n RANGE 1 ${_last_arg})
        set(_arg "${ARGV${_n}}")
        if(_arg MATCHES "[^-./:a-zA-Z0-9_]")
            set(_args "${_args} [==[${_arg}]==]") # form a bracket_argument
        else()
            set(_args "${_args} ${_arg}")
        endif()
    endforeach()
    set(script "${script}${NAME}(${_args})\n" PARENT_SCOPE)
endfunction()

if(NOT EXISTS "${EXECUTABLE}")
    message(FATAL_ERROR
        "Executable \"${EXECUTABLE}\" does not exist"
    )
endif()

if(TESTS_DETAILED)
    set(discovery_arg "-ln")
    set(select_arg "-st")
else()
    set(discovery_arg "-lg")
    set(select_arg "-sg")
endif()

execute_process(
    COMMAND ${EMULATOR} "${EXECUTABLE}" ${discovery_arg}
    OUTPUT_VARIABLE discovered_tests
    RESULT_VARIABLE result
    ERROR_VARIABLE error
)
if(NOT ${result} EQUAL 0)
    message(FATAL_ERROR
        "Error executing ${EXECUTABLE}:\n"
        "${error}"
    )
endif()
separate_arguments(discovered_tests)
foreach(test_name IN LISTS discovered_tests)
    add_command(
        add_test
        "${test_name}"
        ${EMULATOR}
        "${EXECUTABLE}"
        ${ARGS}
        ${select_arg}
        ${test_name}
    )
endforeach()

file(WRITE "${CTEST_FILE}" "${script}")
