# CMake Integration

## FetchContent

CMake can automatically download and integrate CppUTest with
[`FetchContent`](https://cmake.org/cmake/help/latest/module/FetchContent.html).
This is the recommended strategy for CMake integration.

```cmake
cmake_minimum_required(VERSION 3.11)
project(trying_CppUtest)

include(FetchContent)
FetchContent_Declare(
    CppUTest
    GIT_REPOSITORY https://github.com/cpputest/cpputest.git
    GIT_TAG        master # or use release tag, eg. v4.0
    # FIND_PACKAGE_ARGS # Recent CMake versions can integrate FetchContent with find_package
    # https://cmake.org/cmake/help/latest/module/FetchContent.html#integrating-with-find-package
)

# Either set CPPUTEST_PLATFORM to off and # provide a project-specific
# platform.cpp, assign # one of the provided platforms under
# src/Platforms/ # (as shown below), or let CppUTest
# select an appropriate default.
set(CPPUTEST_PLATFORM Gcc STRING BOOL "Platform implementation")

FetchContent_MakeAvailable(CppUTest)

add_executable(trying_CppUtest main.cpp)

target_link_libraries(trying_CppUtest
    PRIVATE
        CppUTest::CppUTest
        #or CppUTest::CppUTestExt
)

include(CTest) # Enable testing

# automatically discover CppUTest tests
include(CppUTest)
cpputest_discover_tests(trying_CppUtest)
```

## System install

If CppUTest has been installed on the system, it can be integrated with
[`find_package`](https://cmake.org/cmake/help/latest/command/find_package.html).
The target platform will be the same as the development host.
This is useful for managing a common
CppUTest installation with the system's package manager.

```cmake
cmake_minimum_required(VERSION 3.8)
project(trying_CppUtest)

find_package(CppUTest REQUIRED)

add_executable(trying_CppUtest main.cpp)

target_link_libraries(trying_CppUtest
    PRIVATE
        CppUTest
        #or CppUTestExt
)

include(CTest) # Enable testing

# automatically discover CppUTest tests
include(CppUTest)
cpputest_discover_tests(trying_CppUtest)
```

## Shared directory

It is possible to integrate CppUTest from a local directory. This may be useful
if CppUTest is being built for a target platform other than that of the
development host and CMake 3.11 is not available. The following is an example
how an external project can refer to this CMakeLists.txt to build CppUTest as a
library and include it as a target dependency.

```cmake
cmake_minimum_required(VERSION 3.8)
project(trying_CppUtest)

SET(CppUTestRootDirectory /path/to/cpputest)

# Either set CPPUTEST_PLATFORM to one of the provided platforms under
# /src/Platforms/, or provide a project-specific
# platform.cpp (as shown below)
set(CPPUTEST_PLATFORM OFF CACHE BOOL "Platform implementation")
add_subdirectory(${CppUTestRootDirectory} CppUTest)
target_sources(CppUTest
    PRIVATE
        ${PROJECT_SOURCE_DIR}/UtestPlatform.cpp
)

add_executable(trying_CppUtest main.cpp)
target_link_libraries(trying_CppUtest
    PRIVATE
        CppUTest::CppUTest
        #or CppUTest::CppUTestExt
)

include(CTest) # Enable testing

# automatically discover CppUTest tests
include(CppUTest)
cpputest_discover_tests(trying_CppUtest)
```
