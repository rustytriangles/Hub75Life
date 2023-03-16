#include <cassert>
#include <cstdint>
#include <vector>

#include "bitset.hpp"
#include "hub75life.hpp"
#include "hub75util.hpp"
#include "life.hpp"
#include "util.hpp"

volatile bool flip = false;

// Create our front and back buffers.
// We'll draw into the frontbuffer and then copy everything into the backbuffer which will be used to refresh the screen.
// Double-buffering the display avoids screen tearing with fast animations or slow update rates.
Pixel backbuffer[FB_WIDTH][FB_HEIGHT];
Pixel frontbuffer[FB_WIDTH][FB_HEIGHT];

void clear_buffer(Pixel (&buffer)[FB_WIDTH][FB_HEIGHT], const Pixel& color) {
    for(auto x = 0u; x < FB_HEIGHT; x++) {
        for(auto y = 0u; y < FB_HEIGHT; y++) {
            buffer[x][y] = color;
        }
    }
}

const uint32_t GRID_WIDTH = 128;
const uint32_t GRID_HEIGHT = 64;

void set_pixels(Pixel (&buffer)[FB_WIDTH][FB_HEIGHT], BitSet &mask, Pixel &curr_color) {
    for(auto x = 0u; x < (uint8_t)std::min((uint32_t)FB_WIDTH,GRID_WIDTH); x++) {
        for(auto y = 0u; y < FB_HEIGHT; y++) {
            if (mask.test(x,y)) {
                buffer[x][y] = curr_color;
            }
        }
    }
}

void hub75_flip () {
    flip = true; // TODO: rewrite to semaphore
}

void hub75_display_update() {

    // Ridiculous register write nonsense for the FM6126A-based 64x64 matrix
    FM6126A_write_register(0b1111111111111110, 12);
    FM6126A_write_register(0b0000001000000000, 13);

    while (true) {
        // 0. Copy the contents of the front buffer into our backbuffer for output to the display.
        //    This uses another whole backbuffer worth of memory, but prevents visual tearing at low frequencies.
        if (flip) {
            memcpy((uint8_t *)backbuffer, (uint8_t *)frontbuffer, FB_WIDTH * FB_HEIGHT * sizeof(Pixel));
            flip = false;
        }

        push_buffer(&backbuffer[0][0], FB_WIDTH, FB_HEIGHT);
    }
}

int main() {
    // 1.3v allows overclock to ~280000-300000 but YMMV. Faster clock = faster screen update rate!
    // vreg_set_voltage(VREG_VOLTAGE_1_30);
    // sleep_ms(100);

    // 200MHz is roughly about the lower limit for driving a 64x64 display smoothly.
    // Just don't look at it out of the corner of your eye.
    set_sys_clock_khz(200000, false);

    init_pins();

    // Launch the display update routine on Core 1, it's hungry for cycles!
    multicore_launch_core1(hub75_display_update);

    rand_init();

    float curr_hue = random_between(0.f, 1.f);
    const float hue_step = 0.005;

    Pixel background(0.f, 0.f, 0.f);

    BitSet prevmask(GRID_WIDTH, GRID_HEIGHT);
    BitSet nextmask(GRID_WIDTH, GRID_HEIGHT);

    srand(time(NULL));
    int countdown = 0;

    while (true) {
        nextmask.clear();

        Pixel curr_color = hsv_to_rgb(curr_hue, 1.f, 1.f);

        if (countdown == 0) {
            for (int x=0u; x<GRID_WIDTH; x++) {
                for (int y=0u; y<GRID_HEIGHT; y++) {
                    if (random_between(0.f, 1.f) > 0.75f) {
                        prevmask.set(x,y, true);
                    }
                }
            }

            clear_buffer(frontbuffer, background);
            set_pixels(frontbuffer, prevmask, curr_color);

            countdown = 5000;
        }
        countdown -= 1;

        tick(nextmask, prevmask);

        set_pixels(frontbuffer, nextmask, curr_color);

        curr_hue = fmod(curr_hue + hue_step, 1.);
        prevmask = nextmask;

        hub75_flip();

        sleep_ms(1);
    }
}
