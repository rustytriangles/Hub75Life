#include <algorithm>
#include <utility>
#include <vector>

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

BOOST_AUTO_TEST_CASE(range_errors) {
    BitSet b(64,64);

    BOOST_CHECK_THROW(b.test(-1,12), std::out_of_range);
    BOOST_CHECK_THROW(b.test(64,12), std::out_of_range);
    BOOST_CHECK_THROW(b.test(12,-1), std::out_of_range);
    BOOST_CHECK_THROW(b.test(12,64), std::out_of_range);

    BOOST_CHECK_THROW(b.set(-1,12,true), std::out_of_range);
    BOOST_CHECK_THROW(b.set(64,12,true), std::out_of_range);
    BOOST_CHECK_THROW(b.set(12,-1,true), std::out_of_range);
    BOOST_CHECK_THROW(b.set(12,64,true), std::out_of_range);
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
