# Toolchain to use Clang compiler with MinGW binutils (e.g. linker) in Windows to generate 64-bits executables

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_FLAGS "-target x86_64-w64-mingw32" CACHE STRING "" FORCE)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "-target x86_64-w64-mingw32" CACHE STRING "" FORCE)