#include "bitset.hpp"

std::ostream& operator<<(std::ostream& str, BitSet const& b) {
    for (int i=b.height()-1; i>=0; i--) {
        for (int j=0; j<b.width(); j++) {
            str << (b.test(j,i) ? "X" : ".");
        }
        str << std::endl;
    }
    return str;
}
