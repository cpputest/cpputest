# Override output properties to put test executable at specificied location
function (cpputest_normalize_test_output_location TEST_TARGET)
    set_target_properties(${TEST_TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${OUTPUT_CONFIG} OUTPUT_CONFIG)
        set_target_properties(${TEST_TARGET} PROPERTIES
                              RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_CURRENT_BINARY_DIR})
    endforeach(OUTPUT_CONFIG)
endfunction ()

