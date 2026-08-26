#ifndef RVV_REG_HPP
#define RVV_REG_HPP

#include "constants.hpp"
#include <riscv_vector.h>
#include <iostream>

#define ALWAYS_INLINE __attribute__((always_inline)) inline

class RvvReg{
    private:
        static size_t VL;

    public:

        static void setVL(size_t vl){
            VL = vl;
        }

        static ALWAYS_INLINE size_t getVL(){
            return VL;
        }

        // =========================================================================
    // LMUL = 1 (m1)
    // =========================================================================

    static ALWAYS_INLINE vint16m1_t add(vint16m1_t a, vint16m1_t b) {
        return __riscv_vadd_vv_i16m1(a, b, VL);
    }
    static ALWAYS_INLINE vint16m1_t add(vint16m1_t a, int16_t value) {
        return __riscv_vadd_vx_i16m1(a, value, VL);
    }
    static ALWAYS_INLINE vint16m1_t sub(vint16m1_t a, vint16m1_t b) {
        return __riscv_vsub_vv_i16m1(a, b, VL);
    }
    static ALWAYS_INLINE vint16m1_t sub(vint16m1_t a, int16_t value) {
        return __riscv_vsub_vx_i16m1(a, value, VL);
    }
    static ALWAYS_INLINE vint16m1_t max(vint16m1_t a, vint16m1_t b) {
        return __riscv_vmax_vv_i16m1(a, b, VL);
    }
    static ALWAYS_INLINE vint16m1_t max(vint16m1_t a, int16_t value) {
        return __riscv_vmax_vx_i16m1(a, value, VL);
    }
    static ALWAYS_INLINE int16_t maxValue(vint16m1_t a) {
        vint16m1_t init = __riscv_vmv_s_x_i16m1(INT16_MIN, 1);
        vint16m1_t red  = __riscv_vredmax_vs_i16m1_i16m1(a, init, VL);
        return __riscv_vmv_x_s_i16m1_i16(red);
    }
    static ALWAYS_INLINE bool anyBiggerElement(vint16m1_t a, vint16m1_t b) {
        vbool16_t mask = __riscv_vmsgt_vv_i16m1_b16(a, b, VL);
        return __riscv_vcpop_m_b16(mask, VL) > 0;
    }
    static ALWAYS_INLINE vint16m1_t shift(vint16m1_t a, int16_t carry) {
        return __riscv_vslide1up_vx_i16m1(a, carry, VL);
    }
    // Zero-Memory-Access extraction of last lane!
    static ALWAYS_INLINE int16_t lastElement(vint16m1_t a) {
        vint16m1_t tmp = __riscv_vslidedown_vx_i16m1(a, VL - 1, VL);
        return __riscv_vmv_x_s_i16m1_i16(tmp);
    }

    // =========================================================================
    // LMUL = 2 (m2)
    // =========================================================================

    static ALWAYS_INLINE vint16m2_t add(vint16m2_t a, vint16m2_t b) {
        return __riscv_vadd_vv_i16m2(a, b, VL);
    }
    static ALWAYS_INLINE vint16m2_t add(vint16m2_t a, int16_t value) {
        return __riscv_vadd_vx_i16m2(a, value, VL);
    }
    static ALWAYS_INLINE vint16m2_t sub(vint16m2_t a, vint16m2_t b) {
        return __riscv_vsub_vv_i16m2(a, b, VL);
    }
    static ALWAYS_INLINE vint16m2_t sub(vint16m2_t a, int16_t value) {
        return __riscv_vsub_vx_i16m2(a, value, VL);
    }
    static ALWAYS_INLINE vint16m2_t max(vint16m2_t a, vint16m2_t b) {
        return __riscv_vmax_vv_i16m2(a, b, VL);
    }
    static ALWAYS_INLINE vint16m2_t max(vint16m2_t a, int16_t value) {
        return __riscv_vmax_vx_i16m2(a, value, VL);
    }
    static ALWAYS_INLINE int16_t maxValue(vint16m2_t a) {
        vint16m1_t init = __riscv_vmv_s_x_i16m1(INT16_MIN, 1);
        vint16m1_t red  = __riscv_vredmax_vs_i16m2_i16m1(a, init, VL);
        return __riscv_vmv_x_s_i16m1_i16(red);
    }
    static ALWAYS_INLINE bool anyBiggerElement(vint16m2_t a, vint16m2_t b) {
        vbool8_t mask = __riscv_vmsgt_vv_i16m2_b8(a, b, VL);
        return __riscv_vcpop_m_b8(mask, VL) > 0;
    }
    static ALWAYS_INLINE vint16m2_t shift(vint16m2_t a, int16_t carry) {
        return __riscv_vslide1up_vx_i16m2(a, carry, VL);
    }
    static ALWAYS_INLINE int16_t lastElement(vint16m2_t a) {
        vint16m2_t tmp = __riscv_vslidedown_vx_i16m2(a, VL - 1, VL);
        return __riscv_vmv_x_s_i16m2_i16(tmp);
    }

    // =========================================================================
    // LMUL = 4 (m4)
    // =========================================================================

    static ALWAYS_INLINE vint16m4_t add(vint16m4_t a, vint16m4_t b) {
        return __riscv_vadd_vv_i16m4(a, b, VL);
    }
    static ALWAYS_INLINE vint16m4_t add(vint16m4_t a, int16_t value) {
        return __riscv_vadd_vx_i16m4(a, value, VL);
    }
    static ALWAYS_INLINE vint16m4_t max(vint16m4_t a, vint16m4_t b) {
        return __riscv_vmax_vv_i16m4(a, b, VL);
    }
    static ALWAYS_INLINE vint16m4_t max(vint16m4_t a, int16_t value) {
        return __riscv_vmax_vx_i16m4(a, value, VL);
    }
    static ALWAYS_INLINE int16_t maxValue(vint16m4_t a) {
        vint16m1_t init = __riscv_vmv_s_x_i16m1(INT16_MIN, 1);
        vint16m1_t red  = __riscv_vredmax_vs_i16m4_i16m1(a, init, VL);
        return __riscv_vmv_x_s_i16m1_i16(red);
    }
    static ALWAYS_INLINE bool anyBiggerElement(vint16m4_t a, vint16m4_t b) {
        vbool4_t mask = __riscv_vmsgt_vv_i16m4_b4(a, b, VL);
        return __riscv_vcpop_m_b4(mask, VL) > 0;
    }
    static ALWAYS_INLINE vint16m4_t shift(vint16m4_t a, int16_t carry) {
        return __riscv_vslide1up_vx_i16m4(a, carry, VL);
    }
    static ALWAYS_INLINE int16_t lastElement(vint16m4_t a) {
        vint16m4_t tmp = __riscv_vslidedown_vx_i16m4(a, VL - 1, VL);
        return __riscv_vmv_x_s_i16m4_i16(tmp);
    }

    // =========================================================================
    // LMUL = 8 (m8)
    // =========================================================================

    static ALWAYS_INLINE vint16m8_t add(vint16m8_t a, vint16m8_t b) {
        return __riscv_vadd_vv_i16m8(a, b, VL);
    }
    static ALWAYS_INLINE vint16m8_t add(vint16m8_t a, int16_t value) {
        return __riscv_vadd_vx_i16m8(a, value, VL);
    }
    static ALWAYS_INLINE vint16m8_t max(vint16m8_t a, vint16m8_t b) {
        return __riscv_vmax_vv_i16m8(a, b, VL);
    }
    static ALWAYS_INLINE vint16m8_t max(vint16m8_t a, int16_t value) {
        return __riscv_vmax_vx_i16m8(a, value, VL);
    }
    static ALWAYS_INLINE int16_t maxValue(vint16m8_t a) {
        vint16m1_t init = __riscv_vmv_s_x_i16m1(INT16_MIN, 1);
        vint16m1_t red  = __riscv_vredmax_vs_i16m8_i16m1(a, init, VL);
        return __riscv_vmv_x_s_i16m1_i16(red);
    }
    static ALWAYS_INLINE bool anyBiggerElement(vint16m8_t a, vint16m8_t b) {
        vbool2_t mask = __riscv_vmsgt_vv_i16m8_b2(a, b, VL);
        return __riscv_vcpop_m_b2(mask, VL) > 0;
    }
    static ALWAYS_INLINE vint16m8_t shift(vint16m8_t a, int16_t carry) {
        return __riscv_vslide1up_vx_i16m8(a, carry, VL);
    }
    static ALWAYS_INLINE int16_t lastElement(vint16m8_t a) {
        vint16m8_t tmp = __riscv_vslidedown_vx_i16m8(a, VL - 1, VL);
        return __riscv_vmv_x_s_i16m8_i16(tmp);
    }
};

#endif
