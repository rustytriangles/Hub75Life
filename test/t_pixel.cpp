#include "../src/pixel.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("pixel") {

    // black
    REQUIRE(hsv_to_rgb(0.f , 0.f, 0.f) == Pixel(0.f, 0.f, 0.f));
    REQUIRE(hsv_to_rgb(0.5f, 0.f, 0.f) == Pixel(0.f, 0.f, 0.f));

    // white
    REQUIRE(hsv_to_rgb(0.f , 0.f, 1.f) == Pixel(1.f, 1.f, 1.f));
    REQUIRE(hsv_to_rgb(0.5f, 0.f, 1.f) == Pixel(1.f, 1.f, 1.f));

    // gray
    REQUIRE(hsv_to_rgb(0.f , 0.f, 0.5f) == Pixel(0.5f, 0.5f, 0.5f));
    REQUIRE(hsv_to_rgb(0.5f, 0.f, 0.5f) == Pixel(0.5f, 0.5f, 0.5f));

    // saturation == 1
    REQUIRE(hsv_to_rgb(0.f      , 1.f, 1.f) == Pixel(1.f, 0.f, 0.f));
//    BOOST_TEST(hsv_to_rgb(0.166667f, 1.f, 1.f) == Pixel(1.f, 1.f, 0.f)); // off-by-one
    REQUIRE(hsv_to_rgb(0.333333f, 1.f, 1.f) == Pixel(0.f, 1.f, 0.f));
    REQUIRE(hsv_to_rgb(0.5f     , 1.f, 1.f) == Pixel(0.f, 1.f, 1.f));
    REQUIRE(hsv_to_rgb(0.666667f, 1.f, 1.f) == Pixel(0.f, 0.f, 1.f));
//    BOOST_TEST(hsv_to_rgb(0.833333f, 1.f, 1.f) == Pixel(1.f, 0.f, 1.f)); // off-by-one
    REQUIRE(hsv_to_rgb(1.f      , 1.f, 1.f) == Pixel(1.f, 0.f, 0.f));

    // saturation == 1/2
    REQUIRE(hsv_to_rgb(0.f      , 0.5f, 1.f) == Pixel(1.f , 0.5f, 0.5f));
//    BOOST_TEST(hsv_to_rgb(0.166667f, 0.5f, 1.f) == Pixel(1.f, 1.f, 0.5f)); // off-by-one
    REQUIRE(hsv_to_rgb(0.333333f, 0.5f, 1.f) == Pixel(0.5f, 1.f , 0.5f));
    REQUIRE(hsv_to_rgb(0.5f     , 0.5f, 1.f) == Pixel(0.5f, 1.f , 1.f ));
    REQUIRE(hsv_to_rgb(0.666667f, 0.5f, 1.f) == Pixel(0.5f, 0.5f, 1.f ));
//    BOOST_TEST(hsv_to_rgb(0.833333f, 0.5f, 1.f) == Pixel(1.f, 0.5f, 1.f)); // off-by-one
    REQUIRE(hsv_to_rgb(1.f      , 0.5f, 1.f) == Pixel(1.f , 0.5f, 0.5f));

}
