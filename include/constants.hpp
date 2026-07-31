#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <array>
#include <string>
#include <limits>


constexpr int MATCH = 2;
constexpr int MISMATCH = -3;
constexpr int GAP = -2;
constexpr int GAP_OPEN = -4;
constexpr int GAP_EXT = -1;

constexpr int16_t MINUS_INF = std::numeric_limits<int16_t>::min();

constexpr size_t STRIPE_WIDTH = 4;

struct Alignment {
    std::string aligned_s0;
    std::string aligned_s1;
};

struct Vector {
    std::array<int16_t, STRIPE_WIDTH> lane;
};

#endif // CONSTANTS_HPP
