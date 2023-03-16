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

BOOST_AUTO_TEST_CASE(life_size_mismatch) {
    BitSet dst(64,64);
    BitSet src(32,32);

    BOOST_CHECK_THROW(tick(dst, src), std::range_error);

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


// block should be stationary
BOOST_AUTO_TEST_CASE(life_block) {
    BitSet src(64,64);

    add_block_at(src, 8, 8);

    std::vector<std::pair<int,int> > pixels = {
        std::make_pair(8,8),
        std::make_pair(9,8),
        std::make_pair(8,9),
        std::make_pair(9,9) };

    for (int i=0; i<64; i++) {
        for (int j=0; j<64; j++) {
            auto offset = std::make_pair(i,j);
            const bool expected = std::find(pixels.begin(), pixels.end(), offset) != pixels.end();
            BOOST_TEST(src.test(i,j) == expected);
        }
    }

    BitSet gen1(64,64);
    tick(gen1, src);

    for (int i=0; i<64; i++) {
        for (int j=0; j<64; j++) {
            auto offset = std::make_pair(i,j);
            const bool expected = std::find(pixels.begin(), pixels.end(), offset) != pixels.end();
            BOOST_TEST(gen1.test(i,j) == expected);
        }
    }
}

// 4 ticks should move glider to X+1, Y-1
BOOST_AUTO_TEST_CASE(life_glider) {
    BitSet src(64,64);

    add_glider_at(src, 13, 13);

    std::vector<std::pair<int,int> > pixels = {
        std::make_pair(14,13),
        std::make_pair(15,13),
        std::make_pair(13,14),
        std::make_pair(15,14),
        std::make_pair(15,15) };

    for (int i=0; i<64; i++) {
        for (int j=0; j<64; j++) {
            auto offset = std::make_pair(i,j);
            const bool expected = std::find(pixels.begin(), pixels.end(), offset) != pixels.end();
            BOOST_TEST(src.test(i,j) == expected);
        }
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

#ifdef DUMP
    std::cout << gen4;
#endif

    for (int i=0; i<64; i++) {
        for (int j=0; j<64; j++) {
            auto offset = std::make_pair(i-1,j+1);
            const bool expected = std::find(pixels.begin(), pixels.end(), offset) != pixels.end();
            BOOST_TEST(gen4.test(i,j) == expected);
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()
