#include "life.hpp"

void tick(BitSet &dst, const BitSet &src) {
    assert(dst.width() == src.width());
    assert(dst.height() == src.height());

    for(int x=0; x < src.width(); x++) {
        const int px = (x + src.width() - 1) % src.width();
        const int nx = (x + 1) % src.width();
        for(int y = 0; y < src.height(); y++) {
            const int py = (y + src.height() - 1) % src.height();
            const int ny = (y + 1) % src.height();

            const int n_count = src.test(px,py)
                              + src.test( x,py)
                              + src.test(nx,py)
                              + src.test(px, y)
                              + src.test(nx, y)
                              + src.test(px,ny)
                              + src.test( x,ny)
                              + src.test(nx,ny);
                bool alive = false;
                if (src.test(x,y)) {
                    alive = (n_count == 2 || n_count == 3);
                } else {
                    alive = (n_count == 3);
                }

                dst.set(x, y, alive);
            }
        }
}

