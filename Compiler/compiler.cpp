#include <iostream>

#include <unordered_map>

#include <fstream>
#include <sstream>

#include <cstdint>

#include <bitset>

std::unordered_map<std::string, std::string> OpCodeMapping {
    {"ldr", "00000"},
    {"str", "00001"},
    {"db", "00010"},
    {"sb", "00011"},
    {"arithmetic", "00100"},
    {"jmp", "00101"},
    {"jz", "00110"},
    {"clear", "00111"},
    {"end", "01000"},
    {"label", "01001"}
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

std::unordered_map<std::string, std::string> ArithmeticOperationMapping {
    {"and", "0000"},
    {"or", "0001"},
    {"xor", "0010"},
    {"not", "0011"},
    {"neg", "0100"},
    {"add", "0101"},
    {"sub", "0110"},  
    {"mul", "0111"},
    {"div", "1000"},
    {"inc", "1001"},
    {"dec", "1010"},
    {"shl", "1011"},
    {"shr", "1100"},
    {"cmp", "1101"},
    {"sar", "1110"},
    {"ror", "1111"}
};

std::unordered_map<std::string, std::string> CompilerVariables;

std::ifstream inputFile {"Assembly/main.rasm"};
std::ofstream outputFile {"Compiled/compiled.bin", std::ios::trunc};

bool arithmetic = false;

void BuildBinary(const std::string& word, std::string& binary)
{
    auto isOpCode = OpCodeMapping.find(word);
    auto isArithmetic = ArithmeticOperationMapping.find(word);

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
    else if (isArithmetic != ArithmeticOperationMapping.end())
    {
        binary += ArithmeticOperationMapping[word];
    }
    else
    {
        binary += RegisterMapping[word];
    }
}

void findLabels()
{
    std::string line;

    int address = 0;

    while (std::getline(inputFile, line))
    {
        if (line.find("label") != std::string::npos)
        {
            std::bitset<16> binaryAddress {address};
            CompilerVariables[line.erase(0, 7)] = binaryAddress.to_string();
        }

        address++;
    }

    inputFile.clear();
    inputFile.seekg(0);
}

int main()
{
    findLabels();

    std::stringstream ss;
    ss << inputFile.rdbuf();

    std::string word;
    std::string binary;

    bool skip = false;

    while (ss >> word)
    {
        if (word.find('@') == 0)
        {
            while (binary.size() < 16)
            {
                binary += "0";
            }

            outputFile << binary << "\n";

            std::bitset<16> binaryWord {std::stoi(word.erase(0, 1))};

            outputFile << binaryWord.to_string() << "\n";

            binary = "";

            continue;
        }
        else if (word.find('$') == 0)
        {            
            while (binary.size() < 16)
            {
                binary += "0";
            }

            outputFile << binary << "\n";

            if (binary == "0100000000000000")
            {
                continue;
            }

            outputFile << CompilerVariables[word.erase(0, 1)] << "\n";

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