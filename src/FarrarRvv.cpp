#include "FarrarRvv.hpp"
#include <chrono>

size_t RvvOps::VL = 16;

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

    RvvOps::setVL(stripe_width);
    initMatrices();
    buildProfile();
    VecType vMaxBlock = RvvTraits<VecType>::set(0, stripe_width);
    for (int i = 0; i < s1.length(); i++)
    {
        vMaxBlock = RvvOps::max(vMaxBlock, processColumn(i));
    }
    maxScore = static_cast<int>(RvvOps::maxValue(vMaxBlock));
    return maxScore;
}

template <typename VecType>
void FarrarRvv<VecType>::buildProfile(){
    
    size_t total_blocks = 5 * segLen;

    posix_memalign((void**)&vProfile, 64, total_blocks * sizeof(RvvBuffer<VecType>));

    for (size_t k = 0; k < total_blocks; ++k) {
        new (&vProfile[k]) RvvBuffer<VecType>(stripe_width, 0);
    }

    for (char residue : alphabet) {
        int resIdx = charToIndex(residue);
        if (resIdx == -1) continue;

        size_t baseOffset = static_cast<size_t>(resIdx) * segLen;

        for (size_t i = 0; i < segLen; i++) {
            RvvBuffer<VecType> scoreVec(stripe_width, 0);
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
    clearData();
    segLen = (s0.length() + stripe_width - 1) / stripe_width;

    posix_memalign((void**)&pvHStore, 64, segLen * sizeof(RvvBuffer<VecType>));
    posix_memalign((void**)&pvHLoad,  64, segLen * sizeof(RvvBuffer<VecType>));
    posix_memalign((void**)&pvE,      64, segLen * sizeof(RvvBuffer<VecType>));

    for (size_t j = 0; j < segLen; ++j) {
        new (&pvHStore[j]) RvvBuffer<VecType>(stripe_width, 0);
        new (&pvHLoad[j])  RvvBuffer<VecType>(stripe_width, 0);
        new (&pvE[j])      RvvBuffer<VecType>(stripe_width, 0);
    }
}


template <typename VecType>
VecType FarrarRvv<VecType>::processColumn(int column)
{
    VecType vF = RvvTraits<VecType>::set(0, stripe_width);
    VecType vE;
    VecType vMax = RvvTraits<VecType>::set(0, stripe_width);
    VecType vH = pvHStore[segLen - 1].load();
    vH = RvvOps::shift(vH,0);

    std::swap(pvHStore, pvHLoad);

    int profileIndex = charToIndex(s1[column]);
    int baseIndex = profileIndex * segLen;

    for (int j = 0; j < segLen; j++)
    {
        if (profileIndex == -1){
            vH = RvvOps::add(vH,mismatch);
	    }
        else{
            VecType vProfileTemp = vProfile[baseIndex + j].load();
            vH = RvvOps::add(vH, vProfileTemp);
        }
        vE = pvE[j].load();

        vH = RvvOps::max(vH,vE);

        vH = RvvOps::max(vH,vF);
        vH = RvvOps::max(vH,0);

        vMax = RvvOps::max(vMax,vH);

        pvHStore[j].store(vH);

        vH = RvvOps::add(vH,gap_open);
        vE = RvvOps::add(vE,gap_ext);
        vE = RvvOps::max(vE,vH);
        pvE[j].store(vE);

        vF = RvvOps::add(vF,gap_ext);
        vF = RvvOps::max(vF,vH);
        vH = pvHLoad[j].load();
    }

    //Prefix Scan F
    int accumulatedDecay;
    vF = RvvOps::shift(vF, 0);

    for (size_t offset = 1; offset < stripe_width; offset <<= 1) {
        VecType vShift = RvvOps::slideup(vF, offset);
        
        accumulatedDecay = offset * gap_ext;
        vShift = RvvOps::add(vShift, accumulatedDecay);
    
        vF = RvvOps::max(vF, vShift);
    }

    for (size_t j = 0; j < segLen; j++)
    {
        vH = pvHStore[j].load();
        vH = RvvOps::max(vH, vF);
        pvHStore[j].store(vH);
        vMax = RvvOps::max(vMax, vH);
        vF = RvvOps::add(vF, gap_ext);
    }

    // maxScore = std::max(maxScore, static_cast<int>(RvvOps::maxValue(vMax)));
    return vMax;
}

template <typename VecType>
void FarrarRvv<VecType>::clearData(){
    if (pvHStore) {
        for (size_t j = 0; j < segLen; ++j) {
            pvHStore[j].~RvvBuffer();
            pvHLoad[j].~RvvBuffer();
            pvE[j].~RvvBuffer();
            vProfile[j].~RvvBuffer<VecType>();
        }
        free(pvHStore); pvHStore = nullptr;
        free(pvHLoad);  pvHLoad  = nullptr;
        free(pvE);      pvE      = nullptr;
        free(vProfile); vProfile = nullptr;
    }
}

template class FarrarRvv<vint16m1_t>;
template class FarrarRvv<vint16m2_t>;
template class FarrarRvv<vint16m4_t>;
template class FarrarRvv<vint16m8_t>;

template class FarrarRvv<vint32m1_t>;
template class FarrarRvv<vint32m2_t>;
template class FarrarRvv<vint32m4_t>;
template class FarrarRvv<vint32m8_t>;
