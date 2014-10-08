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
        check_cxx_compiler_flag("-${flag}" WARNING_CXX_FLAG_${flag})
        if (WARNING_CXX_FLAG_${flag})
            set(CPPUTEST_CXX_WARNING_FLAGS "${CPPUTEST_CXX_WARNING_FLAGS} -${flag}")
        endif (WARNING_CXX_FLAG_${flag})
      endforeach (flag)
    endmacro(check_and_append_cxx_warning_flags)

    set(WARNING_C_FLAGS
        Weverything
        Wall
        Wextra
        Wshadow
        Wswitch-default
        Wswitch-enum
        Wconversion
        Wsign-conversion
        Wno-padded
        Wno-disabled-macro-expansion
        )

    if (NOT GMOCK AND NOT REAL_GTEST)
      list(APPEND WARNING_C_FLAGS Werror pedantic-errors)
    endif (NOT GMOCK AND NOT REAL_GTEST)

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

    check_and_append_c_warning_flags(${WARNING_C_FLAGS})
    check_and_append_c_warning_flags(${WARNING_C_ONLY_FLAGS})
    check_and_append_cxx_warning_flags(${WARNING_CXX_FLAGS})

    if (C++11)
        check_cxx_compiler_flag("-Wno-c++98-compat" NO_WARNING_CXX_98_COMPAT_FLAG)
        if (NO_WARNING_CXX_98_COMPAT_FLAG)
            set(CPPUTEST_CXX_WARNING_FLAGS "${CPPUTEST_CXX_WARNING_FLAGS} -Wno-c++98-compat")
        endif (NO_WARNING_CXX_98_COMPAT_FLAG)
    endif (C++11)
endif (MSVC)
