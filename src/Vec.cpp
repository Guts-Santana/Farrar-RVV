#include "Vec.hpp"

#include <iostream>


int16_t& Vec::operator[](size_t i) { 
    return lanes[i]; 
}

size_t Vec::size() {
        return lanes.size(); 
}


Vec Vec::operator+(Vec& other) {
    Vec result(lanes.size());
    for (size_t i = 0; i < lanes.size(); ++i)
        result[i] = lanes[i] + other[i];
    return result;
}

Vec Vec::operator-(Vec& other) {
    Vec result(lanes.size());
    for (size_t i = 0; i < lanes.size(); ++i)
        result[i] = lanes[i] - other[i];
    return result;
}

Vec Vec::operator*(Vec& other) {
    Vec result(lanes.size());
    for (size_t i = 0; i < lanes.size(); ++i)
        result[i] = lanes[i] * other[i];
    return result;
}


Vec Vec::operator<<(int n) {
    Vec result(lanes.size(), 0);
    if (n < (int)lanes.size()) {
        for (size_t i = 0; i < lanes.size() - n; ++i)
            result[i + n] = lanes[i];
    }
    return result;
}

Vec Vec::operator+(int16_t value) {
    Vec result(lanes.size());
    for (size_t i = 0; i < lanes.size(); ++i)
        result[i] = lanes[i] + value;
    return result;
}

Vec Vec::operator-(int16_t value) {
    Vec result(lanes.size());
    for (size_t i = 0; i < lanes.size(); ++i)
        result[i] = lanes[i] - value;
    return result;
}

Vec Vec::operator*(int16_t value) {
    Vec result(lanes.size());
    for (size_t i = 0; i < lanes.size(); ++i)
        result[i] = lanes[i] * value;
    return result;
}

Vec Vec::shift(int16_t carry)
{
    Vec result(lanes.size());

    result[0] = carry;

    for (size_t i = 1; i < lanes.size(); i++)
        result[i] = lanes[i - 1];

    return result;
}


Vec Vec::max(Vec& other) {
    Vec result(lanes.size());
    for (size_t i = 0; i < lanes.size(); ++i)
        result[i] = std::max(lanes[i], other[i]);
    return result;
}

Vec Vec::max(int16_t value) {
    Vec result(lanes.size());

    for (size_t i = 0; i < lanes.size(); i++)
        result[i] = std::max(lanes[i], value);

    return result;
}

int16_t Vec::maxValue() {
    int16_t value = 0;

    for (size_t i = 0; i < lanes.size(); i++)
        value = std::max(value, lanes[i]);

    return value;
}

void Vec::swap(Vec& other){
    lanes.swap(other.lanes);
}

void Vec::print() {
    for (auto v : lanes) std::cout << v << " ";
    std::cout << "\n";
}

bool Vec::anyBiggerElement(Vec& other){
    for (size_t i = 0; i < lanes.size(); i++){
        if (lanes[i] > other.lanes[i])
        {
            return true;
        }
        
    }
    return false;
}