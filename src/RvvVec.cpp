#include "RvvVec.hpp"

int16_t& RvvVec::operator[](size_t i) { 
    return lanes[i]; 
}

size_t RvvVec::size() {
        return lanes.size(); 
}

RvvVec RvvVec::operator+(RvvVec& other) {
    RvvVec result;

    auto a = __riscv_vle16_v_i16m1(lanes.data(), VL);
    auto b = __riscv_vle16_v_i16m1(other.lanes.data(), VL);

    auto c = __riscv_vadd_vv_i16m1(a, b, VL);

    __riscv_vse16_v_i16m1(result.lanes.data(), c, VL);

    return result;
}

RvvVec RvvVec::operator-(RvvVec& other) {
    RvvVec result;

    auto a = __riscv_vle16_v_i16m1(lanes.data(), VL);
    auto b = __riscv_vle16_v_i16m1(other.lanes.data(), VL);

    auto c = __riscv_vsub_vv_i16m1(a, b, VL);

    __riscv_vse16_v_i16m1(result.lanes.data(), c, VL);

    return result;
}

RvvVec RvvVec::operator*(RvvVec& other) {
    RvvVec result;
    auto a = __riscv_vle16_v_i16m1(lanes.data(), VL);
    auto b = __riscv_vle16_v_i16m1(other.lanes.data(), VL);

    auto c = __riscv_vmul_vv_i16m1(a, b, VL);

    __riscv_vse16_v_i16m1(result.lanes.data(), c, VL);
    return result;
}


RvvVec RvvVec::operator+(int16_t value) {
    RvvVec result;

    auto a = __riscv_vle16_v_i16m1(lanes.data(), VL);
    auto c = __riscv_vadd_vx_i16m1(a, value, VL);

    __riscv_vse16_v_i16m1(result.lanes.data(), c, VL);

    return result;
}

RvvVec RvvVec::operator-(int16_t value) {
    RvvVec result;

    auto a = __riscv_vle16_v_i16m1(lanes.data(), VL);
    auto c = __riscv_vsub_vx_i16m1(a, value, VL);

    __riscv_vse16_v_i16m1(result.lanes.data(), c, VL);

    return result;

}

RvvVec RvvVec::operator*(int16_t value) {
    RvvVec result;

    auto a = __riscv_vle16_v_i16m1(lanes.data(), VL);
    auto c = __riscv_vmul_vx_i16m1(a, value, VL);

    __riscv_vse16_v_i16m1(result.lanes.data(), c, VL);

    return result;
}

RvvVec RvvVec::shift(int16_t carry)
{
    RvvVec result;
    result.lanes[0] = carry;
    for (size_t i = 1; i < VL; ++i)
        result.lanes[i] = lanes[i - 1];
    return result;
}


RvvVec RvvVec::max(RvvVec& other) {
    RvvVec result;

    auto a = __riscv_vle16_v_i16m1(lanes.data(), VL);
    auto b = __riscv_vle16_v_i16m1(other.lanes.data(), VL);

    auto c = __riscv_vmax_vv_i16m1(a, b, VL);

    __riscv_vse16_v_i16m1(result.lanes.data(), c, VL);

    return result;
}

RvvVec RvvVec::max(int16_t value) {
    RvvVec result;

    auto a = __riscv_vle16_v_i16m1(lanes.data(), VL);
    auto c = __riscv_vmax_vx_i16m1(a, value, VL);

    __riscv_vse16_v_i16m1(result.lanes.data(), c, VL);

    return result;
}

int16_t RvvVec::maxValue() {
    auto a = __riscv_vle16_v_i16m1(lanes.data(), VL);

    auto init = __riscv_vmv_v_x_i16m1(INT16_MIN, 1);

    auto red = __riscv_vredmax_vs_i16m1_i16m1(a, init, VL);

    return __riscv_vmv_x_s_i16m1_i16(red);
}

void RvvVec::swap(RvvVec& other)
{
    lanes.swap(other.lanes);
}
void RvvVec::print()
{
{
    for (auto v : lanes)
        std::cout << v << " ";

    std::cout << '\n';
}
}

bool RvvVec::anyBiggerElement(RvvVec& other){

    auto a = __riscv_vle16_v_i16m1(lanes.data(), VL);
    auto b = __riscv_vle16_v_i16m1(other.lanes.data(), VL);

    auto mask = __riscv_vmsgt_vv_i16m1_b16(a, b, VL);

    return __riscv_vcpop_m_b16(mask, VL) > 0;
}