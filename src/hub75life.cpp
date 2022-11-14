#include "hub75life.hpp"

#include <exception>

#include "bitset.hpp"

volatile bool flip = false;

#include "pixel.hpp"

// Create our front and back buffers.
// We'll draw into the frontbuffer and then copy everything into the backbuffer which will be used to refresh the screen.
// Double-buffering the display avoids screen tearing with fast animations or slow update rates.
Pixel backbuffer[FB_WIDTH][FB_HEIGHT];
Pixel frontbuffer[FB_WIDTH][FB_HEIGHT];

#include <vector>

const uint32_t GRID_WIDTH = 128;
const uint32_t GRID_HEIGHT = 128;

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

        // Step through 0b00000001, 0b00000010, 0b00000100 etc
        for(auto bit = 1u; bit < 1 << 11; bit <<= 1) {
            // Since the display is in split into two equal halves, we step through y from 0 to HEIGHT / 2
            for(auto y = 0u; y < FB_HEIGHT / 2; y++) {

                // 1. Shift out pixel data
                //    Shift out FB_WIDTH pixels to the top and bottom half of the display
                for(auto x = 0u; x < FB_WIDTH; x++) {
                    // Get the current pixel for top/bottom half
                    // This is easy since we just need the pixels at X/Y and X/Y+HEIGHT/2
                    Pixel pixel_top     = backbuffer[x][y];
                    Pixel pixel_bottom  = backbuffer[x][y + FB_HEIGHT / 2];

                    // Gamma correct the colour values from 8-bit to 11-bit
                    uint16_t pixel_top_b  = GAMMA_12BIT[pixel_top.b];
                    uint16_t pixel_top_g  = GAMMA_12BIT[pixel_top.g];
                    uint16_t pixel_top_r  = GAMMA_12BIT[pixel_top.r];

                    uint16_t pixel_bottom_b = GAMMA_12BIT[pixel_bottom.b];
                    uint16_t pixel_bottom_g = GAMMA_12BIT[pixel_bottom.g];
                    uint16_t pixel_bottom_r = GAMMA_12BIT[pixel_bottom.r];

                    // Set the clock low while we set up the data pins
                    gpio_put(PIN_CLK, !CLK_POLARITY);

                    // Top half
                    gpio_put(PIN_R0, (bool)(pixel_top_r & bit));
                    gpio_put(PIN_G0, (bool)(pixel_top_g & bit));
                    gpio_put(PIN_B0, (bool)(pixel_top_b & bit));

                    // Bottom half
                    gpio_put(PIN_R1, (bool)(pixel_bottom_r & bit));
                    gpio_put(PIN_G1, (bool)(pixel_bottom_g & bit));
                    gpio_put(PIN_B1, (bool)(pixel_bottom_b & bit));

                    // Wiggle the clock
                    // The gamma correction above will ensure our clock stays asserted
                    // for some small amount of time, avoiding the need for an explicit delay.
                    gpio_put(PIN_CLK, CLK_POLARITY);
                }


                // 2. Set address pins
                //    Set the address pins to reflect the row to light up: 0 through 15 for 32x32 pixel panels
                //    We decode our 5-bit row address out onto the 5 GPIO pins by masking each bit in turn.
                gpio_put_masked(0b11111 << PIN_ROW_A, y << PIN_ROW_A);

                // 3. Assert latch/strobe signal (STB)
                //    This latches all the values we've just clocked into the column shift registers.
                //    The values will appear on the output pins, ready for the display to be driven.
                gpio_put(PIN_STB, STB_POLARITY);
                asm volatile("nop \nnop"); // Batman!
                gpio_put(PIN_STB, !STB_POLARITY);

                // 4. Asset the output-enable signal (OE)
                //    This turns on the display for a brief period to light the selected rows/columns.
                gpio_put(PIN_OE, OE_POLARITY);

                // 5. Delay
                //    Delay for a period of time coressponding to "bit"'s significance
                for(auto s = 0u; s < bit; ++s) {
                    // The basic premise here is that "bit" will step through the values:
                    // 1, 2, 4, 8, 16, 32, 64, etc in sequence.
                    // If we plug this number into a delay loop, we'll get different magnitudes
                    // of delay which correspond exactly to the significance of each bit.
                    // The longer we delay here, the slower the overall panel refresh rate will be.
                    // But we need to delay *just enough* that we're not under-driving the panel and
                    // losing out on brightness.
                    asm volatile("nop \nnop"); // Batman!
                }

                // 6. De-assert output-enable signal (OE)
                //    Ready to go again!
                gpio_put(PIN_OE, !OE_POLARITY);

                // 7. GOTO 1.
            }
            sleep_us(1);
        }
    }
}

int main() {
    // 1.3v allows overclock to ~280000-300000 but YMMV. Faster clock = faster screen update rate!
    // vreg_set_voltage(VREG_VOLTAGE_1_30);
    // sleep_ms(100);

    // 200MHz is roughly about the lower limit for driving a 64x64 display smoothly.
    // Just don't look at it out of the corner of your eye.
    set_sys_clock_khz(200000, false);

    // Set up allllll the GPIO
    gpio_init(PIN_R0); gpio_set_function(PIN_R0, GPIO_FUNC_SIO); gpio_set_dir(PIN_R0, true);
    gpio_init(PIN_G0); gpio_set_function(PIN_G0, GPIO_FUNC_SIO); gpio_set_dir(PIN_G0, true);
    gpio_init(PIN_B0); gpio_set_function(PIN_B0, GPIO_FUNC_SIO); gpio_set_dir(PIN_B0, true);

    gpio_init(PIN_R1); gpio_set_function(PIN_R1, GPIO_FUNC_SIO); gpio_set_dir(PIN_R1, true);
    gpio_init(PIN_G1); gpio_set_function(PIN_G1, GPIO_FUNC_SIO); gpio_set_dir(PIN_G1, true);
    gpio_init(PIN_B1); gpio_set_function(PIN_B1, GPIO_FUNC_SIO); gpio_set_dir(PIN_B1, true);

    gpio_init(PIN_ROW_A); gpio_set_function(PIN_ROW_A, GPIO_FUNC_SIO); gpio_set_dir(PIN_ROW_A, true);
    gpio_init(PIN_ROW_B); gpio_set_function(PIN_ROW_B, GPIO_FUNC_SIO); gpio_set_dir(PIN_ROW_B, true);
    gpio_init(PIN_ROW_C); gpio_set_function(PIN_ROW_C, GPIO_FUNC_SIO); gpio_set_dir(PIN_ROW_C, true);
    gpio_init(PIN_ROW_D); gpio_set_function(PIN_ROW_D, GPIO_FUNC_SIO); gpio_set_dir(PIN_ROW_D, true);
    gpio_init(PIN_ROW_E); gpio_set_function(PIN_ROW_E, GPIO_FUNC_SIO); gpio_set_dir(PIN_ROW_E, true);

    gpio_init(PIN_CLK); gpio_set_function(PIN_CLK, GPIO_FUNC_SIO); gpio_set_dir(PIN_CLK, true);
    gpio_init(PIN_STB); gpio_set_function(PIN_STB, GPIO_FUNC_SIO); gpio_set_dir(PIN_STB, true);
    gpio_init(PIN_OE); gpio_set_function(PIN_OE, GPIO_FUNC_SIO); gpio_set_dir(PIN_OE, true);

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
