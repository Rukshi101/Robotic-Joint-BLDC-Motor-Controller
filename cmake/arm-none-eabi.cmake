# Cross-compilation setup - target is ARM not host PC
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Use ARM gcc instead of default PC compiler
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_SIZE arm-none-eabi-size)

# Skip compiler test - ARM binary won't run on PC
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)