#include "RvvVec.hpp"

int16_t& RvvVec::operator[](size_t i) { 
    return lanes[i]; 
}

size_t RvvVec::size() {
        return lanes.size(); 
}

vint16m1_t RvvVec::load() {

    return __riscv_vle16_v_i16m1(lanes.data(), VL);;
}

void RvvVec::store(vint16m1_t vec) {
    __riscv_vse16_v_i16m1(lanes.data(), vec, VL);
}

void RvvVec::swap(RvvVec& other)
{
    lanes.swap(other.lanes);
}
void RvvVec::print()
{
    for (auto v : lanes)
        std::cout << v << " ";

    std::cout << '\n';
}


