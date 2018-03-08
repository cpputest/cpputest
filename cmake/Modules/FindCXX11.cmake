# - Finds if the compiler has C++11 support
# This module can be used to detect compiler flags for using C++11, and checks
# a small subset of the language.
#
# The following variables are set:
#   CXX11_FLAGS - flags to add to the CXX compiler for C++11 support
#   CXX11_FOUND - true if the compiler supports C++11
#
# TODO: When compilers starts implementing the whole C++11, check the full set

include(CheckCXXSourceCompiles)
include(FindPackageHandleStandardArgs)

set(CXX11_FLAG_CANDIDATES
    # Eveerything that automatically accepts C++11
    " "
    # gcc, clang and Intel Linux
    "-std=c++11"
    # Intel windows
    "/Qstd=c++0x"
    "/Qstd=c++11"
    )

set(CXX11_TEST_SOURCE
"
#if defined(_MSC_VER)
  #if _MSC_VER < 1800
    #error \"Can not compile with C++11\"
  #endif
#elif __cplusplus < 201103L
  #error \"Can not compile with C++11\"
#endif

int main()
{}
")

foreach(FLAG ${CXX11_FLAG_CANDIDATES})
    set(SAFE_CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS}")
    set(CMAKE_REQUIRED_FLAGS "${FLAG}")
    unset(CXX11_FLAG_DETECTED CACHE)
    message(STATUS "Try C++11 flag = [${FLAG}]")
    check_cxx_source_compiles("${CXX11_TEST_SOURCE}" CXX11_FLAG_DETECTED)
    set(CMAKE_REQUIRED_FLAGS "${SAFE_CMAKE_REQUIRED_FLAGS}")
    if(CXX11_FLAG_DETECTED)
        set(CXX11_FLAGS_INTERNAL "${FLAG}")
        break()
    endif(CXX11_FLAG_DETECTED)
endforeach(FLAG ${CXX11_FLAG_CANDIDATES})

set(CXX11_FLAGS "${CXX11_FLAGS_INTERNAL}")

find_package_handle_standard_args(CXX11 DEFAULT_MSG CXX11_FLAGS)
mark_as_advanced(CXX11_FLAGS)
