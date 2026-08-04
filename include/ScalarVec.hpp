#ifndef SCALAR_VEC_HPP
#define SCALAR_VEC_HPP

#include <vector>
#include <algorithm>
#include "constants.hpp"
#include <string>
#include "vectorial_call.hpp"

class ScalarVec {
private:
    std::array<int16_t, STRIPE_WIDTH> lanes;

public:
    ScalarVec(int16_t value = 0){
        lanes.fill(value);
    }

    ScalarVec(std::array<int16_t, STRIPE_WIDTH>& data) :lanes(data){}

    int16_t& operator[](size_t i);
    size_t size();

    ScalarVec operator+(ScalarVec& other);
    ScalarVec operator-(ScalarVec& other);
    ScalarVec operator*(ScalarVec& other);
    ScalarVec operator<<(int n);

    ScalarVec operator+(int16_t value);
    ScalarVec operator-(int16_t value);
    ScalarVec operator*(int16_t value);
    ScalarVec shift(int16_t carry);

    void swap(ScalarVec& other);

    ScalarVec max(ScalarVec& other);
    ScalarVec max(int16_t value);

    int16_t maxValue();

    bool anyBiggerElement(ScalarVec& other);

    void print();
};





#endif