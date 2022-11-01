#include "hub75life.hpp"

using namespace pimoroni;

volatile bool flip = false;

// This gamma table is used to correct our 8-bit (0-255) colours up to 11-bit,
// allowing us to gamma correct without losing dynamic range.
const uint16_t GAMMA_12BIT[256] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 47, 50,
    52, 54, 57, 59, 62, 65, 67, 70, 73, 76, 79, 82, 85, 88, 91, 94,
    98, 101, 105, 108, 112, 115, 119, 123, 127, 131, 135, 139, 143, 147, 151, 155,
    160, 164, 169, 173, 178, 183, 187, 192, 197, 202, 207, 212, 217, 223, 228, 233,
    239, 244, 250, 255, 261, 267, 273, 279, 285, 291, 297, 303, 309, 316, 322, 328,
    335, 342, 348, 355, 362, 369, 376, 383, 390, 397, 404, 412, 419, 427, 434, 442,
    449, 457, 465, 473, 481, 489, 497, 505, 513, 522, 530, 539, 547, 556, 565, 573,
    582, 591, 600, 609, 618, 628, 637, 646, 656, 665, 675, 685, 694, 704, 714, 724,
    734, 744, 755, 765, 775, 786, 796, 807, 817, 828, 839, 850, 861, 872, 883, 894,
    905, 917, 928, 940, 951, 963, 975, 987, 998, 1010, 1022, 1035, 1047, 1059, 1071, 1084,
    1096, 1109, 1122, 1135, 1147, 1160, 1173, 1186, 1199, 1213, 1226, 1239, 1253, 1266, 1280, 1294,
    1308, 1321, 1335, 1349, 1364, 1378, 1392, 1406, 1421, 1435, 1450, 1465, 1479, 1494, 1509, 1524,
    1539, 1554, 1570, 1585, 1600, 1616, 1631, 1647, 1663, 1678, 1694, 1710, 1726, 1743, 1759, 1775,
    1791, 1808, 1824, 1841, 1858, 1875, 1891, 1908, 1925, 1943, 1960, 1977, 1994, 2012, 2029, 2047};


// We don't *need* to make Pixel a fancy struct with RGB values, but it helps.
#pragma pack(push, 1)
struct alignas(4) Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t _;
    constexpr Pixel() : r(0), g(0), b(0), _(0) {};
    constexpr Pixel(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b), _(0) {};
    constexpr Pixel(float r, float g, float b) : r((uint8_t)(r * 255.0f)), g((uint8_t)(g * 255.0f)), b((uint8_t)(b * 255.0f)), _(0) {};
};
#pragma pack(pop)

// Create our front and back buffers.
// We'll draw into the frontbuffer and then copy everything into the backbuffer which will be used to refresh the screen.
// Double-buffering the display avoids screen tearing with fast animations or slow update rates.
Pixel backbuffer[WIDTH][HEIGHT];
Pixel frontbuffer[WIDTH][HEIGHT];

#include <vector>

class BitSet {
public:
    BitSet() :
        _bits(WIDTH*HEIGHT, false) {
    }

    BitSet& operator=(const BitSet& other) {
        _bits = other._bits;
        return *this;
    }

    bool test(int i, int j) const {
        return _bits[i*HEIGHT+j];
    }

    void set(int i, int j, bool v) {
        _bits[i*HEIGHT+j] = v;
    }

    void clear() {
        _bits = std::vector(WIDTH*HEIGHT, false);
    }

private:
    std::vector<bool> _bits;
};

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
            memcpy((uint8_t *)backbuffer, (uint8_t *)frontbuffer, WIDTH * HEIGHT * sizeof(Pixel));
            flip = false;
        }

        // Step through 0b00000001, 0b00000010, 0b00000100 etc
        for(auto bit = 1u; bit < 1 << 11; bit <<= 1) {
            // Since the display is in split into two equal halves, we step through y from 0 to HEIGHT / 2
            for(auto y = 0u; y < HEIGHT / 2; y++) {

                // 1. Shift out pixel data
                //    Shift out WIDTH pixels to the top and bottom half of the display
                for(auto x = 0u; x < WIDTH; x++) {
                    // Get the current pixel for top/bottom half
                    // This is easy since we just need the pixels at X/Y and X/Y+HEIGHT/2
                    Pixel pixel_top     = backbuffer[x][y];
                    Pixel pixel_bottom  = backbuffer[x][y + HEIGHT / 2];

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

    BitSet prevmask;
    BitSet nextmask;

    srand(time(NULL));

    for (int x=0u; x<WIDTH; x++) {
        for (int y=0u; y<WIDTH; y++) {
            if (rand() & 0x10 != 0) {
                prevmask.set(x,y, true);
            }
        }
    }

    while (true) {
        nextmask.clear();
        for(auto x = 0u; x < WIDTH; x++) {
            const int px = (x + WIDTH - 1) % WIDTH;
            const int nx = (x + 1) % WIDTH;
            for(auto y = 0u; y < HEIGHT; y++) {
                const int py = (y + HEIGHT - 1) % HEIGHT;
                const int ny = (y + 1) % HEIGHT;

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
                frontbuffer[x][y] = alive ? foreground : background;
            }
        }

        prevmask = nextmask;

        hub75_flip();
    }
}
