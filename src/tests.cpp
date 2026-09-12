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


void testTimeInt16() {
    std::string folderPath = "tests";
    if (!fs::exists(folderPath)) fs::create_directory(folderPath);

    std::ofstream outFile(folderPath + "/benchmark_lazyF_int16.txt");
    if (!outFile.is_open()) return;

    outFile << "INT16:\n";
    std::cout << "INT16:\n";
    std::vector<std::string> folders = {"10k", "18k", "30k", "50k", "150k"};

    for (const auto& folder : folders) {
        auto fastaFiles = getFastaFiles("Sequences/" + folder);
        std::string seq0 = readFasta(fastaFiles[0]);
        std::string seq1 = readFasta(fastaFiles[1]);

        std::cout << "Folder: " << folder << "\n\n";
        outFile << "Folder: " << folder << "\n\n";

        runBenchmark<vint16m1_t>(outFile, seq0, seq1, 1, 16, 5);
        runBenchmark<vint16m2_t>(outFile, seq0, seq1, 2, 32, 5);
        runBenchmark<vint16m4_t>(outFile, seq0, seq1, 4, 64, 5);
        runBenchmark<vint16m8_t>(outFile, seq0, seq1, 8, 128, 5);
    }
}

void testTimeInt32() {
    std::string folderPath = "tests";
    if (!fs::exists(folderPath)) fs::create_directory(folderPath);

    std::ofstream outFile(folderPath + "/benchmark_lazyF_int32.txt");
    if (!outFile.is_open()) return;

    outFile << "INT32:\n";
    std::vector<std::string> folders = {"10k", "18k", "30k", "50k", "150k"};

    for (const auto& folder : folders) {
        auto fastaFiles = getFastaFiles("Sequences/" + folder);
        std::string seq0 = readFasta(fastaFiles[0]);
        std::string seq1 = readFasta(fastaFiles[1]);

        std::cout << "Benchmarking INT32 - Folder: " << folder << std::endl;

        runBenchmark<vint32m1_t>(outFile, seq0, seq1, 1, 8, 5);
        runBenchmark<vint32m2_t>(outFile, seq0, seq1, 2, 16, 5);
        runBenchmark<vint32m4_t>(outFile, seq0, seq1, 4, 32, 5);
        runBenchmark<vint32m8_t>(outFile, seq0, seq1, 8, 64, 5);
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

template <typename VecType>
void runBenchmark(std::ofstream& outFile, const std::string& seq0,
                const std::string& seq1, int lmul, int stripeWidth,
                int executions) {
    outFile << "LMUL " << lmul << ":\n";
    std::cout << "LMUL " << lmul << ":\n";

    double totalTime = 0.0;

    for (int j = 0; j < executions; ++j) {
        auto start = std::chrono::high_resolution_clock::now();
        
        FarrarRvv<VecType> aligner(seq0, seq1, stripeWidth);
        int score = aligner.obtainScore();
        
        auto end = std::chrono::high_resolution_clock::now();
        aligner.clearData();

        std::chrono::duration<double> duration = end - start;
        double seconds = duration.count();
        totalTime += seconds;

        outFile << "Execution " << j+1 << "\nScore: " << score << "\nTime: " << seconds << "s\n\n";
        std::cout << "Execution " << j+1 << "\nScore: " << score << "\nTime: " << seconds << "s\n\n";
        outFile.flush();

    }

    outFile << "Media of " << executions << " executions: " << (totalTime / executions) << " s\n\n\n";
    std::cout << "Media of " << executions << " executions: " << (totalTime / executions) << " s\n\n\n";
    outFile.flush();
}