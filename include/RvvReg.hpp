#ifndef RVV_REG_HPP
#define RVV_REG_HPP

#include "constants.hpp"
#include <riscv_vector.h>
#include <iostream>

class RvvReg{
    private:
        static size_t VL;

    public:

        static void setVL(size_t vl){
            VL = vl;
        }

        static size_t getVL(){
            return VL;
        }

        static vint16m1_t set(int16_t value);

        static vint16m1_t add(vint16m1_t a, vint16m1_t b);
        static vint16m1_t sub(vint16m1_t a, vint16m1_t b);
        static vint16m1_t mul(vint16m1_t a, vint16m1_t b);
 
        static vint16m1_t add(vint16m1_t a, int16_t value);
        static vint16m1_t sub(vint16m1_t a, int16_t value);
        static vint16m1_t mul(vint16m1_t a, int16_t value);
 
        static vint16m1_t max(vint16m1_t a, vint16m1_t b);
        static vint16m1_t max(vint16m1_t a, int16_t value);

        static vint16m1_t shift(vint16m1_t a, int16_t carry);
        
        static int16_t maxValue(vint16m1_t a);

        static int16_t lastElement(vint16m1_t a);

        static bool anyBiggerElement(vint16m1_t a, vint16m1_t b);
};

#endif