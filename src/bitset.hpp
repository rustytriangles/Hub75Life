#include <vector>

#include <stdexcept>

#pragma once

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
        if (i < 0 || i >= _width) {
            throw std::out_of_range("X");
        }
        if (j < 0 || j >= _height) {
            throw std::out_of_range("Y");
        }
        return _bits[i*_height+j];
    }

    void set(int i, int j, bool v) {
        if (i < 0 || i >= _width) {
            throw std::out_of_range("X");
        }
        if (j < 0 || j >= _height) {
            throw std::out_of_range("Y");
        }
        _bits[i*_height+j] = v;
    }

    void clear() {
        _bits = std::vector<bool>(_width*_height, false);
    }

    bool any() const {
        for (int i=0; i<_width; i++) {
            for (int j=0; j<_height; j++) {
                if (test(i,j)) {
                    return true;
                }
            }
        }
        return false;
    }

private:
    uint32_t _width;
    uint32_t _height;
    std::vector<bool> _bits;
};
