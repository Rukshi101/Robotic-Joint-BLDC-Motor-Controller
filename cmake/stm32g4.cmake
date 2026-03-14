# Cortex-M4 with hardware FPU
set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

# Core compiler flags
set(CMAKE_C_FLAGS "${CPU_FLAGS} -fdata-sections -ffunction-sections -Wall" CACHE STRING "")
set(CMAKE_ASM_FLAGS "${CPU_FLAGS}" CACHE STRING "")

# Strip unused sections to save flash
set(CMAKE_EXE_LINKER_FLAGS "${CPU_FLAGS} -Wl,--gc-sections -Wl,--print-memory-usage -specs=nano.specs -specs=nosys.specs" CACHE STRING "")

# STM32G4 HAL defines
add_definitions(-DSTM32G431xx -DUSE_HAL_DRIVER)