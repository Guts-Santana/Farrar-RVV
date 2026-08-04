#include "vectorial_call.hpp"

bool isRiscv() {
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    while (std::getline(cpuinfo, line)) {
        if (line.find("riscv") != std::string::npos) {
            return true;
        }
    }
    return false;
}

bool hasRISCVVectorExtension() {
    if (isRiscv())
    {
        std::ifstream cpuinfo("/proc/cpuinfo");
        std::string line;
        while (std::getline(cpuinfo, line)) {
            if (line.find("isa") != std::string::npos &&
                line.find("v") != std::string::npos) {
                return true;
            }
        }
    }
    return false;
}