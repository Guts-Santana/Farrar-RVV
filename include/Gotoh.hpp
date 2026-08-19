#ifndef GOTOH_HPP
#define GOTOH_HPP

#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>
#include "constants.hpp"
#include "utility"
#include <vector>

class Gotoh
{
    int gap_open = GAP_OPEN;
    int gap_ext = GAP_EXT;
    int match = MATCH;
    int mismatch = MISMATCH;

    std::string s0;
    std::string s1;
    int maxScore;
    std::pair<int,int> maxPoint;
    Alignment alignment;

    public:
    Gotoh(std::string s0, std::string s1) : s0(s0), s1(s1), maxScore(0), maxPoint(0,0){}

    ~Gotoh(){}

    void setSequences(std::string s0, std::string s1);

    int obtainScore();

    void call();
};

#endif
