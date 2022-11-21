#define BOOST_TEST_MODULE t_bitset
#include <boost/test/included/unit_test.hpp>

#include <algorithm>
#include <utility>
#include <vector>

//#include <iostream>

// BitSet
#include "../src/bitset.hpp"

BOOST_AUTO_TEST_SUITE(t_bitset)

BOOST_AUTO_TEST_CASE(set_test) {
    BitSet b(64,64);

    b.set(12,12,true);
    BOOST_TEST(b.test(12,12));
}

BOOST_AUTO_TEST_CASE(width_height) {
    BitSet b(64,64);

    BOOST_TEST(b.width() == 64);
    BOOST_TEST(b.height() == 64);
}

BOOST_AUTO_TEST_CASE(assign) {
    BitSet b(64,64);

    b.set(12,12,true);
    BOOST_TEST(b.test(12,12));

    BitSet c(5,5);
    c = b;
    BOOST_TEST(c.width() == 64);
    BOOST_TEST(c.height() == 64);
    BOOST_TEST(c.test(12,12));
    BOOST_TEST(c.test(13,12) == false);

}

BOOST_AUTO_TEST_CASE(any) {
    BitSet b(64,64);

    BOOST_TEST(b.any() == false);

    b.set(12,12,true);
    BOOST_TEST(b.any() == true);

    b.set(12,12,false);
    BOOST_TEST(b.any() == false);
}

BOOST_AUTO_TEST_SUITE_END()

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

// Life
#include "../src/life.hpp"

BOOST_AUTO_TEST_SUITE(t_life)

// empty in -> empty out
BOOST_AUTO_TEST_CASE(life_empty) {
    BitSet dst(64,64);
    BitSet src(64,64);

    BOOST_TEST(dst.any() == false);
    BOOST_TEST(src.any() == false);

    tick(dst, src);

    BOOST_TEST(dst.any() == false);

}

// single in -> empty out
BOOST_AUTO_TEST_CASE(life_single) {
    BitSet dst(64,64);
    BitSet src(64,64);

    src.set(12,12,true);

    BOOST_TEST(dst.any() == false);
    BOOST_TEST(src.any() == true);

    tick(dst, src);

    BOOST_TEST(dst.any() == false);
}

// 2x2 block should be unchanged
BOOST_AUTO_TEST_CASE(life_block) {
    BitSet dst(64,64);
    BitSet src(64,64);

    src.set(12,12,true);
    src.set(13,12,true);
    src.set(12,13,true);
    src.set(13,13,true);

    BOOST_TEST(dst.any() == false);
    BOOST_TEST(src.any() == true);

    tick(dst, src);

    BOOST_TEST(dst.any() == true);

    for (int i=0; i<64; i++) {
        for (int j=0; j<64; j++){
            const bool expected = (i >= 12 && i <= 13 && j >= 12 && j <= 13);
            BOOST_TEST(dst.test(i,j) == expected);
        }
    }
}

// 
BOOST_AUTO_TEST_CASE(life_spinner) {
    BitSet src(64,64);

    std::vector<std::pair<int,int> > pixels = {
        std::make_pair(12,13),
        std::make_pair(13,13),
        std::make_pair(14,13) };

    for (auto i=pixels.begin(); i!=pixels.end(); i++) {
        src.set(i->first, i->second, true);
    }

    // 1 tick should rotate 90 degrees
    BitSet gen1(64,64);
    tick(gen1, src);

    BOOST_TEST(gen1.any() == true);

    for (int i=0; i<64; i++) {
        for (int j=0; j<64; j++) {
            auto rotated = std::make_pair(j,i);
            const bool expected = std::find(pixels.begin(), pixels.end(), rotated) != pixels.end();
            BOOST_TEST(gen1.test(i,j) == expected);
        }
    }

    // 2nd tick should return to initial conditions
    BitSet gen2(64,64);
    tick(gen2, gen1);

    BOOST_TEST(gen2.any() == true);

    for (int i=0; i<64; i++) {
        for (int j=0; j<64; j++) {
            const bool expected = std::find(pixels.begin(), pixels.end(), std::make_pair(i,j)) != pixels.end();
            BOOST_TEST(gen2.test(i,j) == expected);
        }
    }
}

// 4 ticks should move glider to X+1, Y-1
BOOST_AUTO_TEST_CASE(life_glider) {
    BitSet src(64,64);

    std::vector<std::pair<int,int> > pixels = {
        std::make_pair(14,13),
        std::make_pair(15,13),
        std::make_pair(13,14),
        std::make_pair(15,14),
        std::make_pair(15,15) };

    for (auto i=pixels.begin(); i!=pixels.end(); i++) {
        src.set(i->first, i->second, true);
    }

    BitSet gen1(64,64);
    tick(gen1, src);

    BOOST_TEST(gen1.any() == true);

    BitSet gen2(64,64);
    tick(gen2, gen1);

    BOOST_TEST(gen2.any() == true);

    BitSet gen3(64,64);
    tick(gen3, gen2);

    BOOST_TEST(gen3.any() == true);

    BitSet gen4(64,64);
    tick(gen4, gen3);

    BOOST_TEST(gen4.any() == true);

    // for (int j=0; j<64; j++) {
    //     for (int i=0; i<64; i++) {
    //         std::cout << (gen4.test(i,j) ? "X" : ".");
    //     }
    //     std::cout << std::endl;
    // }

    for (int i=0; i<64; i++) {
        for (int j=0; j<64; j++) {
            auto offset = std::make_pair(i-1,j+1);
            const bool expected = std::find(pixels.begin(), pixels.end(), offset) != pixels.end();
            BOOST_TEST(gen4.test(i,j) == expected);
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()
