#include <iostream>
#include <string>
#include "NWAlgorithm.hpp"
#include "Gotoh.hpp"
#include "Farrar.hpp"
#include "tests.hpp"
#include "ScalarVec.hpp"


int main(int argc, char *argv[])
{
    std::string sequence0 = "ACGCGTAG";
    std::string sequence1 = "ACGAACGTA";

    if (argc != 2)
    {
        return 1;
    }

    if (argv[1] == std::string("gotoh"))
    {
        Gotoh gotoh(sequence0, sequence1);
        gotoh.call();
    }

    if (argv[1] == std::string("farrar"))
    {

        Farrar<ScalarVec> farrar(sequence0, sequence1);
        farrar.call();
    }
    if (argv[1] == std::string("scalar"))
    {
        testScalar();
    }

    if (argv[1] == std::string("rvv"))
    {
        testTimeInt16();
        testTimeInt32();

    }

    

    return 0;
}
