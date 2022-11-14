#include <vector>

class BitSet {
public:
    BitSet(uint32_t width, uint32_t height) :
        _width(width),
        _height(height),
        _bits(_width*_height, false) {
    }

    BitSet& operator=(const BitSet& other) {
        _width = other._width;
        _height = other._height;
        _bits = other._bits;
        return *this;
    }

    uint32_t width() const {
        return _width;
    }

    uint32_t height() const {
        return _height;
    }

    bool test(int i, int j) const {
        assert(i >= 0 && i < _width);
        assert(j >= 0 && j < _height);

        return _bits[i*_height+j];
    }

    void set(int i, int j, bool v) {
        assert(i >= 0 && i < _width);
        assert(j >= 0 && j < _height);

        _bits[i*_height+j] = v;
    }

    void clear() {
        _bits = std::vector<bool>(_width*_height, false);
    }

private:
    uint32_t _width;
    uint32_t _height;
    std::vector<bool> _bits;
};

