#ifndef RVV_VEC_HPP
#define RVV_VEC_HPP

#include "constants.hpp"
#include <riscv_vector.h>
#include <iostream>

class RvvVec{

    private:
        std::array<int16_t, STRIPE_WIDTH> lanes;
        size_t VL;

    public:
        RvvVec(int16_t value = 0){
            lanes.fill(value);
            VL = __riscv_vsetvl_e16m1(STRIPE_WIDTH);
        }

        RvvVec(std::array<int16_t, STRIPE_WIDTH> lanes): lanes(lanes) {
            VL = __riscv_vsetvl_e16m1(STRIPE_WIDTH);
        }

        int16_t& operator[](size_t i);
        size_t size();

        void swap(RvvVec& other);

        vint16m1_t load();

        void store(vint16m1_t vec);

        void print();
};

#endif