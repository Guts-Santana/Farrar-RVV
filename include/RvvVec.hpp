#ifndef RVV_VEC_HPP
#define RVV_VEC_HPP

#include "constants.hpp"
#include <riscv_vector.h>
#include <iostream>

#define ALWAYS_INLINE __attribute__((always_inline)) inline



// VecType The RVV intrinsic type (e.g., vint16m1_t, vint16m2_t, vint16m4_t)
template <typename VecType>
struct RvvTraits;

template <>
struct RvvTraits<vint16m1_t> {
    static ALWAYS_INLINE vint16m1_t load(const int16_t* ptr, size_t vl) {
        return __riscv_vle16_v_i16m1(ptr, vl);
    }
    static ALWAYS_INLINE void store(int16_t* ptr, vint16m1_t vec, size_t vl) {
        __riscv_vse16_v_i16m1(ptr, vec, vl);
    }
    static ALWAYS_INLINE vint16m1_t set(int16_t value, size_t vl) {
        return __riscv_vmv_v_x_i16m1(value, vl);
    }
};

template <>
struct RvvTraits<vint16m2_t> {
    static ALWAYS_INLINE vint16m2_t load(const int16_t* ptr, size_t vl) {
        return __riscv_vle16_v_i16m2(ptr, vl);
    }
    static ALWAYS_INLINE void store(int16_t* ptr, vint16m2_t vec, size_t vl) {
        __riscv_vse16_v_i16m2(ptr, vec, vl);
    }
    static ALWAYS_INLINE vint16m2_t set(int16_t value, size_t vl) {
        return __riscv_vmv_v_x_i16m2(value, vl);
    }
};

template <>
struct RvvTraits<vint16m4_t> {
    static ALWAYS_INLINE vint16m4_t load(const int16_t* ptr, size_t vl) {
        return __riscv_vle16_v_i16m4(ptr, vl);
    }
    static ALWAYS_INLINE void store(int16_t* ptr, vint16m4_t vec, size_t vl) {
        __riscv_vse16_v_i16m4(ptr, vec, vl);
    }
    static ALWAYS_INLINE vint16m4_t set(int16_t value, size_t vl) {
        return __riscv_vmv_v_x_i16m4(value, vl);
    }
};

template <>
struct RvvTraits<vint16m8_t> {
    static ALWAYS_INLINE vint16m8_t load(const int16_t* ptr, size_t vl) {
        return __riscv_vle16_v_i16m8(ptr, vl);
    }
    static ALWAYS_INLINE void store(int16_t* ptr, vint16m8_t vec, size_t vl) {
        __riscv_vse16_v_i16m8(ptr, vec, vl);
    }
    static ALWAYS_INLINE vint16m8_t set(int16_t value, size_t vl) {
        return __riscv_vmv_v_x_i16m8(value, vl);
    }
};



// VecType The RVV intrinsic type (e.g., vint16m1_t, vint16m2_t, vint16m4_t)
template <typename VecType>
class RvvVec{

    private:
        std::array<int16_t, STRIPE_WIDTH> lanes;
        size_t VL;

    public:
        ALWAYS_INLINE RvvVec(size_t vl = STRIPE_WIDTH, int16_t value = 0): VL(vl){
            lanes.fill(value);
        }

        ALWAYS_INLINE RvvVec(std::array<int16_t, STRIPE_WIDTH> lanes, size_t vl): lanes(lanes), VL(vl) {

        }

        ALWAYS_INLINE int16_t& operator[](size_t i){
            return lanes[i];
        }
        ALWAYS_INLINE size_t size(){
            return lanes.size(); 
        }

        ALWAYS_INLINE void swap(RvvVec& other){
            lanes.swap(other.lanes);
        }

        ALWAYS_INLINE VecType load(){
            return RvvTraits<VecType>::load(lanes.data(), VL);
        }

        ALWAYS_INLINE void store(VecType vec){
            RvvTraits<VecType>::store(lanes.data(), vec, VL);
        }

        static ALWAYS_INLINE VecType set(int16_t value, size_t vl){
            return RvvTraits<VecType>::set(value, vl);
        }

        ALWAYS_INLINE void print(){
            for (auto v : lanes)
                std::cout << v << " ";
            std::cout << '\n';
        }
};

#endif
