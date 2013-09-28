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
    )

set(CXX11_TEST_SOURCE
"

#include <array>
class foo
{
public:
    foo(int a, int b, int c, int d)
    {}

    virtual void override1()
    {}

    virtual void override2()
    {}

private:
    int data[4];
};

class bar : public foo
{
public:
    virtual void override1() override
    {}

    void override2() final
    {}
};

int main()
{
    int n[] = {4,7,6,1,2};
    for (auto i : n)
        foo a (3,5,1,2);
    return 0;
}
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
