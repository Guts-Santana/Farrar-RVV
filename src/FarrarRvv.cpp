#include "FarrarRvv.hpp"

size_t RvvReg::VL = 16;

template <typename VecType>
void FarrarRvv<VecType>::setSequences(std::string s0, std::string s1){
    this->s0 = s0;
    this->s1 = s1;
}

template <typename VecType>
void FarrarRvv<VecType>::call(bool visual)
{
    
    std::cout << "Score: " << obtainScore() << '\n';
    if (visual){
        //printHMatrix();
    }
    
}

template <typename VecType>
int FarrarRvv<VecType>::obtainScore(){

    initMatrices();
    buildProfile();
    RvvReg::setVL(stripe_width);
    for (int i = 0; i < s1.length(); i++)
    {
        processColumn(i);
    }
    
    return maxScore;
}

template <typename VecType>
void FarrarRvv<VecType>::buildProfile(){

    for (auto& RvvVec : vProfile) {
        RvvVec.clear();
    }

    for(char residue : alphabet)
    {
        std::vector<RvvVec<VecType>> profile(segLen);
        for(int i = 0; i < segLen; i++)
        {
            RvvVec<VecType> scoreRvvVec(stripe_width, 0);
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

template <typename VecType>
void FarrarRvv<VecType>::initMatrices(){
    maxScore = 0;

    segLen = (s0.length() + stripe_width - 1) / stripe_width;
    size_t size = segLen * stripe_width;

    pvHStore.assign(size,RvvVec<VecType>(stripe_width, 0));
    pvHLoad.assign(size, RvvVec<VecType>(stripe_width, 0));
    pvE.assign(size, RvvVec<VecType>(stripe_width, 0));
}

template <typename VecType>
int16_t FarrarRvv<VecType>::processColumn(int column)
{
    VecType vF = RvvVec<VecType>::set(0, stripe_width);
    VecType vE;
    VecType vMax = RvvVec<VecType>::set(0, stripe_width);
    VecType vH = pvHStore[segLen - 1].load();
    VecType vHStore; 
    vH = RvvReg::shift(vH,0);

    VecType vProfileTemp;


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
    vHStore = pvHStore[j].load();
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

    // previousVH = vH;
    return maxScore;
}

template <typename VecType>
void FarrarRvv<VecType>::clearData(){
    maxScore = 0;
    pvHStore.clear();
    pvHLoad.clear();
    pvE.clear();
    for (auto &profileRvvVec : vProfile)
    {
        profileRvvVec.clear();
    }
}

template class FarrarRvv<vint16m1_t>;
template class FarrarRvv<vint16m2_t>;
template class FarrarRvv<vint16m4_t>;
template class FarrarRvv<vint16m8_t>;