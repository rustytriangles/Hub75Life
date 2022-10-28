# What CMake to start at
cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)

# Include the subsidiary .cmake file to get the SDK
include(pico_sdk_import.cmake)

set(OUTPUT_NAME Hub75Life)

project(${OUTPUT_NAME} VERSION 1.0.0 LANGUAGES CXX)

set(PIMORONI_DIR /Users/mike/development/pimoroni-pico)

# Link the Project to a source file (step 4.6)
add_executable(${OUTPUT_NAME} hub75life.cpp)

# Link the Project to an extra library (pico_stdlib)
target_link_libraries(${OUTPUT_NAME}
    pico_stdlib
    pico_multicore
)

# target_include_directories(${OUTPUT_NAME}
#     ${PICO_SDK_PATH}/src/common/pico_stdlib/include
#     ${PICO_SDK_PATH}/src/common/pico_base/include
# )

target_include_directories(${OUTPUT_NAME} PRIVATE ${PIMORONI_DIR})

# Initalise the SDK
pico_sdk_init()

# Enable USB, UART output
pico_enable_stdio_usb(${OUTPUT_NAME} 1)
pico_enable_stdio_uart(${OUTPUT_NAME} 1)

# Enable extra outputs (SWD?)
pico_add_extra_outputs(${OUTPUT_NAME})
