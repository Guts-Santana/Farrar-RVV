#ifndef FARRAR_RVV_HPP
#define FARRAR_RVV_HPP

#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>
#include "constants.hpp"
#include <vector>

#include "RvvBuffer.hpp"
#include "RvvOps.hpp"

template <typename VecType>
class FarrarRvv
{
    int gap_open = GAP_OPEN;
    int gap_ext = GAP_EXT;
    int match = MATCH;
    int mismatch = MISMATCH;

    int maxScore; 
    std::string s0;
    std::string s1;

    size_t stripe_width;
    RvvBuffer<VecType>* pvHStore = nullptr;
    RvvBuffer<VecType>* pvHLoad  = nullptr;
    RvvBuffer<VecType>* pvE      = nullptr;

    RvvBuffer<VecType>* vProfile = nullptr;

    const std::vector<char> alphabet = {
        'A','C','G','T', 'N'
    };

    int segLen;

    public:
    FarrarRvv(std::string s0, std::string s1, size_t stripe_width) :  maxScore(0), s0(s0), s1(s1), stripe_width(stripe_width){
        this->segLen = (s0.length() + stripe_width - 1)/stripe_width;        
    }

    ~FarrarRvv(){
        clearData();
    }

    void setSequences(std::string s0, std::string s1);

    void buildProfile();

    void initMatrices();

    int processColumn(int column);

    int obtainScore();

    void call(bool visual);

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
		std::cerr << "Invalid character: "
              << (int)(unsigned char)c
              << " (0x"
              << std::hex << (int)(unsigned char)c
              << std::dec << ")\n";
		return -1;
        }
    }

    void clearData();
};

#endif


