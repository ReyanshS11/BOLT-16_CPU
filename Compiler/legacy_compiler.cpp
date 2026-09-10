#include <iostream>

#include <unordered_map>

#include <fstream>
#include <sstream>

#include <cstdint>

std::unordered_map<std::string, std::string> OpCodeMapping {
    {"ldr", "00000"},
    {"str", "00001"},
    {"dbnxt", "00010"},
    {"swp", "00011"},
    {"add", "00100"},
    {"sub", "00101"},
    {"inc", "00110"},
    {"dec", "00111"},
    {"and", "01000"},
    {"or", "01001"},
    {"xor", "01010"},
    {"not", "01011"},
    {"neg", "01100"},
    {"jmp", "01101"},
    {"jz", "01110"},
    {"end", "01111"},
};

std::unordered_map<std::string, std::string> RegisterMapping {
    {"R0", "000"},
    {"R1", "001"},
    {"R2", "010"},
    {"R3", "011"},
    {"R4", "100"},
    {"R5", "101"},
    {"R6", "110"},
    {"R7", "111"}    
};

std::unordered_map<std::string, unsigned int> CompilerVariables;

std::ifstream inputFile {"main.rasm"};
std::ofstream outputFile {"compiled.bin", std::ios::trunc};

void BuildBinary(const std::string& word, std::string& binary)
{
    auto isOpCode = OpCodeMapping.find(word);
    auto isRegister = RegisterMapping.find(word);

    if (isOpCode != OpCodeMapping.end())
    {
        while (binary.size() < 16 && binary.size() > 4)
        {
            binary += "0";
        }

        if (binary.size() == 16)
        {
            outputFile << binary << "\n";
        }

        binary = OpCodeMapping[word];
    }
    else
    {
        binary += RegisterMapping[word];
    }
}

int main()
{
    std::stringstream ss;
    ss << inputFile.rdbuf();

    std::string word;
    std::string binary;

    while (ss >> word) {
        int isNonCommandValue = word.find('@');

        if (isNonCommandValue == 0)
        {
            while (binary.size() < 16)
            {
                binary += "0";
            }

            outputFile << binary << "\n";
            outputFile << word.substr(1) << "\n";

            binary = "";

            continue;
        }

        BuildBinary(word, binary);
    }

    while (binary.size() < 16)
    {
        binary += "0";
    }
        
    outputFile << binary << "\n";

    outputFile.close();
}