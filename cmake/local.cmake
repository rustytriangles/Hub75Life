message(STATUS "found local.cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

find_package(Catch2 3 REQUIRED)

add_executable(cpp_test
    test/t_bitmap.cpp
    test/t_pixel.cpp
    test/t_life.cpp
    test/t_util.cpp

    src/pixel.cpp
    src/bitset.cpp
    src/life.cpp
    src/util.cpp
)

target_link_libraries(cpp_test PRIVATE Catch2::Catch2WithMain)

enable_testing()
add_test(NAME cpp_test
         COMMAND $<TARGET_FILE:cpp_test>)

