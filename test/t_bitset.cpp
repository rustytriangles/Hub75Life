#define BOOST_TEST_MODULE t_bitset
#include <boost/test/included/unit_test.hpp>

#include "../src/bitset.hpp"

BOOST_AUTO_TEST_CASE(t_bitset) {
    BitSet b(64,64);

    b.set(12,12,true);
    BOOST_TEST(b.test(12,12));
}
