#ifndef FARRAR_HPP
#define FARRAR_HPP

#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>
#include "constants.hpp"
#include <vector>

#include "Vec.hpp"

class Farrar
{
    int gap_open = GAP_OPEN;
    int gap_ext = GAP_EXT;
    int match = MATCH;
    int mismatch = MISMATCH;

    int16_t maxScore; 
    std::string s0;
    std::string s1;

    int stripe_width = STRIPE_WIDTH;
    std::vector<Vec> pvHStore;
    std::vector<Vec> pvHLoad;
    std::vector<Vec> pvE;

    std::vector<std::vector<Vec>> HHistory;

    Vec previousVH;

    std::array<std::vector<Vec>,5> vProfile;

    const std::vector<char> alphabet = {
        'A','C','G','T', 'N'
    };

    int segLen;

    public:
    Farrar(std::string s0, std::string s1) :  maxScore(0), s0(s0), s1(s1){

        this->segLen = (s0.length() + stripe_width - 1)/stripe_width;        
    }

    ~Farrar(){
        clearData();
    }

    void setSequences(std::string s0, std::string s1);

    void buildProfile();

    void initMatrices();

    Vec processColumn(int column);

    int obtainScore();

    void call(bool visual);

    void printHMatrix();

    inline int charToIndex(char c)
    {
        switch(c)
        {
            case 'A':
            case 'a':
                return 0;

            case 'C':
            case 'c':
                return 1;

            case 'G':
            case 'g':
                return 2;

            case 'T':
            case 't':
                return 3;

            case 'N':
            case 'n':
                return 4;

            default:
                throw std::runtime_error("Invalid nucleotide");
        }
    }

    void clearData();
};

#endif