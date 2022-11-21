#include <cassert>
#include <cstdint>
#include <vector>

#include "bitset.hpp"
#include "hub75life.hpp"
#include "hub75util.hpp"

volatile bool flip = false;


// Create our front and back buffers.
// We'll draw into the frontbuffer and then copy everything into the backbuffer which will be used to refresh the screen.
// Double-buffering the display avoids screen tearing with fast animations or slow update rates.
Pixel backbuffer[FB_WIDTH][FB_HEIGHT];
Pixel frontbuffer[FB_WIDTH][FB_HEIGHT];

const uint32_t GRID_WIDTH = 64;
const uint32_t GRID_HEIGHT = 64;

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

    Pixel foreground(1.f, 1.f, 1.f);
    Pixel background(0.f, 0.f, 0.f);

    BitSet prevmask(GRID_WIDTH, GRID_HEIGHT);
    BitSet nextmask(GRID_WIDTH, GRID_HEIGHT);

    srand(time(NULL));
    int countdown = 0;

    while (true) {
        nextmask.clear();

        if (countdown == 0) {
            for (int x=0u; x<GRID_WIDTH; x++) {
                for (int y=0u; y<GRID_HEIGHT; y++) {
                    if (rand() & 0x10 != 0) {
                        prevmask.set(x,y, true);
                    }
                }
            }
            foreground = hsv_to_rgb((float)rand()/(float)RAND_MAX, 1.f, 1.f);
            countdown = 5000;
        }
        countdown -= 1;

        for(auto x = 0u; x < GRID_WIDTH; x++) {
            const int px = (x + GRID_WIDTH - 1) % GRID_WIDTH;
            const int nx = (x + 1) % GRID_WIDTH;
            for(auto y = 0u; y < GRID_HEIGHT; y++) {
                const int py = (y + GRID_HEIGHT - 1) % GRID_HEIGHT;
                const int ny = (y + 1) % GRID_HEIGHT;

                const int n_count = prevmask.test(px,py)
                                  + prevmask.test( x,py)
                                  + prevmask.test(nx,py)
                                  + prevmask.test(px, y)
                                  + prevmask.test(nx, y)
                                  + prevmask.test(px,ny)
                                  + prevmask.test( x,ny)
                                  + prevmask.test(nx,ny);
                bool alive = false;
                if (prevmask.test(x,y)) {
                    alive = (n_count == 2 || n_count == 3);
                } else {
                    alive = (n_count == 3);
                }

                nextmask.set(x, y, alive);
            }
        }

        for(auto x = 0u; x < (uint8_t)std::min((uint32_t)FB_WIDTH,GRID_WIDTH); x++) {
            for(auto y = 0u; y < FB_HEIGHT; y++) {
                frontbuffer[x][y] = nextmask.test(x,y) ? foreground : background;
            }
        }

        //
        // This is not correct
        if (GRID_HEIGHT > FB_HEIGHT) {
            int extra_columns = std::min(GRID_HEIGHT - FB_HEIGHT, FB_WIDTH - GRID_WIDTH);
            for (int i=0; i<extra_columns; i++) {
                int xdst = FB_HEIGHT + i;
                int xsrc = GRID_WIDTH - i;
                for (auto ydst = 0u; ydst < FB_HEIGHT; ydst++) {
                    int ysrc = GRID_HEIGHT - ydst;
                    frontbuffer[xdst][ydst] = nextmask.test(xsrc,ysrc) ? foreground : background;
                }
            }
        }

        prevmask = nextmask;

        hub75_flip();

        sleep_ms(1);
    }
}
