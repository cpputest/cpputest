if (MSVC)
    set(CPPUTEST_C_WARNING_FLAGS "/WX")
    set(CPPUTEST_CXX_WARNING_FLAGS "/WX /wd4290")
else (MSVC)
    include(CheckCCompilerFlag)
    include(CheckCXXCompilerFlag)

    macro(check_and_append_c_warning_flags)
      foreach (flag ${ARGN})
        check_c_compiler_flag("-${flag}" WARNING_C_FLAG_${flag})
        if (WARNING_C_FLAG_${flag})
            set(CPPUTEST_C_WARNING_FLAGS "${CPPUTEST_C_WARNING_FLAGS} -${flag}")
        endif (WARNING_C_FLAG_${flag})
      endforeach (flag)
    endmacro(check_and_append_c_warning_flags)

    macro(check_and_append_cxx_warning_flags)
      foreach (flag ${ARGN})
        check_c_compiler_flag("-${flag}" WARNING_CXX_FLAG_${flag})
        if (WARNING_CXX_FLAG_${flag})
            set(CPPUTEST_CXX_WARNING_FLAGS "${CPPUTEST_CXX_WARNING_FLAGS} -${flag}")
        endif (WARNING_CXX_FLAG_${flag})
      endforeach (flag)
    endmacro(check_and_append_cxx_warning_flags)

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
        Wno-disabled-macro-expansion
        )

    set(WARNING_C_ONLY_FLAGS
        Wstrict-prototypes
        )

    set(WARNING_CXX_ONLY_FLAGS
        Wno-global-constructors
        Wno-exit-time-destructors
        Wno-weak-vtables
        )

    check_and_append_c_warning_flags(${WARNING_C_FLAGS})
    check_and_append_c_warning_flags(${WARNING_C_ONLY_FLAGS})
    check_and_append_cxx_warning_flags(${WARNING_C_FLAGS})
    check_and_append_cxx_warning_flags(${WARNING_CXX_ONLY_FLAGS})

endif (MSVC)
