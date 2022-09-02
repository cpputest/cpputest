cmake_minimum_required(VERSION 3.8)

project(CppUTestExample)

if (CMAKE_VERSION GREATER_EQUAL "3.21")
  # PROJECT_IS_TOP_LEVEL is defined automatically
elseif(CMAKE_CURRENT_BINARY_DIR STREQUAL CMAKE_BINARY_DIR)
  set(PROJECT_IS_TOP_LEVEL TRUE)
else()
  set(PROJECT_IS_TOP_LEVEL FALSE)
endif()

if(PROJECT_IS_TOP_LEVEL)
  find_package(CppUTest 4.0 REQUIRED)
endif()

include(CTest)

add_subdirectory(AllTests)
add_subdirectory(ApplicationLib)
