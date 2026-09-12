#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sys/resource.h>

#include "FarrarRvv.hpp"
#include "Gotoh.hpp"
#include "Farrar.hpp"

namespace fs = std::filesystem;

std::string readFasta(const fs::path& fastaPath);

std::vector<fs::path> getFastaFiles(const fs::path& folder);

void testTimeInt16();

void testTimeInt32();

void testScalar();

template <typename VecType>
void runBenchmark(std::ofstream& outFile,const std::string& seq0,
                const std::string& seq1, int lmul, int stripeWidth,
                int executions);


