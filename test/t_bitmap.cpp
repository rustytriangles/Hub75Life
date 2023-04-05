#include "../src/bitset.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("set_test") {
    BitSet b(64,64);

    b.set(12,12,true);
    REQUIRE(b.test(12,12));
}

TEST_CASE("width_height") {
    BitSet b(64,64);

    REQUIRE(b.width() == 64);
    REQUIRE(b.height() == 64);
}

TEST_CASE("range_errors") {
    BitSet b(64,64);

    REQUIRE_THROWS_AS(b.test(-1,12), std::out_of_range);
    REQUIRE_THROWS_AS(b.test(64,12), std::out_of_range);
    REQUIRE_THROWS_AS(b.test(12,-1), std::out_of_range);
    REQUIRE_THROWS_AS(b.test(12,64), std::out_of_range);

    REQUIRE_THROWS_AS(b.set(-1,12,true), std::out_of_range);
    REQUIRE_THROWS_AS(b.set(64,12,true), std::out_of_range);
    REQUIRE_THROWS_AS(b.set(12,-1,true), std::out_of_range);
    REQUIRE_THROWS_AS(b.set(12,64,true), std::out_of_range);
}

TEST_CASE("assign") {
    BitSet b(64,64);

    b.set(12,12,true);
    REQUIRE(b.test(12,12));

    BitSet c(5,5);
    c = b;
    REQUIRE(c.width() == 64);
    REQUIRE(c.height() == 64);
    REQUIRE(c.test(12,12));
    REQUIRE(c.test(13,12) == false);

}

TEST_CASE("any") {
    BitSet b(64,64);

    REQUIRE(b.any() == false);

    b.set(12,12,true);
    REQUIRE(b.any() == true);

    b.set(12,12,false);
    REQUIRE(b.any() == false);
}

