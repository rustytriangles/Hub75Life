# What CMake to start at
cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)

# Include the Pico SDK cmake file
# Include the subsidiary .cmake file to get the SDK
include(/Users/mike/development/pico-sdk/external/pico_sdk_import.cmake)

set(OUTPUT_NAME Hub75Life)

project(${OUTPUT_NAME} VERSION 1.0.0)

# Link the Project to a source file (step 4.6)
add_executable(${OUTPUT_NAME} hub75life.cpp)

# Link the Project to an extra library (pico_stdlib)
target_link_libraries(${OUTPUT_NAME}
    pico_stdlib
    pico_multicore
)

# Initalise the SDK
pico_sdk_init()

# Enable USB, UART output
pico_enable_stdio_usb(${OUTPUT_NAME} 1)
pico_enable_stdio_uart(${OUTPUT_NAME} 1)

# Enable extra outputs (SWD?)
pico_add_extra_outputs(${OUTPUT_NAME})
