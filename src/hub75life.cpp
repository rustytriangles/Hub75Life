#include <cassert>
#include <cstdint>
#include <vector>

#include "bitset.hpp"
#include "hub75life.hpp"
#include "life.hpp"
#include "util.hpp"

#include "hub75.hpp"

volatile bool flip = false;

const uint32_t GRID_WIDTH = 128;
const uint32_t GRID_HEIGHT = 64;

Hub75 hub75(FB_WIDTH, FB_HEIGHT, nullptr, PANEL_GENERIC, true);

void set_pixels(BitSet &mask, Pixel &curr_color) {
    for (uint32_t x=0; x<GRID_WIDTH; x++) {
        for (uint32_t y=0; y<GRID_HEIGHT; y++) {
            if (mask.test(x,y)) {
                std::pair<uint,uint> fb_index = grid_to_framebuffer(x,y,GRID_WIDTH,GRID_HEIGHT,FB_WIDTH,FB_HEIGHT);
                hub75.set_color(fb_index.first,fb_index.second,curr_color);
            }
        }
    }
}

void __isr dma_complete() {
    hub75.dma_complete();
}

int main() {

    stdio_init_all();

    hub75.start(dma_complete);

    // 1.3v allows overclock to ~280000-300000 but YMMV. Faster clock = faster screen update rate!
    // vreg_set_voltage(VREG_VOLTAGE_1_30);
    // sleep_ms(100);

    // 200MHz is roughly about the lower limit for driving a 64x64 display smoothly.
    // Just don't look at it out of the corner of your eye.
    set_sys_clock_khz(200000, false);

    rand_init();

    float curr_hue = random_between(0.f, 1.f);
    const float hue_step = 0.005;

    Pixel background(0.f, 0.f, 0.f);

    BitSet prevmask(GRID_WIDTH, GRID_HEIGHT);
    BitSet nextmask(GRID_WIDTH, GRID_HEIGHT);

    srand(time(NULL));
    int countdown = 0;
    constexpr int num_gen = 5000;
    absolute_time_t prev_time = {};

    while (true) {
        nextmask.clear();

        Pixel curr_color = hsv_to_rgb(curr_hue, 1.f, 1.f);

        if (countdown == 0) {
            absolute_time_t curr_time = get_absolute_time();

            int64_t delta = absolute_time_diff_us(prev_time, curr_time);
            prev_time = curr_time;
            double sec = (double)delta/1.e6;
            printf("Generated %d generations in %lf seconds (%lf)\n",
                   num_gen, sec, (double)num_gen / sec);

            for (int x=0u; x<GRID_WIDTH; x++) {
                for (int y=0u; y<GRID_HEIGHT; y++) {
                    if (random_between(0.f, 1.f) > 0.75f) {
                        prevmask.set(x,y, true);
                    }
                }
            }

            set_pixels(prevmask, curr_color);

            countdown = num_gen;
        }
        countdown -= 1;

        tick(nextmask, prevmask);

        set_pixels(nextmask, curr_color);

        curr_hue = fmod(curr_hue + hue_step, 1.);
        prevmask = nextmask;

        hub75.flip(true); // Flip and clear to the background colour

        sleep_ms(1);
    }
}
