#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sys/resource.h>

#include "FarrarRvv.hpp"

namespace fs = std::filesystem;

std::string readFasta(const fs::path& fastaPath);

std::vector<fs::path> getFastaFiles(const fs::path& folder);

void testTimeInt16();

void testTimeInt32();


