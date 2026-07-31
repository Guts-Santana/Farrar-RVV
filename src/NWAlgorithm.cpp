#include "NWAlgorithm.hpp"

void NWAlgorithm::setSequences(std::string s0, std::string s1){
    this->s0 = s0;
    this->s1 = s1;
}


int NWAlgorithm::obtainScore()
{
    for (size_t i = 0; i <= s0.length(); i++)
    {
        for (size_t j = 0; j <= s1.length(); j++)
        {

            // Setar (0,0) para 0, primeira linha para gap
            if (i == 0)
            {
                if (j == 0)
                {
                    matrix[i][j] = 0;
                }
                else
                {
                    matrix[i][j] = matrix[i][j - 1] + gap;
                }
            }
            // Setar primeira coluna com gaps
            else if (j == 0)
            {
                matrix[i][j] = matrix[i - 1][j] + gap;
            }

            // calculo de escore
            else
            {
                int matchScore = (s0[i - 1] == s1[j - 1]) ? match : mismatch;
                matrix[i][j] = std::max({matrix[i - 1][j - 1] + matchScore, matrix[i - 1][j] + gap, matrix[i][j - 1] + gap});
            }
        }
    }
    score = matrix[s0.length()][s1.length()];
    return score;
}

Alignment NWAlgorithm::obtainAlignment()
{
    int i = s0.length();
    int j = s1.length();
    while (i > 0 || j > 0)
    {
        if (i > 0 && j > 0) // testar diagonal, cima e esquerda
        {
            int matchScore = (s0[i - 1] == s1[j - 1]) ? match : mismatch;
            if (matrix[i][j] == matrix[i - 1][j - 1] + matchScore)
            {
                alignment.aligned_s0 = s0[i - 1] + alignment.aligned_s0;
                alignment.aligned_s1 = s1[j - 1] + alignment.aligned_s1;
                i--;
                j--;
            }
            else if (matrix[i][j] == matrix[i - 1][j] + gap)
            {
                alignment.aligned_s0 = s0[i - 1] + alignment.aligned_s0;
                alignment.aligned_s1 = '-' + alignment.aligned_s1;
                i--;
            }
            else
            {
                alignment.aligned_s0 = '-' + alignment.aligned_s0;
                alignment.aligned_s1 = s1[j - 1] + alignment.aligned_s1;
                j--;
            }
        }

        else if (i > 0) // encher o segundo alinhamento com gaps
        {
            alignment.aligned_s0 = s0[i - 1] + alignment.aligned_s0;
            alignment.aligned_s1 = '-' + alignment.aligned_s1;
            i--;
        }

        else if (j > 0) // encher o primeiro alinhamento com gaps
        {
            alignment.aligned_s0 = '-' + alignment.aligned_s0;
            alignment.aligned_s1 = s1[j - 1] + alignment.aligned_s1;
            j--;
        }
    }
    return alignment;
}

void NWAlgorithm::printAlignment()
{
    std::cout << "Alignment Score: " << score << std::endl;
    std::cout << "Sequence 1: " << alignment.aligned_s0 << std::endl;
    std::cout << "Sequence 2: " << alignment.aligned_s1 << std::endl;
}

void NWAlgorithm::printDPMatrix()
{
    std::cout << "DP Matrix:" << std::endl;

    std::cout << std::setw(4) << " " << " ";
    std::cout << std::setw(4) << "-" << " ";
    for (size_t i = 0; i < s0.length(); i++)
    {
        std::cout << std::setw(4) << s0[i] << " ";
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
                std::cout << std::setw(4) << s1[t++] << " ";
            }
            if (j>0)
            {
            std::cout << std::setw(4) << matrix[i][j-1] << " ";
            }

        }
        std::cout << std::endl;
    }
}

void NWAlgorithm::call(bool visual)
{
    
    obtainScore();
    obtainAlignment();
    if (visual){
        printAlignment();
        printDPMatrix();
    }
    
}
