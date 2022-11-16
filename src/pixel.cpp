#include <cmath>

#include "pixel.hpp"

std::ostream& operator<<(std::ostream& str, Pixel const& p) {
    str << "[" << (int)p.r << ", " << (int)p.g << ", " << (int)p.b << "]";
    return str;
}

bool operator==(const Pixel& a, const Pixel& b) {
    return a.r == b.r && a.g == b.g && a.b == b.b;
}

Pixel hsv_to_rgb(float h, float s, float v) {
    if(h < 0.0f) {
        h = 1.0f + fmod(h, 1.0f);
    }

    int i = int(h * 6);
    float f = h * 6 - i;

    v = v * 255.0f;

    float sv = s * v;
    float fsv = f * sv;

    auto p = uint8_t(-sv + v);
    auto q = uint8_t(-fsv + v);
    auto t = uint8_t(fsv - sv + v);

    uint8_t bv = uint8_t(v);

    switch (i % 6) {
        default:
        case 0: return Pixel(bv, t, p);
        case 1: return Pixel(q, bv, p);
        case 2: return Pixel(p, bv, t);
        case 3: return Pixel(p, q, bv);
        case 4: return Pixel(t, p, bv);
        case 5: return Pixel(bv, p, q);
    }
}
