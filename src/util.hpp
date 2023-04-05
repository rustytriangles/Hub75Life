#include <cstdint>
#include <iostream>
#include <utility>

#pragma once

void rand_init();

float random_between(float minval, float maxval);

std::pair<unsigned int,unsigned int> grid_to_framebuffer(uint32_t x,uint32_t y,
                        uint32_t grid_width, uint32_t grid_height,
                        unsigned int fb_width, unsigned int fb_height);

