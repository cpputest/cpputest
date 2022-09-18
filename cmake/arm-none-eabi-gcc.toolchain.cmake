set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m4)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)

set(CMAKE_C_FLAGS_INIT "-mcpu=cortex-m4")
set(CMAKE_CXX_FLAGS_INIT "-mcpu=cortex-m4")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-specs=rdimon.specs")
