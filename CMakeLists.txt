# What CMake to start at
cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)

set(OUTPUT_NAME Hub75Life)

project(${OUTPUT_NAME} VERSION 1.0.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

option(PICO_BUILD "Build for RasPi Pico" ON)
if (PICO_BUILD)
  include(pico)
endif()

option(LOCAL_BUILD "Build unittests locally" OFF)
if (LOCAL_BUILD)
  include(local)
endif()
