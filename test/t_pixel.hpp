#include "../src/pixel.hpp"

BOOST_AUTO_TEST_SUITE(t_pixel)

BOOST_AUTO_TEST_CASE(pixel1) {

    // black
    BOOST_TEST(hsv_to_rgb(0.f , 0.f, 0.f) == Pixel(0.f, 0.f, 0.f));
    BOOST_TEST(hsv_to_rgb(0.5f, 0.f, 0.f) == Pixel(0.f, 0.f, 0.f));

    // white
    BOOST_TEST(hsv_to_rgb(0.f , 0.f, 1.f) == Pixel(1.f, 1.f, 1.f));
    BOOST_TEST(hsv_to_rgb(0.5f, 0.f, 1.f) == Pixel(1.f, 1.f, 1.f));

    // gray
    BOOST_TEST(hsv_to_rgb(0.f , 0.f, 0.5f) == Pixel(0.5f, 0.5f, 0.5f));
    BOOST_TEST(hsv_to_rgb(0.5f, 0.f, 0.5f) == Pixel(0.5f, 0.5f, 0.5f));

    // saturation == 1
    BOOST_TEST(hsv_to_rgb(0.f      , 1.f, 1.f) == Pixel(1.f, 0.f, 0.f));
//    BOOST_TEST(hsv_to_rgb(0.166667f, 1.f, 1.f) == Pixel(1.f, 1.f, 0.f)); // off-by-one
    BOOST_TEST(hsv_to_rgb(0.333333f, 1.f, 1.f) == Pixel(0.f, 1.f, 0.f));
    BOOST_TEST(hsv_to_rgb(0.5f     , 1.f, 1.f) == Pixel(0.f, 1.f, 1.f));
    BOOST_TEST(hsv_to_rgb(0.666667f, 1.f, 1.f) == Pixel(0.f, 0.f, 1.f));
//    BOOST_TEST(hsv_to_rgb(0.833333f, 1.f, 1.f) == Pixel(1.f, 0.f, 1.f)); // off-by-one
    BOOST_TEST(hsv_to_rgb(1.f      , 1.f, 1.f) == Pixel(1.f, 0.f, 0.f));

    // saturation == 1/2
    BOOST_TEST(hsv_to_rgb(0.f      , 0.5f, 1.f) == Pixel(1.f , 0.5f, 0.5f));
//    BOOST_TEST(hsv_to_rgb(0.166667f, 0.5f, 1.f) == Pixel(1.f, 1.f, 0.5f)); // off-by-one
    BOOST_TEST(hsv_to_rgb(0.333333f, 0.5f, 1.f) == Pixel(0.5f, 1.f , 0.5f));
    BOOST_TEST(hsv_to_rgb(0.5f     , 0.5f, 1.f) == Pixel(0.5f, 1.f , 1.f ));
    BOOST_TEST(hsv_to_rgb(0.666667f, 0.5f, 1.f) == Pixel(0.5f, 0.5f, 1.f ));
//    BOOST_TEST(hsv_to_rgb(0.833333f, 0.5f, 1.f) == Pixel(1.f, 0.5f, 1.f)); // off-by-one
    BOOST_TEST(hsv_to_rgb(1.f      , 0.5f, 1.f) == Pixel(1.f , 0.5f, 0.5f));

}
BOOST_AUTO_TEST_SUITE_END()
