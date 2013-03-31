if (MSVC)
    set(CPPUTEST_C_WARNING_FLAGS "/WX")
    set(CPPUTEST_CXX_WARNING_FLAGS "/WX /wd4290")
else (MSVC)
    include(CheckCCompilerFlag)
    include(CheckCXXCompilerFlag)

    set(WARNING_C_FLAGS
        Werror
        Weverything
        Wall
        Wextra
        Wshadow
        Wswitch-default
        Wswitch-enum
        Wconversion
        pedantic-errors
        Wsign-conversion
        Woverloaded-virtual
        Wno-padded
        )

    set(WARNING_C_ONLY_FLAGS
        Wstrict-prototypes
        )

    set(WARNING_CXX_FLAGS
        Wno-global-constructors
        Wno-exit-time-destructors
        Wno-weak-vtables
        )

    foreach (flag ${WARNING_C_FLAGS})
        check_c_compiler_flag("-${flag}" WARNING_FLAG_${flag})
        if (WARNING_FLAG_${flag})
            set(CPPUTEST_C_WARNING_FLAGS "${CPPUTEST_C_WARNING_FLAGS} -${flag}")
        endif (WARNING_FLAG_${flag})
    endforeach (flag ${WARNING_C_FLAGS})
    check_c_compiler_flag(-Wno-disabled-macro-expansion WARNING_FLAG_no_disabled_macro_expansion)
    if (WARNING_FLAG_no_disabled_macro_expansion)
        set(CPPUTEST_C_WARNING_FLAGS "${CPPUTEST_C_WARNING_FLAGS} -Wno-disabled-macro-expansion")
    endif (WARNING_FLAG_no_disabled_macro_expansion)

    set(CPPUTEST_CXX_WARNING_FLAGS "${CPPUTEST_CXX_WARNING_FLAGS} ${CPPUTEST_C_WARNING_FLAGS}")
    check_cxx_compiler_flag(-Wno-global-constructors WARNING_FLAG_no_global_constructors)
    if (WARNING_FLAG_no_global_constructors)
        set(CPPUTEST_CXX_WARNING_FLAGS "${CPPUTEST_CXX_WARNING_FLAGS} -Wno-global-constructors")
    endif (WARNING_FLAG_no_global_constructors)

    check_cxx_compiler_flag(-Wno-exit-time-destructors WARNING_FLAG_no_exit_time_destructors)
    if (WARNING_FLAG_no_exit_time_destructors)
        set(CPPUTEST_CXX_WARNING_FLAGS "${CPPUTEST_CXX_WARNING_FLAGS} -Wno-exit-time-destructors")
    endif (WARNING_FLAG_no_exit_time_destructors)

    check_cxx_compiler_flag(-Wno-weak-vtables WARNING_FLAG_no_weak_vtables)
    if (WARNING_FLAG_no_weak_vtables)
        set(CPPUTEST_CXX_WARNING_FLAGS "${CPPUTEST_CXX_WARNING_FLAGS} -Wno-weak-vtables")
    endif (WARNING_FLAG_no_weak_vtables)

    foreach (flag ${WARNING_C_ONLY_FLAGS})
        check_c_compiler_flag("-${flag}" WARNING_FLAG_${flag})
        if (WARNING_FLAG_${flag})
            set(CPPUTEST_C_WARNING_FLAGS "${CPPUTEST_C_WARNING_FLAGS} -${flag}")
        endif (WARNING_FLAG_${flag})
    endforeach (flag ${WARNING_C_ONLY_FLAGS})
endif (MSVC)
