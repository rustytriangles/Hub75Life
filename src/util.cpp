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
