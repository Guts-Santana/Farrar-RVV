#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sys/resource.h>

#include "Gotoh.hpp"
#include "Farrar.hpp"

namespace fs = std::filesystem;

std::string readFasta(const fs::path& fastaPath);

std::vector<fs::path> getFastaFiles(const fs::path& folder);

void testScore(bool visual);

void testTime(bool visual);