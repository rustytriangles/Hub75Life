#include "util.hpp"

#include <cstdlib>
#include <ctime>

void rand_init() {
    std::srand(std::time(nullptr));
    for (int i=0; i<15; i++) {
        (void)std::rand();
    }
}

float random_between(float minval, float maxval) {

    float v = (float)std::rand() / (float)RAND_MAX;
    return minval + v*(maxval - minval);
}

std::pair<unsigned int,unsigned int> grid_to_framebuffer(uint32_t x,uint32_t y,
                                     uint32_t grid_width, uint32_t grid_height,
                                     unsigned int fb_width, unsigned int fb_height) {
    unsigned int tx = (unsigned int)x;
    unsigned int ty = (unsigned int)y;
    if (ty < fb_height) {
        return std::make_pair(tx,ty);
    } else {
        tx = fb_width - 1 - tx;
        ty = (unsigned int)grid_height - 1 - ty;
        return std::make_pair(tx,ty);
    }
}

