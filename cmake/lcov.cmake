if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(FATAL_ERROR
        "Coverage requires GCC or Clang (got ${CMAKE_CXX_COMPILER_ID})")
endif()

find_program(LCOV lcov REQUIRED)
find_program(GENHTML genhtml REQUIRED)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CTestCustom.cmake.in"
    "${CMAKE_BINARY_DIR}/CTestCustom.cmake"
    @ONLY
)
