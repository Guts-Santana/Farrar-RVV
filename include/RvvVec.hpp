#ifndef RVV_VEC_HPP
#define RVV_VEC_HPP

#include "constants.hpp"
#include <riscv_vector.h>
#include <iostream>

class RvvVec{

    private:
        std::array<int16_t, STRIPE_WIDTH> lanes;
        static constexpr size_t VL = STRIPE_WIDTH;

    public:
        RvvVec(int16_t value = 0){
            lanes.fill(value);
        }

        RvvVec(std::array<int16_t, STRIPE_WIDTH> lanes) : lanes(lanes) {}

        int16_t& operator[](size_t i);
        size_t size();

        RvvVec operator+(RvvVec& other);
        RvvVec operator-(RvvVec& other);
        RvvVec operator*(RvvVec& other);

        RvvVec operator+(int16_t value);
        RvvVec operator-(int16_t value);
        RvvVec operator*(int16_t value);
        RvvVec shift(int16_t carry);

        void swap(RvvVec& other);

        RvvVec max(RvvVec& other);
        RvvVec max(int16_t value);

        int16_t maxValue();

        bool anyBiggerElement(RvvVec& other);

        void print();
};

#endif