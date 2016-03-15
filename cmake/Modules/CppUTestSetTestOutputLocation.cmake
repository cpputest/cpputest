# Override output properties to put test executable at specificied location
function (cpputest_set_test_output_location TEST_TARGET OUTPUT_LOCATION)
    set_target_properties(${TEST_TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_LOCATION})
    foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${OUTPUT_CONFIG} OUTPUT_CONFIG)
        set_target_properties(${TEST_TARGET} PROPERTIES
                              RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${OUTPUT_LOCATION})
    endforeach(OUTPUT_CONFIG)
endfunction ()

