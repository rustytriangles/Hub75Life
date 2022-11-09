# What CMake to start at
cmake_minimum_required(VERSION 3.12)

#set(CMAKE_C_COMPILER_FORCED TRUE)
#set(CMAKE_CXX_COMPILER_FORCED TRUE)
set(CMAKE_SYSTEM_NAME Generic)

enable_language( C CXX ASM )

set(OUTPUT_NAME Hub75Life)

#set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(${OUTPUT_NAME} VERSION 1.0.0)

#add_compile_options(--specs=nosys.specs)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

option(PICO_BUILD "Build for RasPi Pico" ON)
if (PICO_BUILD)
  include(pico)
endif()

option(LOCAL_BUILD "Build unittests locally" OFF)
if (LOCAL_BUILD)
  include(local)
endif()
