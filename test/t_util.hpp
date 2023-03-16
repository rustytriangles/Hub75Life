#include "../src/util.hpp"

BOOST_AUTO_TEST_SUITE(t_util)

BOOST_AUTO_TEST_CASE(random) {

    rand_init();

    const float v1 = random_between(10.f, 12.f);
    
    BOOST_TEST(v1 >= 10.f);
    BOOST_TEST(v1 <= 12.f);

    const float v2 = random_between(-1.f, 1.f);
    
    BOOST_TEST(v2 >= -1.f);
    BOOST_TEST(v2 <= 1.f);
}

BOOST_AUTO_TEST_SUITE_END()
