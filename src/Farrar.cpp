#include "Farrar.hpp"

void Farrar::setSequences(std::string s0, std::string s1){
    this->s0 = s0;
    this->s1 = s1;
}



void Farrar::call(bool visual)
{
    
    std::cout << "Score: " << obtainScore() << '\n';
    //obtainAlignment();
    if (visual){
        // printAlignment();
        // printDPMatrix();
    }
    
}

int Farrar::obtainScore(){

    initMatrices();
    buildProfile();
    for (int i = 0; i < s1.length(); i++)
    {
        Vec vMax = processColumn(i);

        maxScore = std::max(maxScore,vMax.maxValue());
    }
    
    return maxScore;
}

void Farrar::buildProfile(){

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
                    continue;
                }

                scoreVec[j] = 
                (s0[idx] == residue)
                    ? match
                    : mismatch;
            }


            profile.push_back(scoreVec);
            
        }
        vProfile[charToIndex(residue)] = std::move(profile);


    }

}

void Farrar::initMatrices(){
    maxScore = 0;

    segLen = (s0.length() + stripe_width - 1) / stripe_width;

    // Allocate all striped vectors
    pvHStore.clear();
    pvHLoad.clear();
    pvE.clear();

    pvHStore.resize(segLen, Vec(stripe_width));
    pvHLoad.resize(segLen, Vec(stripe_width));
    pvE.resize(segLen, Vec(stripe_width));

    // Initialize H and E
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

Vec Farrar::processColumn(int column)
{
    Vec vF(stripe_width, 0);
    Vec vMax(stripe_width, 0);

    Vec vH = pvHStore[segLen - 1].shift(0);

    pvHStore.swap(pvHLoad);

    int profileIndex = charToIndex(s1[column]);

    for (int j = 0; j < segLen; j++)
    {
        vH = vH + vProfile[profileIndex][j];;
        vH = vH.max(pvE[j]);

        vH = vH.max(vF);
        vH = vH.max(0);

        vMax = vMax.max(vH);

        pvHStore[j] = vH;

        //Vec tmp = vH - gap_open;

        vH = vH + gap_open;
        pvE[j] = (pvE[j] + gap_ext);
        pvE[j] = pvE[j].max(vH);

        vF = (vF + gap_ext);
        vF = vF.max(vH);
        vH = pvHLoad[j];
    }
    
    //Lazy F
    vF = vF.shift(0);
    size_t j = 0;
    Vec temp = pvHStore[j] + gap_open;
    while (vF.anyBiggerElement(temp))
    {
        pvHStore[j] = pvHStore[j].max(vF);
        //vMax = vMax.max(pvHStore[j]);

        temp = pvHStore[j] + gap_open;
        j++;
        vF = vF + gap_ext;
        if (j >= segLen)
        {
            vF = vF.shift(0);
            j = 0;
        }
    }
    maxScore = std::max(maxScore,vMax.maxValue());


    return vMax;
}