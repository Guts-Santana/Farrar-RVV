#include "Farrar.hpp"

template<typename Vec>
void Farrar<Vec>::setSequences(std::string s0, std::string s1){
    this->s0 = s0;
    this->s1 = s1;
}


template<typename Vec>
void Farrar<Vec>::call(bool visual)
{
    
    std::cout << "Score: " << obtainScore() << '\n';
    if (visual){
        printHMatrix();
    }
    
}

template<typename Vec>
int Farrar<Vec>::obtainScore(){

    initMatrices();
    buildProfile();
    for (int i = 0; i < s1.length(); i++)
    {
        Vec vMax = processColumn(i);

        maxScore = std::max(maxScore,vMax.maxValue());
    }
    
    return maxScore;
}

template<typename Vec>
void Farrar<Vec>::buildProfile(){

    for (auto& vec : vProfile) {
        vec.clear();
    }

    for(char residue : alphabet)
    {
        std::vector<Vec> profile;
        profile.reserve(segLen);
        for(int i = 0; i < segLen; i++)
        {
            Vec scoreVec(stripe_width);
            for(int j = 0; j < stripe_width; j++)
            {
                int idx = j * segLen + i;
                if(idx >= s0.length())
                {
                    scoreVec[j] = 0;
                }

                scoreVec[j] = (s0[idx] == residue) ? match : mismatch;
            }


            profile.push_back(scoreVec);
            
        }
        vProfile[charToIndex(residue)] = std::move(profile);


    }

}

template<typename Vec>
void Farrar<Vec>::initMatrices(){
    maxScore = 0;

    segLen = (s0.length() + stripe_width - 1) / stripe_width;

    pvHStore.clear();
    pvHLoad.clear();
    pvE.clear();

    pvHStore.resize(segLen, Vec(stripe_width));
    pvHLoad.resize(segLen, Vec(stripe_width));
    pvE.resize(segLen, Vec(stripe_width));

    for (int i = 0; i < segLen; i++)
    {
        for (int j = 0; j < stripe_width; j++)
        {
            pvHStore[i][j] = 0;
            pvHLoad[i][j]  = 0;

            pvE[i][j] = 0;
        }
    }
}

template<typename Vec>
Vec Farrar<Vec>::processColumn(int column)
{
    Vec vF(0);
    Vec vMax(0);
    Vec vH = pvHStore[segLen - 1].shift(0);

    pvHStore.swap(pvHLoad);

    int profileIndex = charToIndex(s1[column]);

    for (int j = 0; j < segLen; j++)
    {
	if (profileIndex == -1){
		vH = vH + mismatch;
	}
	else{
       		vH = vH + vProfile[profileIndex][j];
	}
        vH = vH.max(pvE[j]);

        vH = vH.max(vF);
        vH = vH.max(0);

        vMax = vMax.max(vH);

        pvHStore[j] = vH;

        vH = vH + gap_open;
        pvE[j] = (pvE[j] + gap_ext);
        pvE[j] = pvE[j].max(vH);

        vF = (vF + gap_ext);
        vF = vF.max(vH);
        vH = pvHLoad[j];
    }
    
    vF = vF.shift(0);
    size_t j = 0;
    Vec temp = pvHStore[j] + gap_open;
    int16_t vFCarry;
    while (vF.anyBiggerElement(temp))
    {
        pvHStore[j] = pvHStore[j].max(vF);
        vMax = vMax.max(pvHStore[j]);

        temp = pvHStore[j] + gap_open;

        j++;
        vF = vF + gap_ext;

        if (j >= segLen)
        {
            vFCarry = vF[vF.size()-1];
            vF = vF.shift(vFCarry);
            j = 0;
        }
    }
    maxScore = std::max(maxScore,vMax.maxValue());

    HHistory.push_back(pvHStore);

    // previousVH = vH;
    return vMax;
}

template<typename Vec>
void Farrar<Vec>::printHMatrix(){

    std::cout << "\nH Matrix (striped)\n\n";

    std::cout << "     ";
    for (char c : s1)
        std::cout << std::setw(4) << c;
    std::cout << '\n';

    for (size_t row = 0; row < s0.size(); row++)
    {
        std::cout << std::setw(4) << s0[row] << " ";

        int seg = row % segLen;
        int lane = row / segLen;

        for (size_t col = 0; col < s1.size(); col++)
        {
            std::cout << std::setw(4)
                    << HHistory[col][seg][lane];
        }

        std::cout << '\n';
    }
}

template<typename Vec>
void Farrar<Vec>::clearData(){
    maxScore = 0;
    pvHStore.clear();
    pvHLoad.clear();
    pvE.clear();
    HHistory.clear();
    // previousVH = ScalarVec();
    for (auto &profileVec : vProfile)
    {
        profileVec.clear();
    }
}

template class Farrar<ScalarVec>;
template class Farrar<RvvVec>;
