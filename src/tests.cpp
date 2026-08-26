#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sys/resource.h>

#include "tests.hpp"

#include "Gotoh.hpp"
#include "Farrar.hpp"
#include "FarrarRvv.hpp"


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



void testScore(bool visual){
    std::vector<std::string> folders = {"10k", "18k"};
    for (size_t i = 0; i < folders.size(); i++)
    {
        auto fastaFiles = getFastaFiles("Sequences/"+folders[i]);
        std::string seq0 = readFasta(fastaFiles[0]);
        std::string seq1 = readFasta(fastaFiles[1]);

        std::cout << "Folder: " << folders[i] << '\n';
        Farrar<ScalarVec>* farrarComparison = new Farrar<ScalarVec>(seq0, seq1);

        auto start = std::chrono::high_resolution_clock::now();

        farrarComparison->call(visual);

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


void testTime(bool visual){
	std::vector<std::string> folders = {"10k", "18k", "30k", "50k", "150k"};
    std::cout << "Stripe: " << STRIPE_WIDTH << '\n';
    for (size_t i = 0; i < folders.size(); i++)
    {
        auto fastaFiles = getFastaFiles("Sequences/"+folders[i]);
        std::string seq0 = readFasta(fastaFiles[0]);
        std::string seq1 = readFasta(fastaFiles[1]);
        int score;
        std::cout << "Folder: " << folders[i] << '\n';
        auto end = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::high_resolution_clock::now();

        switch (STRIPE_WIDTH) {
            case 16: {
                // LMUL = 1 (16 elements per vector register)
                FarrarRvv<vint16m1_t> aligner(seq0, seq1, 16);
                score = aligner.obtainScore();
                end = std::chrono::high_resolution_clock::now();
                aligner.clearData();
                break;
            }
            case 32: {
                // LMUL = 2 (32 elements per vector register group)
                FarrarRvv<vint16m2_t> aligner(seq0, seq1, 32);
                score = aligner.obtainScore();
                end = std::chrono::high_resolution_clock::now();
                aligner.clearData();
                break;
            }
            case 64: {
                // LMUL = 4 (64 elements per vector register group)
                FarrarRvv<vint16m4_t> aligner(seq0, seq1, 64);
                score = aligner.obtainScore();
                end = std::chrono::high_resolution_clock::now();
                aligner.clearData();
                break;
            }
            case 128: {
                // LMUL = 8 (128 elements per vector register group)
                FarrarRvv<vint16m8_t> aligner(seq0, seq1, 128);
                score = aligner.obtainScore();
                end = std::chrono::high_resolution_clock::now();
                aligner.clearData();
                break;
            }
            default:
                std::cerr << "Error: Unsupported stripe_width (" << STRIPE_WIDTH
                        << "). Supported values: 16, 32, 64, 128.\n";
                return;
        }
        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << "Score: " << score << '\n';
        std::cout << "Farrar Function execution time: " << duration.count()/1000 << " s\n";
        std::cout << '\n' << '\n';
    }
}


void testFarrarTime(bool visual){
    std::vector<std::string> folders = {"10k", "18k", "30k", "50k"};
    for (size_t i = 0; i < folders.size(); i++)
    {
        auto fastaFiles = getFastaFiles("Sequences/"+folders[i]);
        std::string seq0 = readFasta(fastaFiles[0]);
        std::string seq1 = readFasta(fastaFiles[1]);

        std::cout << "Folder: " << folders[i] << '\n';
        Farrar<ScalarVec>* farrarScalar = new Farrar<ScalarVec>(seq0, seq1);
        auto start = std::chrono::high_resolution_clock::now();
        farrarScalar->call(visual);
        auto end = std::chrono::high_resolution_clock::now();
        delete farrarScalar;
        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << "Scalar Farrar Function execution time: " << duration.count()/1000 << " ms\n";

        Farrar<RvvVec>* farrarRVV = new Farrar<RvvVec>(seq0, seq1);
        start = std::chrono::high_resolution_clock::now();
        farrarRVV->call(visual);
        end = std::chrono::high_resolution_clock::now();
        delete farrarRVV;
        duration = end - start;
        std::cout << "Rvv Farrar Function execution time: " << duration.count()/1000 << " ms\n";

        std::cout << '\n' << '\n';
    }
}
