message(STATUS "found local.cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")


add_executable(cpp_test test/t_bitset.cpp)

target_include_directories(cpp_test
PRIVATE
    /Users/mike/development/boost_1_80_0
)

enable_testing()
add_test(NAME cpp_test
         COMMAND $<TARGET_FILE:cpp_test>)

