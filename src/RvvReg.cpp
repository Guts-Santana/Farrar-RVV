#include "RvvReg.hpp"

size_t RvvReg::VL = 16;

vint16m1_t RvvReg::set(int16_t value){
    return __riscv_vmv_v_x_i16m1(value, VL);
}

vint16m1_t RvvReg::add(vint16m1_t a, vint16m1_t b){
    return __riscv_vadd_vv_i16m1(a, b, VL);
}

vint16m1_t RvvReg::sub(vint16m1_t a, vint16m1_t b){
    return __riscv_vsub_vv_i16m1(a, b, VL);

}

vint16m1_t RvvReg::mul(vint16m1_t a, vint16m1_t b){
    return __riscv_vmul_vv_i16m1(a, b, VL);

}

vint16m1_t RvvReg::add(vint16m1_t a, int16_t value){
    return __riscv_vadd_vx_i16m1(a, value, VL);
}

vint16m1_t RvvReg::sub(vint16m1_t a, int16_t value){
    return __riscv_vsub_vx_i16m1(a, value, VL);

}

vint16m1_t RvvReg::mul(vint16m1_t a, int16_t value){
    return __riscv_vmul_vx_i16m1(a, value, VL);
}

vint16m1_t RvvReg::max(vint16m1_t a, vint16m1_t b){
    return __riscv_vmax_vv_i16m1(a, b, VL);
}

vint16m1_t RvvReg::max(vint16m1_t a, int16_t value){
    return __riscv_vmax_vx_i16m1(a, value, VL);
}

int16_t RvvReg::maxValue(vint16m1_t a){
    auto init = __riscv_vmv_v_x_i16m1(INT16_MIN, 1);

    auto red = __riscv_vredmax_vs_i16m1_i16m1(a, init, VL);

    return __riscv_vmv_x_s_i16m1_i16(red);
}

bool RvvReg::anyBiggerElement(vint16m1_t a, vint16m1_t b){
    auto mask = __riscv_vmsgt_vv_i16m1_b16(a, b, VL);
    return __riscv_vcpop_m_b16(mask, VL) > 0;
}

vint16m1_t RvvReg::shift(vint16m1_t a, int16_t carry){
    return __riscv_vslide1up_vx_i16m1(a, carry, VL);
}

int16_t RvvReg::lastElement(vint16m1_t a){
    std::array<int16_t, STRIPE_WIDTH> tmp;
    __riscv_vse16_v_i16m1(tmp.data(), a, VL);
    return tmp[VL - 1];
}