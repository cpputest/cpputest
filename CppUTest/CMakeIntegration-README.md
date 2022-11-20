It is possible to integrate CppUTest as a sub-module of an enclosing CMake
project. This may be useful if CppUTest is being built for a target platform
other than that of the development host. The following is an example how an
external project can refer to this CMakeLists.txt to build CppUTest as a
library and include it as a target dependency.

```cmake
cmake_minimum_required(VERSION 3.7)
project(trying_CppUtest)

SET(CppUTestRootDirectory /path/to/cpputest)

# Either set CPP_PLATFORM to one of the provided platforms under
# ${CppUTestRootDirectory}/src/Platforms/, or provide a project-specific
# platform.cpp (as shown below)
add_subdirectory(${CppUTestRootDirectory}/src/CppUTest CppUTest)
target_sources(CppUTest
	PRIVATE
		${PROJECT_SOURCE_DIR}/UtestPlatform.cpp
)

add_executable(trying_CppUtest main.cpp)
target_link_libraries(trying_CppUtest CppUTest)
```
