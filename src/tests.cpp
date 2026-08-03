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
        Farrar* farrarComparison = new Farrar(seq0, seq1);

        auto start = std::chrono::high_resolution_clock::now();

        farrarComparison->call(visual);

        auto end = std::chrono::high_resolution_clock::now();

        delete farrarComparison;
        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << "Farrar Function execution time: " << duration.count() << " ms\n";

        Gotoh* gotohComparison = new Gotoh(seq0, seq1);

        start = std::chrono::high_resolution_clock::now();

        gotohComparison->call(visual);

        end = std::chrono::high_resolution_clock::now();
        delete gotohComparison;
        duration = end - start;
        std::cout << "Gotoh Function execution time: " << duration.count() << " ms\n";


        
        std::cout << '\n' << '\n';
    }
}