#include "Gotoh.hpp"

void Gotoh::setSequences(std::string s0, std::string s1){
    this->s0 = s0;
    this->s1 = s1;
}

void Gotoh::printAlignment()
{
    std::cout << "Alignment Score: " << maxScore << std::endl << "Point: " << maxPoint.first << maxPoint.second << '\n';
    std::cout << "Sequence 1: " << alignment.aligned_s0 << std::endl;
    std::cout << "Sequence 2: " << alignment.aligned_s1 << std::endl;
}

void Gotoh::printDPMatrix()
{
    std::cout << "DP Matrix H:" << std::endl;
    printMatrix(matrixH);
    std::cout << '\n';
    std::cout << '\n';

    std::cout << "DP Matrix E:" << std::endl;
    printMatrix(matrixE);
    std::cout << '\n';
    std::cout << '\n';

    std::cout << "DP Matrix F" << std::endl;
    printMatrix(matrixF);
}

void Gotoh::printMatrix(int** matrix){
    std::cout << std::setw(4) << " " << " ";
    std::cout << std::setw(4) << "-" << " ";
    for (size_t i = 0; i < s1.length(); i++)
    {
        std::cout << std::setw(4) << s1[i] << " ";
    }
    std::cout << std::endl;
    
    int t = 0;
    for (size_t i = 0; i <= s0.length(); i++)
    {
        for (size_t j = 0; j <= s1.length()+1; j++)
        {
            if (j == 0 && i == 0)
            {
                std::cout << std::setw(4) << "-" << " ";
            }
            if (j == 0 && i > 0)
            {
                std::cout << std::setw(4) << s0[t++] << " ";
            }
            if (j>0)
            {
                std::cout << std::setw(4) << matrix[i][j-1] << " ";
            }

        }
        std::cout << std::endl;
    }
}

void Gotoh::call(bool visual)
{
    
    std::cout << "Score: " << obtainScore() << '\n';
    //obtainAlignment();
    if (visual){
        //printAlignment();
        printDPMatrix();
    }
    
}

int Gotoh::obtainScore(){
    for (size_t i = 0; i <= s0.length(); i++)
    {
        for (size_t j = 0; j <= s1.length(); j++)
        {

            // Setar (0,0) para 0, primeira linha para gap
            if (i == 0)
            {
                matrixH[i][j] = 0;
                matrixE[i][j] = -1000;
                matrixF[i][j] = -3000;
    
            }
            // Setar primeira coluna com gaps
            else if (j == 0)
            {
                matrixH[i][j] = 0;
                matrixE[i][j] = -3000;
                matrixF[i][j] = -1000;
            }

            // calculo de escore
            else
            {
                int matchScore = (s0[i - 1] == s1[j - 1]) ? match : mismatch;
                matrixF[i][j] = std::max({  matrixF[i - 1][j] + gap_ext, 
                                            matrixH[i - 1][j] + gap_open});
                matrixE[i][j] = std::max({  matrixE[i][j - 1] + gap_ext, 
                                            matrixH[i][j - 1] + gap_open});
                matrixH[i][j] = std::max({  matrixH[i - 1][j - 1] + matchScore, 
                                            matrixF[i][j],
                                            matrixE[i][j],
                                            0});
                
                if (matrixH[i][j] > maxScore){
                    maxScore = matrixH[i][j];
                    maxPoint.first = i;
                    maxPoint.second = j;
                }
            }
        }
    }
    return maxScore;
}