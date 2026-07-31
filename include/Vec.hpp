#ifndef VEC_HPP
#define VEC_HPP

#include <vector>
#include <algorithm>
#include "constants.hpp"
#include <string>

class Vec {
private:
    std::vector<int16_t> lanes;

public:
    Vec(size_t size = STRIPE_WIDTH, int initVal = 0) : lanes(size, initVal){}
    Vec(std::vector<int16_t>& data) :lanes(data){}

    int16_t& operator[](size_t i);
    size_t size();

    Vec operator+(Vec& other);
    Vec operator-(Vec& other);
    Vec operator*(Vec& other);
    Vec operator<<(int n);

    Vec operator+(int16_t value);
    Vec operator-(int16_t value);
    Vec operator*(int16_t value);
    Vec shift(int16_t carry);

    void swap(Vec& other);

    Vec max(Vec& other);
    Vec max(int16_t value);

    int16_t maxValue();

    bool anyBiggerElement(Vec& other);

    void print();
};





#endif