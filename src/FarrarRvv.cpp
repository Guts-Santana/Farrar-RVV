#include "FarrarRvv.hpp"

void FarrarRvv::setSequences(std::string s0, std::string s1){
    this->s0 = s0;
    this->s1 = s1;
}


void FarrarRvv::call(bool visual)
{
    
    std::cout << "Score: " << obtainScore() << '\n';
    if (visual){
        printHMatrix();
    }
    
}

int FarrarRvv::obtainScore(){

    initMatrices();
    buildProfile();
    RvvReg::setVL(16);
    for (int i = 0; i < s1.length(); i++)
    {
        processColumn(i);
    }
    
    return maxScore;
}

void FarrarRvv::buildProfile(){

    for (auto& RvvVec : vProfile) {
        RvvVec.clear();
    }

    for(char residue : alphabet)
    {
        std::vector<RvvVec> profile(segLen);
        for(int i = 0; i < segLen; i++)
        {
            RvvVec scoreRvvVec(0);
            for(int j = 0; j < stripe_width; j++)
            {
                int idx = j * segLen + i;
                if(idx >= s0.length())
                {
                    scoreRvvVec[j] = 0;
                }
		else
                scoreRvvVec[j] = (s0[idx] == residue) ? match : mismatch;
            }


            profile[i] = scoreRvvVec;
            
        }
        vProfile[charToIndex(residue)] = profile;


    }

}

void FarrarRvv::initMatrices(){
    maxScore = 0;

    segLen = (s0.length() + stripe_width - 1) / stripe_width;

    pvHStore.clear();
    pvHLoad.clear();
    pvE.clear();

    pvHStore.resize(segLen, RvvVec(0));
    pvHLoad.resize(segLen, RvvVec(0));
    pvE.resize(segLen, RvvVec(0));

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

int16_t FarrarRvv::processColumn(int column)
{
    vint16m1_t vF = RvvReg::set(0);
    vint16m1_t vE;
    vint16m1_t vMax = RvvReg::set(0);
    vint16m1_t vH = pvHStore[segLen - 1].load();
    vH = RvvReg::shift(vH,0);

    vint16m1_t vProfileTemp;


    pvHStore.swap(pvHLoad);

    int profileIndex = charToIndex(s1[column]);

    for (int j = 0; j < segLen; j++)
    {
	if (profileIndex == -1){
		vH = RvvReg::add(vH,mismatch);
	}
	else{
        vProfileTemp = vProfile[profileIndex][j].load();
       	vH = RvvReg::add(vH, vProfileTemp);
	}
        vE = pvE[j].load();

        vH = RvvReg::max(vH,vE);

        vH = RvvReg::max(vH,vF);
        vH = RvvReg::max(vH,0);

        vMax = RvvReg::max(vMax,vH);

        pvHStore[j].store(vH);

        vH = RvvReg::add(vH,gap_open);
        vE = RvvReg::add(vE,gap_ext);
        vE = RvvReg::max(vE,vH);
        pvE[j].store(vE);

        vF = RvvReg::add(vF,gap_ext);
        vF = RvvReg::max(vF,vH);
        vH = pvHLoad[j].load();
    }
    
    vF = RvvReg::shift(vF, 0);
    size_t j = 0;
    vint16m1_t vHStore = pvHStore[j].load();
    int16_t vFCarry;
    while (RvvReg::anyBiggerElement(vF, RvvReg::add(vHStore, gap_open)))
    {
        vHStore = pvHStore[j].load();
        vHStore = RvvReg::max(vHStore,vF);
        pvHStore[j].store(vHStore);
        vMax = RvvReg::max(vMax,vHStore);

        j++;
        vF = RvvReg::add(vF,gap_ext);

        if (j >= segLen)
        {
            vFCarry = RvvReg::lastElement(vF);
            vF = RvvReg::shift(vF,vFCarry);
            j = 0;
        }
    }
    maxScore = std::max(maxScore,RvvReg::maxValue(vMax));
    // HHistory.push_back(pvHStore);

    // previousVH = vH;
    return maxScore;
}

void FarrarRvv::printHMatrix(){

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

void FarrarRvv::clearData(){
    maxScore = 0;
    pvHStore.clear();
    pvHLoad.clear();
    pvE.clear();
    HHistory.clear();
    // previousVH = ScalarRvvVec();
    for (auto &profileRvvVec : vProfile)
    {
        profileRvvVec.clear();
    }
}