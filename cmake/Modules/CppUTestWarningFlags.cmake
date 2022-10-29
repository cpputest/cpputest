if (MSVC)
else (MSVC)
    include(CheckCCompilerFlag)
    include(CheckCXXCompilerFlag)

    set(CMAKE_REQUIRED_FLAGS "-Wno-error")
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
        string(REPLACE "++" "xx" WARNING_CXX_FLAG_VAR "WARNING_CXX_FLAG_${flag}")
        check_cxx_compiler_flag("-${flag}" ${WARNING_CXX_FLAG_VAR})
        if (${WARNING_CXX_FLAG_VAR})
            set(CPPUTEST_CXX_WARNING_FLAGS "${CPPUTEST_CXX_WARNING_FLAGS} -${flag}")
        endif (${WARNING_CXX_FLAG_VAR})
      endforeach (flag)
    endmacro(check_and_append_cxx_warning_flags)

    set(WARNING_C_FLAGS
        Wall
        Wextra
        pedantic
        Wshadow
        Wswitch-default
        Wswitch-enum
        Wconversion
        Wsign-conversion
        Wno-padded
        Wno-disabled-macro-expansion
        Wno-reserved-id-macro
        Wno-keyword-macro
        Wno-long-long
        )

    set(WARNING_C_ONLY_FLAGS
        Wstrict-prototypes
        )

    set(WARNING_CXX_FLAGS
        ${WARNING_C_FLAGS}
        Woverloaded-virtual
        Wno-global-constructors
        Wno-exit-time-destructors
        Wno-weak-vtables
        Wno-old-style-cast
        )

    if (DEFINED CMAKE_CXX_STANDARD AND NOT CMAKE_CXX_STANDARD EQUAL 98)
        set(WARNING_CXX_FLAGS
            ${WARNING_CXX_FLAGS}
            Wno-c++98-compat
            Wno-c++98-compat-pedantic
            Wno-c++14-compat
            Wno-inconsistent-missing-destructor-override
            Wsuggest-override
        )
    endif ()

    check_and_append_c_warning_flags(${WARNING_C_FLAGS})
    check_and_append_c_warning_flags(${WARNING_C_ONLY_FLAGS})
    check_and_append_cxx_warning_flags(${WARNING_CXX_FLAGS})

endif (MSVC)
