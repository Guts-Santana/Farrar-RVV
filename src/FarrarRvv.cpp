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

    RvvReg::setVL(stripe_width);
    initMatrices();
    buildProfile();
    for (int i = 0; i < s1.length(); i++)
    {
        processColumn(i);
    }
    
    return maxScore;
}

template <typename VecType>
void FarrarRvv<VecType>::buildProfile(){
    size_t total_blocks = 5 * segLen;

    if (vProfile){
        free(vProfile);
    }

    posix_memalign((void**)&vProfile, 64, total_blocks * sizeof(RvvVec<VecType>));

    for (size_t k = 0; k < total_blocks; ++k) {
        new (&vProfile[k]) RvvVec<VecType>(stripe_width, 0);
    }

    for (char residue : alphabet) {
        int resIdx = charToIndex(residue);
        if (resIdx == -1) continue;

        size_t baseOffset = static_cast<size_t>(resIdx) * segLen;

        for (size_t i = 0; i < segLen; i++) {
            RvvVec<VecType> scoreVec(stripe_width, 0);
            for (size_t j = 0; j < stripe_width; j++) {
                size_t idx = j * segLen + i;
                scoreVec[j] = (idx < s0.length() && s0[idx] == residue) ? match : mismatch;
            }
            vProfile[baseOffset + i] = scoreVec;
        }
    }
}

template <typename VecType>
void FarrarRvv<VecType>::initMatrices(){
    segLen = (s0.length() + stripe_width - 1) / stripe_width;

    clearData();

    posix_memalign((void**)&pvHStore, 64, segLen * sizeof(RvvVec<VecType>));
    posix_memalign((void**)&pvHLoad,  64, segLen * sizeof(RvvVec<VecType>));
    posix_memalign((void**)&pvE,      64, segLen * sizeof(RvvVec<VecType>));

    for (size_t j = 0; j < segLen; ++j) {
        new (&pvHStore[j]) RvvVec<VecType>(stripe_width, 0);
        new (&pvHLoad[j])  RvvVec<VecType>(stripe_width, 0);
        new (&pvE[j])      RvvVec<VecType>(stripe_width, 0);
    }
}


template <typename VecType>
int FarrarRvv<VecType>::processColumn(int column)
{
    VecType dummy;
    VecType vF = RvvReg::set(0, dummy);
    VecType vE;
    VecType vMax = RvvReg::set(0, dummy);
    VecType vH = pvHStore[segLen - 1].load();
    VecType vHStore; 
    vH = RvvReg::shift(vH,0);

    VecType vProfileTemp;


    std::swap(pvHStore, pvHLoad);

    int profileIndex = charToIndex(s1[column]);
    int baseIndex = profileIndex * segLen;

    for (int j = 0; j < segLen; j++)
    {
	if (profileIndex == -1){
		vH = RvvReg::add(vH,mismatch);
	}
	else{
        
        vProfileTemp = vProfile[baseIndex + j].load();
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
    int vFCarry;
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
    maxScore = std::max(maxScore,static_cast<int>(RvvReg::maxValue(vMax)));

    // previousVH = vH;
    return maxScore;
}

template <typename VecType>
void FarrarRvv<VecType>::clearData(){
    if (pvHStore) { free(pvHStore); pvHStore = nullptr; }
    if (pvHLoad)  { free(pvHLoad);  pvHLoad  = nullptr; }
    if (pvE)      { free(pvE);      pvE      = nullptr; }
}

template class FarrarRvv<vint16m1_t>;
template class FarrarRvv<vint16m2_t>;
template class FarrarRvv<vint16m4_t>;
template class FarrarRvv<vint16m8_t>;

template class FarrarRvv<vint32m1_t>;
template class FarrarRvv<vint32m2_t>;
template class FarrarRvv<vint32m4_t>;
template class FarrarRvv<vint32m8_t>;

