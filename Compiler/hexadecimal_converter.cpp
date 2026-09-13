#include <iostream>

#include <fstream>
#include <sstream>

#include <iomanip>

std::ifstream inputFile {"Compiled/compiled.bin"};
std::ofstream outputFile {"Compiled/compiled.hex", std::ios::trunc};

int main()
{
    std::stringstream ss;
    ss << inputFile.rdbuf();

    std::string byte;

    outputFile << "v2.0 raw" << "\n";

    while (ss >> byte)
    {
        unsigned long long decimal = std::stoull(byte, nullptr, 2);

        outputFile << std::hex << std::uppercase << decimal << "\n";
    }

    outputFile.close();
}