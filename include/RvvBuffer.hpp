#ifndef RVV_BUFFER_HPP
#define RVV_BUFFER_HPP

#include <riscv_vector.h>
#include "RvvTraits.hpp"
#include <iostream>

#define ALWAYS_INLINE __attribute__((always_inline)) inline

template <typename VecType>
class RvvBuffer{

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

        ALWAYS_INLINE RvvBuffer(size_t vl, ElemType value = 0){
            allocate(vl);
            if (lanes) {

                VecType vec = RvvTraits<VecType>::set(value, VL);
                RvvTraits<VecType>::store(lanes, vec, VL);
            }
        }

        ALWAYS_INLINE RvvBuffer(const RvvBuffer& other) {
            allocate(other.VL);
            if (lanes && other.lanes) {

                VecType vec = RvvTraits<VecType>::load(other.lanes, other.VL);
                RvvTraits<VecType>::store(lanes, vec, other.VL);
            }
        }

        ALWAYS_INLINE RvvBuffer(RvvBuffer&& other) noexcept : lanes(other.lanes), VL(other.VL) {
            other.lanes = nullptr;
            other.VL = 0;
        }

        ALWAYS_INLINE ~RvvBuffer(){
            if (lanes) {
                free(lanes);
            }
        }

        ALWAYS_INLINE RvvBuffer& operator=(const RvvBuffer& other) {
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

        ALWAYS_INLINE RvvBuffer& operator=(RvvBuffer&& other) noexcept {
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

        ALWAYS_INLINE void swap(RvvBuffer& other) noexcept{
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
