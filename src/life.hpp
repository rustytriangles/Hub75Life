#include <cstdint>
#include "bitset.hpp"

#pragma once

void tick(BitSet &dst, const BitSet &src);

void add_glider_at(BitSet &dst, int x, int y);

void add_block_at(BitSet &dst, int x, int y);
