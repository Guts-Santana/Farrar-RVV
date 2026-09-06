#ifndef RVV_VEC_HPP
#define RVV_VEC_HPP

#include <riscv_vector.h>
#include "RvvReg.hpp"
#include <iostream>

#define ALWAYS_INLINE __attribute__((always_inline)) inline


template <typename VecType>
struct RvvTraits;

template <>
struct RvvTraits<vint16m1_t> {
    using ElemType = int16_t;
    static ALWAYS_INLINE vint16m1_t load(const int16_t* ptr, size_t vl) {
        return __riscv_vle16_v_i16m1(ptr, vl);
    }
    static ALWAYS_INLINE void store(int16_t* ptr, vint16m1_t vec, size_t vl) {
        __riscv_vse16_v_i16m1(ptr, vec, vl);
    }
};

template <>
struct RvvTraits<vint16m2_t> {
    using ElemType = int16_t;
    static ALWAYS_INLINE vint16m2_t load(const int16_t* ptr, size_t vl) {
        return __riscv_vle16_v_i16m2(ptr, vl);
    }
    static ALWAYS_INLINE void store(int16_t* ptr, vint16m2_t vec, size_t vl) {
        __riscv_vse16_v_i16m2(ptr, vec, vl);
    }
};

template <>
struct RvvTraits<vint16m4_t> {
    using ElemType = int16_t;
    static ALWAYS_INLINE vint16m4_t load(const int16_t* ptr, size_t vl) {
        return __riscv_vle16_v_i16m4(ptr, vl);
    }
    static ALWAYS_INLINE void store(int16_t* ptr, vint16m4_t vec, size_t vl) {
        __riscv_vse16_v_i16m4(ptr, vec, vl);
    }
};

template <>
struct RvvTraits<vint16m8_t> {
    using ElemType = int16_t;
    static ALWAYS_INLINE vint16m8_t load(const int16_t* ptr, size_t vl) {
        return __riscv_vle16_v_i16m8(ptr, vl);
    }
    static ALWAYS_INLINE void store(int16_t* ptr, vint16m8_t vec, size_t vl) {
        __riscv_vse16_v_i16m8(ptr, vec, vl);
    }
};


// Using INT32 for comparisons;

template <>
struct RvvTraits<vint32m1_t> {
    using ElemType = int32_t;
    static ALWAYS_INLINE vint32m1_t load(const int32_t* ptr, size_t vl) {
        return __riscv_vle32_v_i32m1(ptr, vl);
    }
    static ALWAYS_INLINE void store(int32_t* ptr, vint32m1_t vec, size_t vl) {
        __riscv_vse32_v_i32m1(ptr, vec, vl);
    }
};

template <>
struct RvvTraits<vint32m2_t> {
    using ElemType = int32_t;
    static ALWAYS_INLINE vint32m2_t load(const int32_t* ptr, size_t vl) {
        return __riscv_vle32_v_i32m2(ptr, vl);
    }
    static ALWAYS_INLINE void store(int32_t* ptr, vint32m2_t vec, size_t vl) {
        __riscv_vse32_v_i32m2(ptr, vec, vl);
    }
};

template <>
struct RvvTraits<vint32m4_t> {
    using ElemType = int32_t;
    static ALWAYS_INLINE vint32m4_t load(const int32_t* ptr, size_t vl) {
        return __riscv_vle32_v_i32m4(ptr, vl);
    }
    static ALWAYS_INLINE void store(int32_t* ptr, vint32m4_t vec, size_t vl) {
        __riscv_vse32_v_i32m4(ptr, vec, vl);
    }
};

template <>
struct RvvTraits<vint32m8_t> {
    using ElemType = int32_t;
    static ALWAYS_INLINE vint32m8_t load(const int32_t* ptr, size_t vl) {
        return __riscv_vle32_v_i32m8(ptr, vl);
    }
    static ALWAYS_INLINE void store(int32_t* ptr, vint32m8_t vec, size_t vl) {
        __riscv_vse32_v_i32m8(ptr, vec, vl);
    }
};


template <typename VecType>
class RvvVec{

    using ElemType = typename RvvTraits<VecType>::ElemType;

    private:
        ElemType* lanes = nullptr;
        size_t VL;

        void allocate(size_t vl) {
            this->VL = vl;
            if (posix_memalign((void**)&lanes, 64, vl * sizeof(ElemType)) != 0) {
                lanes = nullptr;
            }
        }

    public:

        ALWAYS_INLINE RvvVec(size_t vl, ElemType value = 0){
            allocate(vl);
            if (lanes) {

                VecType vec = RvvReg::set(value, vec);
                RvvTraits<VecType>::store(lanes, vec, VL);
            }
        }

        ALWAYS_INLINE RvvVec(const RvvVec& other) {
            allocate(other.VL);
            if (lanes && other.lanes) {

                VecType vec = RvvTraits<VecType>::load(other.lanes, other.VL);
                RvvTraits<VecType>::store(lanes, vec, other.VL);
            }
        }

        ALWAYS_INLINE RvvVec(RvvVec&& other) noexcept : lanes(other.lanes), VL(other.VL) {
            other.lanes = nullptr;
            other.VL = 0;
        }

        ALWAYS_INLINE ~RvvVec(){
            if (lanes) {
                free(lanes);
            }
        }

        ALWAYS_INLINE RvvVec& operator=(const RvvVec& other) {
            if (this != &other) {
                if (this->VL != other.VL) {
                    if (lanes){
                        free(lanes);
                    }
                    allocate(other.VL);
                }
                if (lanes && other.lanes && VL > 0) {
                    VecType vec = RvvTraits<VecType>::load(other.lanes, other.VL);
                    RvvTraits<VecType>::store(lanes, vec, other.VL);
                }
            }
            return *this;
        }

        ALWAYS_INLINE RvvVec& operator=(RvvVec&& other) noexcept {
            if (this != &other) {
                if (lanes) free(lanes);
                lanes = other.lanes;
                VL = other.VL;
                other.lanes = nullptr;
                other.VL = 0;
            }
            return *this;
        }

        ALWAYS_INLINE ElemType& operator[](size_t i){
            return lanes[i];
        }
        ALWAYS_INLINE size_t size(){
            return VL; 
        }

        ALWAYS_INLINE void swap(RvvVec& other) noexcept{
            std::swap(lanes, other.lanes);
            std::swap(VL, other.VL);
        }

        ALWAYS_INLINE VecType load(){
            return RvvTraits<VecType>::load(lanes, VL);
        }

        ALWAYS_INLINE void store(VecType vec){
            RvvTraits<VecType>::store(lanes, vec, VL);
        }

        ALWAYS_INLINE void print(){
            for (size_t i = 0; i < VL; ++i) {
                std::cout << lanes[i] << " ";
            }
            std::cout << '\n';
        }
};

#endif

