#ifndef GOTOH_HPP
#define GOTOH_HPP

#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>
#include "constants.hpp"
#include "utility"

class Gotoh
{
    int gap_open = GAP_OPEN;
    int gap_ext = GAP_EXT;
    int match = MATCH;
    int mismatch = MISMATCH;

    std::string s0;
    std::string s1;
    int maxScore;
    int** matrixH;
    int** matrixE;
    int** matrixF;
    std::pair<int,int> maxPoint;
    Alignment alignment;

    public:
    Gotoh(std::string s0, std::string s1) : s0(s0), s1(s1), maxScore(0), maxPoint(0,0), alignment({ "", "" }){
        matrixH = new int*[s0.length() + 1];
        for (size_t i = 0; i <= s0.length(); i++) {
            matrixH[i] = new int[s1.length() + 1];
        }

        matrixE = new int*[s0.length() + 1];
        for (size_t i = 0; i <= s0.length(); i++) {
            matrixE[i] = new int[s1.length() + 1];
        }

        matrixF = new int*[s0.length() + 1];
        for (size_t i = 0; i <= s0.length(); i++) {
            matrixF[i] = new int[s1.length() + 1];
        }
    }

    ~Gotoh(){}

    void setSequences(std::string s0, std::string s1);

    int obtainScore();

    void printAlignment();

    void printDPMatrix();

    void printMatrix(int** matrix);

    void call(bool visual);
};

#endif