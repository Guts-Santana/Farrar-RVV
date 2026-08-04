#include <iostream>
#include <string>
#include "NWAlgorithm.hpp"
#include "Gotoh.hpp"
#include "Farrar.hpp"
#include "tests.hpp"
#include "vectorial_call.hpp"
#include "ScalarVec.hpp"
#include "RvvVec.hpp"


int main(int argc, char *argv[])
{
    std::string sequence0 = "ACGCGTAG";
    std::string sequence1 = "ACGAACGTA";

    if (argc != 2)
    {
        return 1;
    }

    if (argv[1] == std::string("nw"))
    {
        NWAlgorithm nw(sequence0, sequence1);
        nw.call(true);
    }

    if (argv[1] == std::string("gotoh"))
    {
        Gotoh gotoh(sequence0, sequence1);
        gotoh.call(true);
    }

    if (argv[1] == std::string("farrar"))
    {

        Farrar<ScalarVec> farrar(sequence0, sequence1);
        farrar.call(true);
    }
    if (argv[1] == std::string("tests"))
    {
        testTime(false);
    }

    if (argv[1] == std::string("rvv"))
    {
        std::cout << isRiscv() << '\n';
        std::cout << hasRISCVVectorExtension() << '\n';

        Farrar<RvvVec> farrar(sequence0, sequence1);
        farrar.call(true);
    }

    

    return 0;
}
