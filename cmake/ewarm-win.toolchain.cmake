### BEGIN CMAKE_TOOLCHAIN_FILE
# "Generic" is used when cross compiling
set(CMAKE_SYSTEM_NAME Generic)

# Set the EW installation root directory
#(Avoid spaces in the path or you need to escape them)
set(EW_ROOT_DIR "E:/Program Files\ (x86)/IAR\ Systems/Embedded\ Workbench\ Arm 8.50.6/arm")

# Compiler flags needed to compile for this CPU
set(CPU_FLAGS "--cpu Cortex-M4F")

# Set up the CMake variables for compiler and assembler
# (The reason for both C and CXX variables is that CMake
# treats C and C++ tools individually)
set(CMAKE_C_COMPILER "${EW_ROOT_DIR}/bin/iccarm.exe" "${CPU_FLAGS} --dlib_config normal")
set(CMAKE_CXX_COMPILER "${EW_ROOT_DIR}/bin/iccarm.exe" "${CPU_FLAGS} --dlib_config normal")
set(CMAKE_ASM_COMPILER "${EW_ROOT_DIR}/bin/iasmarm.exe" "${CPU_FLAGS}")

# For CppUTest, set the IAR variable
set(IAR True)

# Build with cmake -H. -Bbuild -G "NMake Makefiles" -DCMAKE_TOOLCHAIN_FILE=cmake/ewarm-win.toolchain.cmake

### END CMAKE_TOOLCHAIN_FILE
