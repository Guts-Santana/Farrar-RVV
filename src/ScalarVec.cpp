#include "ScalarVec.hpp"
#include <iostream>


int16_t& ScalarVec::operator[](size_t i) { 
    return lanes[i]; 
}

size_t ScalarVec::size() {
        return lanes.size(); 
}


ScalarVec ScalarVec::operator+(ScalarVec& other) {
    ScalarVec result;
    for (size_t i = 0; i < lanes.size(); ++i)
        result[i] = lanes[i] + other[i];
    return result;
}

ScalarVec ScalarVec::operator-(ScalarVec& other) {
    ScalarVec result;
    for (size_t i = 0; i < lanes.size(); ++i)
        result[i] = lanes[i] - other[i];
    return result;
}

ScalarVec ScalarVec::operator*(ScalarVec& other) {
    ScalarVec result;
    for (size_t i = 0; i < lanes.size(); ++i)
        result[i] = lanes[i] * other[i];
    return result;
}


ScalarVec ScalarVec::operator<<(int n) {
    ScalarVec result;
    if (n < (int)lanes.size()) {
        for (size_t i = 0; i < lanes.size() - n; ++i)
            result[i + n] = lanes[i];
    }
    return result;
}

ScalarVec ScalarVec::operator+(int16_t value) {
    ScalarVec result;
    for (size_t i = 0; i < lanes.size(); ++i)
        result[i] = lanes[i] + value;
    return result;
}

ScalarVec ScalarVec::operator-(int16_t value) {
    ScalarVec result;
    for (size_t i = 0; i < lanes.size(); ++i)
        result[i] = lanes[i] - value;
    return result;
}

ScalarVec ScalarVec::operator*(int16_t value) {
    ScalarVec result;
    for (size_t i = 0; i < lanes.size(); ++i)
        result[i] = lanes[i] * value;
    return result;
}

ScalarVec ScalarVec::shift(int16_t carry)
{
    ScalarVec result;

    result[0] = carry;

    for (size_t i = 1; i < lanes.size(); i++)
        result[i] = lanes[i - 1];

    return result;
}


ScalarVec ScalarVec::max(ScalarVec& other) {
    ScalarVec result;
    for (size_t i = 0; i < lanes.size(); ++i)
        result[i] = std::max(lanes[i], other[i]);
    return result;
}

ScalarVec ScalarVec::max(int16_t value) {
    ScalarVec result;

    for (size_t i = 0; i < lanes.size(); i++)
        result[i] = std::max(lanes[i], value);

    return result;
}

int16_t ScalarVec::maxValue() {
    int16_t value = 0;

    for (size_t i = 0; i < lanes.size(); i++)
        value = std::max(value, lanes[i]);

    return value;
}

void ScalarVec::swap(ScalarVec& other){
    lanes.swap(other.lanes);
}

void ScalarVec::print() {
    for (auto v : lanes) std::cout << v << " ";
    std::cout << "\n";
}

bool ScalarVec::anyBiggerElement(ScalarVec& other){
    for (size_t i = 0; i < lanes.size(); i++){
        if (lanes[i] > other.lanes[i])
        {
            return true;
        }
        
    }
    return false;
}