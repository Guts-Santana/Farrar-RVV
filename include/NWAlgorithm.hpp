#ifndef NWALGORITHM_HPP
#define NWALGORITHM_HPP

#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>
#include "constants.hpp"

class NWAlgorithm{
    int gap = GAP;
    int match = MATCH;
    int mismatch = MISMATCH;

    std::string s0;
    std::string s1;
    int score;
    int** matrix;
    Alignment alignment;

    public:

    NWAlgorithm(std::string s0, std::string s1) : s0(s0), s1(s1), score(0), alignment({ "", "" }) {
    matrix = new int*[s0.length() + 1];
    for (size_t i = 0; i <= s0.length(); i++) {
        matrix[i] = new int[s1.length() + 1];
    }
}

    void setSequences(std::string s0, std::string s1);

    int obtainScore();

    Alignment obtainAlignment();

    void printAlignment();

    void printDPMatrix();

    void call(bool);
    
};

#endif // NWALGORITHM_HPP