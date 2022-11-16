# Helper to handle generating a map file, which is annoyingly tricky.
function(add_mapfile target)
    if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.13")
        set(mapfile "$<TARGET_FILE:${target}>.map")
        if(CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
            # ClangCL (lld-link) can't generate map files
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "IAR")
            target_link_options(${target} PRIVATE "SHELL:--map ${mapfile}.map")
        elseif(MSVC)
            target_link_options(${target} PRIVATE "/MAP:${mapfile}")
        elseif(
            (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") OR
            (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        )
            target_link_options(${target} PRIVATE "LINKER:-Map=${mapfile}")
        endif()
    endif()
endfunction()

add_subdirectory(CppUTest)
if (CPPUTEST_EXTENSIONS)
    add_subdirectory(CppUTestExt)
endif ()
