message("found pico.cmake")

# Include the Pico SDK cmake file
# Include the subsidiary .cmake file to get the SDK
list(APPEND CMAKE_MODULE_PATH "/Users/mike/development/pico-sdk/external")
include(pico_sdk_import)

# Link the Project to a source file (step 4.6)
add_executable(${OUTPUT_NAME} hub75life.cpp)

# Link the Project to an extra library (pico_stdlib)
target_link_libraries(${OUTPUT_NAME}
    pico_stdlib
    pico_multicore
)

# Initalise the SDK
pico_sdk_init()
