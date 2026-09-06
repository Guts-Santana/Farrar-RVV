#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sys/resource.h>

#include "tests.hpp"


namespace fs = std::filesystem;

std::string readFasta(const fs::path& fastaPath)
{
    std::ifstream file(fastaPath);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open " + fastaPath.string());
    }

    std::string line;
    std::string sequence;

    std::getline(file, line);

    while (std::getline(file, line))
    {
        sequence += line;
    }

    return sequence;
}

std::vector<fs::path> getFastaFiles(const fs::path& folder)
{
    std::vector<fs::path> fastaFiles;

    for (const auto& entry : fs::directory_iterator(folder))
    {
        if (entry.path().extension() == ".fasta")
        {
            fastaFiles.push_back(entry.path());
        }
    }

    return fastaFiles;
}


void testTimeInt16(){
    std::vector<std::string> folders = {"10k", "18k", "30k", "50k", "150k"};
        std::cout << "INT16: " << '\n';
    for (size_t i = 0; i < folders.size(); i++)
    {
        auto fastaFiles = getFastaFiles("Sequences/"+folders[i]);
        std::string seq0 = readFasta(fastaFiles[0]);
        std::string seq1 = readFasta(fastaFiles[1]);
	    int score;
        std::cout << "Folder: " << folders[i] << '\n';
        auto end = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::high_resolution_clock::now();

        // LMUL = 1 (16 elements per vector register)
        std::cout << "LMUL: 1" << '\n';
        start = std::chrono::high_resolution_clock::now();
        FarrarRvv<vint16m1_t> alignerm1(seq0, seq1, 16);
        score = alignerm1.obtainScore();
        end = std::chrono::high_resolution_clock::now();
        alignerm1.clearData();

        std::chrono::duration<double, std::milli> duration = end - start;
	    std::cout << "Score: " << score << '\n'; 
        std::cout << "Farrar Function execution time: " << duration.count()/1000 << " s\n";
        std::cout << '\n' << '\n';

        // LMUL = 2 (32 elements per vector register group)
        std::cout << "LMUL: 2" << '\n';
        start = std::chrono::high_resolution_clock::now();
        FarrarRvv<vint16m2_t> alignerm2(seq0, seq1, 32);
        score = alignerm2.obtainScore();
        end = std::chrono::high_resolution_clock::now();
        alignerm2.clearData();

        duration = end - start;
	    std::cout << "Score: " << score << '\n'; 
        std::cout << "Farrar Function execution time: " << duration.count()/1000 << " s\n";
        std::cout << '\n' << '\n';

        // LMUL = 4 (64 elements per vector register group)
        std::cout << "LMUL: 4" << '\n';
        start = std::chrono::high_resolution_clock::now();
        FarrarRvv<vint16m4_t> alignerm4(seq0, seq1, 64);
        score = alignerm4.obtainScore();
        end = std::chrono::high_resolution_clock::now();
        alignerm4.clearData();

        duration = end - start;
	    std::cout << "Score: " << score << '\n'; 
        std::cout << "Farrar Function execution time: " << duration.count()/1000 << " s\n";
        std::cout << '\n' << '\n';

        // LMUL = 8 (128 elements per vector register group)
        std::cout << "LMUL: 8" << '\n';
        start = std::chrono::high_resolution_clock::now();
        FarrarRvv<vint16m8_t> alignerm8(seq0, seq1, 128);
        score = alignerm8.obtainScore();
        end = std::chrono::high_resolution_clock::now();
        alignerm8.clearData();


        duration = end - start;
	    std::cout << "Score: " << score << '\n'; 
        std::cout << "Farrar Function execution time: " << duration.count()/1000 << " s\n";
        std::cout << '\n' << '\n';
    }
}

void testTimeInt32(){
    std::vector<std::string> folders = {"10k", "18k", "30k", "50k", "150k"};
        std::cout << "INT32: " << '\n';
    for (size_t i = 0; i < folders.size(); i++)
    {
        auto fastaFiles = getFastaFiles("Sequences/"+folders[i]);
        std::string seq0 = readFasta(fastaFiles[0]);
        std::string seq1 = readFasta(fastaFiles[1]);
	    int score;
        std::cout << "Folder: " << folders[i] << '\n';
        auto end = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::high_resolution_clock::now();

        // LMUL = 1 (16 elements per vector register)
        std::cout << "LMUL: 1" << '\n';
        start = std::chrono::high_resolution_clock::now();
        FarrarRvv<vint32m1_t> alignerm1(seq0, seq1, 8);
        score = alignerm1.obtainScore();
        end = std::chrono::high_resolution_clock::now();
        alignerm1.clearData();

        std::chrono::duration<double, std::milli> duration = end - start;
	    std::cout << "Score: " << score << '\n'; 
        std::cout << "Farrar Function execution time: " << duration.count()/1000 << " s\n";
        std::cout << '\n' << '\n';

        // LMUL = 2 (32 elements per vector register group)
        std::cout << "LMUL: 2" << '\n';
        start = std::chrono::high_resolution_clock::now();
        FarrarRvv<vint32m2_t> alignerm2(seq0, seq1, 16);
        score = alignerm2.obtainScore();
        end = std::chrono::high_resolution_clock::now();
        alignerm2.clearData();

        duration = end - start;
	    std::cout << "Score: " << score << '\n'; 
        std::cout << "Farrar Function execution time: " << duration.count()/1000 << " s\n";
        std::cout << '\n' << '\n';

        // LMUL = 4 (64 elements per vector register group)
        std::cout << "LMUL: 4" << '\n';
        start = std::chrono::high_resolution_clock::now();
        FarrarRvv<vint32m4_t> alignerm4(seq0, seq1, 32);
        score = alignerm4.obtainScore();
        end = std::chrono::high_resolution_clock::now();
        alignerm4.clearData();

        duration = end - start;
	    std::cout << "Score: " << score << '\n'; 
        std::cout << "Farrar Function execution time: " << duration.count()/1000 << " s\n";
        std::cout << '\n' << '\n';

        // LMUL = 8 (128 elements per vector register group)
        std::cout << "LMUL: 8" << '\n';
        start = std::chrono::high_resolution_clock::now();
        FarrarRvv<vint32m8_t> alignerm8(seq0, seq1, 64);
        score = alignerm8.obtainScore();
        end = std::chrono::high_resolution_clock::now();
        alignerm8.clearData();


        duration = end - start;
	    std::cout << "Score: " << score << '\n'; 
        std::cout << "Farrar Function execution time: " << duration.count()/1000 << " s\n";
        std::cout << '\n' << '\n';
    }
}

void testScalar(){
    std::vector<std::string> folders = {"10k", "18k", "30k", "50k", "150k"};
        std::cout << "Scalar Way: " << '\n';
    for (size_t i = 0; i < folders.size(); i++)
    for (size_t i = 0; i < folders.size(); i++)
    {
        auto fastaFiles = getFastaFiles("Sequences/"+folders[i]);
        std::string seq0 = readFasta(fastaFiles[0]);
        std::string seq1 = readFasta(fastaFiles[1]);

        std::cout << "Folder: " << folders[i] << '\n';
        Farrar<ScalarVec>* farrarComparison = new Farrar<ScalarVec>(seq0, seq1);

        auto start = std::chrono::high_resolution_clock::now();

        farrarComparison->call();

        auto end = std::chrono::high_resolution_clock::now();

        delete farrarComparison;
        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << "Farrar Function execution time: " << duration.count()/1000 << " s\n";

        Gotoh* gotohComparison = new Gotoh(seq0, seq1);

        start = std::chrono::high_resolution_clock::now();

        gotohComparison->call();

        end = std::chrono::high_resolution_clock::now();
        delete gotohComparison;
        duration = end - start;
        std::cout << "Gotoh Function execution time: " << duration.count()/1000 << " s\n";
        std::cout << '\n' << '\n';
    }
}