#include "../src/util.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("random") {

    rand_init();

    const float v1 = random_between(10.f, 12.f);

    REQUIRE(v1 >= 10.f);
    REQUIRE(v1 <= 12.f);

    const float v2 = random_between(-1.f, 1.f);

    REQUIRE(v2 >= -1.f);
    REQUIRE(v2 <= 1.f);
}

TEST_CASE("grid_to_framebuffer") {
    const uint32_t GRW = 128;
    const uint32_t GRH = 128;
    const unsigned int FBW = 256;
    const unsigned int FBH = 64;

    REQUIRE(grid_to_framebuffer(  0,  0, GRW,GRH,FBW,FBH) == std::make_pair(  0u, 0u));
    REQUIRE(grid_to_framebuffer( 63,  0, GRW,GRH,FBW,FBH) == std::make_pair( 63u, 0u));
    REQUIRE(grid_to_framebuffer(  0, 63, GRW,GRH,FBW,FBH) == std::make_pair(  0u,63u));
    REQUIRE(grid_to_framebuffer( 63, 63, GRW,GRH,FBW,FBH) == std::make_pair( 63u,63u));

    REQUIRE(grid_to_framebuffer( 64,  0, GRW,GRH,FBW,FBH) == std::make_pair( 64u, 0u));
    REQUIRE(grid_to_framebuffer(127,  0, GRW,GRH,FBW,FBH) == std::make_pair(127u, 0u));
    REQUIRE(grid_to_framebuffer( 64, 63, GRW,GRH,FBW,FBH) == std::make_pair( 64u,63u));
    REQUIRE(grid_to_framebuffer(127, 63, GRW,GRH,FBW,FBH) == std::make_pair(127u,63u));

    REQUIRE(grid_to_framebuffer(  0, 64, GRW,GRH,FBW,FBH) == std::make_pair(255u,63u));
    REQUIRE(grid_to_framebuffer( 63, 64, GRW,GRH,FBW,FBH) == std::make_pair(192u,63u));
    REQUIRE(grid_to_framebuffer(  0,127, GRW,GRH,FBW,FBH) == std::make_pair(255u, 0u));
    REQUIRE(grid_to_framebuffer( 63,127, GRW,GRH,FBW,FBH) == std::make_pair(192u, 0u));

    REQUIRE(grid_to_framebuffer( 64, 64, GRW,GRH,FBW,FBH) == std::make_pair(191u,63u));
    REQUIRE(grid_to_framebuffer(127, 64, GRW,GRH,FBW,FBH) == std::make_pair(128u,63u));
    REQUIRE(grid_to_framebuffer( 64,127, GRW,GRH,FBW,FBH) == std::make_pair(191u, 0u));
    REQUIRE(grid_to_framebuffer(127,127, GRW,GRH,FBW,FBH) == std::make_pair(128u, 0u));
}
