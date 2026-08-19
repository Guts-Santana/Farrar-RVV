#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <array>
#include <string>
#include <limits>
#include <cstdint>

constexpr int MATCH = 1;
constexpr int MISMATCH = -3;
constexpr int GAP = -2;
constexpr int GAP_OPEN = -5;
constexpr int GAP_EXT = -2;

constexpr int16_t MINUS_INF = std::numeric_limits<int16_t>::min();

constexpr size_t STRIPE_WIDTH = 16;

struct Alignment {
    std::string aligned_s0;
    std::string aligned_s1;
};

#endif // CONSTANTS_HPP
