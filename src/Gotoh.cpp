#include "Gotoh.hpp"

void Gotoh::setSequences(std::string s0, std::string s1){
    this->s0 = s0;
    this->s1 = s1;
}

void Gotoh::call()
{
    std::cout << "Score: " << obtainScore() << '\n';
}

int Gotoh::obtainScore(){
    std::vector<int> last_h(s1.length()+1, 0);
    std::vector<int> last_e(s1.length()+1, 0);
    std::vector<int> last_f(s1.length()+1, 0);

    std::vector<int> current_h(s1.length()+1, 0);
    std::vector<int> current_e(s1.length()+1, 0);
    std::vector<int> current_f(s1.length()+1, 0);

    for (size_t i = 0; i <= s0.length(); i++)
    {
        for (size_t j = 0; j <= s1.length(); j++)
        {

            // Setar (0,0) para 0, primeira linha para gap
            if (i == 0)
            {
                current_h[j] = 0;
                current_e[j] = -1000;
                current_f[j] = -3000;
    
            }
            // Setar primeira coluna com gaps
            else if (j == 0)
            {
                current_h[j] = 0;
                current_e[j] = -3000;
                current_f[j] = -1000;
            }

            // calculo de escore
            else
            {
                int matchScore = (s0[i - 1] == s1[j-1]) ? match : mismatch;
                current_f[j] = std::max({  last_f[j] + gap_ext, 
                                            last_h[j] + gap_open});
                current_e[j] = std::max({ current_e[j - 1] + gap_ext, 
                                            current_h[j - 1] + gap_open});
                current_h[j] = std::max({  last_h[j - 1] + matchScore, 
                                            current_f[j],
                                            current_e[j],
                                            0});
                
                if (current_h[j] > maxScore){
                    maxScore = current_h[j];
                    maxPoint.first = i;
                    maxPoint.second = j;
                }
            }
        }
        last_h = current_h;
        last_e = current_e;
        last_f = current_f;
    }
    return maxScore;
}

