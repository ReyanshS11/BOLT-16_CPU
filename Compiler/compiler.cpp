#include <iostream>

#include <vector>
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

int main()
{
    int lineAddress = 0;
    std::string line;

    bool jumptoAddress = false;
    int addressToJump;

    std::vector<std::string> lines;

    while (std::getline(inputFile, line))
    {
        if (!line.empty())
        {
            if (line != "end")
            {
                lines.emplace_back(line);
            }
        }
    }

    int startLabelAddress = 0;

    for (int i = 0; i < lines.size(); i++)
    {
        if (lines[i].find("label $start") != std::string::npos)
        {
            startLabelAddress = i;
            break;
        }
    }

    std::vector<std::string> reorderedLines;

    for (int i = startLabelAddress; i < lines.size(); i++)
    {
        reorderedLines.emplace_back(lines[i]);
    }

    for (int i = 0; i < startLabelAddress; i++)
    {
        reorderedLines.emplace_back(lines[i]);
    }

    reorderedLines.emplace_back("end");

    for (const std::string& currentLine : reorderedLines)
    {
        std::istringstream ss(currentLine);

        std::string arg;
        std::vector<std::string> args;

        while (ss >> arg)
        {
            args.push_back(arg);
        }

        if (args.empty())
            continue;

        if (args[0] == "label")
        {
            std::string labelName = args[1].substr(1);

            std::bitset<16> binaryAddress(lineAddress);

            CompilerVariables[labelName] = binaryAddress.to_string();

            lineAddress++;
        }
        else if (args[0] == "db" || args[0] == "sb" ||
                args[0] == "jmp" || args[0] == "jz")
        {
            lineAddress += 2;
        }
        else
        {
            lineAddress++;
        }
    }
    for (int i = 0; i < reorderedLines.size(); i++)
    {
        std::string binary;

        line = reorderedLines[i];

        std::istringstream ss {line};

        std::string arg;
        std::vector<std::string> args;

        while (ss >> arg) {
            args.push_back(arg);
        }

        if (jumptoAddress)
        {
            i = addressToJump - 1;
            jumptoAddress = false;
        }

        if (args[0] == "ldr")
        {
            std::string DST = RegisterMapping[args[1]];
            std::string SRC = RegisterMapping[args[2]];

            outputFile << OpCodeMapping[args[0]] + DST + SRC + "00000" + "\n";
            continue;
        }
        else if (args[0] == "str")
        {
            std::string DST = RegisterMapping[args[1]];
            std::string SRC = RegisterMapping[args[2]];

            outputFile << OpCodeMapping[args[0]] + DST + SRC + "00000" + "\n";
            continue;
        }
        else if (args[0] == "db")
        {
            std::string REG = RegisterMapping[args[1]];
            std::string VAL = std::bitset<16>(std::stoi(args[2].erase(0, 1))).to_string();

            outputFile << OpCodeMapping[args[0]] + REG + "00000000" + "\n";
            outputFile << VAL << "\n";
            continue;
        }
        else if (args[0] == "sb")
        {
            std::string REG = RegisterMapping[args[1]];
            std::string VAL = std::bitset<16>(std::stoi(args[2].erase(0, 1))).to_string();

            outputFile << OpCodeMapping[args[0]] + REG + "00000000" + "\n";
            outputFile << VAL << "\n";
            continue;
        }
        else if (args[0] == "arithmetic")
        {
            std::string start = OpCodeMapping[args[0]];

            if (args[1] == "and")
            {
                std::string DST = RegisterMapping[args[2]];
                std::string SRC = RegisterMapping[args[3]];

                outputFile << start + ArithmeticOperationMapping[args[1]] + DST + SRC + "\n";
                continue;
            }
            else if (args[1] == "or")
            {
                std::string DST = RegisterMapping[args[2]];
                std::string SRC = RegisterMapping[args[3]];

                outputFile << start + ArithmeticOperationMapping[args[1]] + DST + SRC + "\n";
                continue;
            }
            else if (args[1] == "xor")
            {
                std::string DST = RegisterMapping[args[2]];
                std::string SRC = RegisterMapping[args[3]];

                outputFile << start + ArithmeticOperationMapping[args[1]] + DST + SRC + "\n";
                continue;
            }
            else if (args[1] == "not")
            {
                std::string DST = RegisterMapping[args[2]];

                outputFile << start + ArithmeticOperationMapping[args[1]] + DST + "000" + "\n";
                continue;
            }
            else if (args[1] == "neg")
            {
                std::string DST = RegisterMapping[args[2]];

                outputFile << start + ArithmeticOperationMapping[args[1]] + DST + "000" + "\n";
                continue;
            }
            else if (args[1] == "add")
            {
                std::string DST = RegisterMapping[args[2]];
                std::string SRC = RegisterMapping[args[3]];

                outputFile << start + ArithmeticOperationMapping[args[1]] + DST + SRC + "\n";
                continue;
            }
            else if (args[1] == "sub")
            {
                std::string DST = RegisterMapping[args[2]];
                std::string SRC = RegisterMapping[args[3]];

                outputFile << start + ArithmeticOperationMapping[args[1]] + DST + SRC + "\n";
                continue;
            }
            else if (args[1] == "mul")
            {
                std::string DST = RegisterMapping[args[2]];
                std::string SRC = RegisterMapping[args[3]];

                outputFile << start + ArithmeticOperationMapping[args[1]] + DST + SRC + "\n";
                continue;
            }
            else if (args[1] == "div")
            {
                std::string DST = RegisterMapping[args[2]];
                std::string SRC = RegisterMapping[args[3]];

                outputFile << start + ArithmeticOperationMapping[args[1]] + DST + SRC + "\n";
                continue;
            }
            else if (args[1] == "inc")
            {
                std::string DST = RegisterMapping[args[2]];

                outputFile << start + ArithmeticOperationMapping[args[1]] + DST + "0000" + "\n";
                continue;
            }
            else if (args[1] == "dec")
            {
                std::string DST = RegisterMapping[args[2]];

                outputFile << start + ArithmeticOperationMapping[args[1]] + DST + "0000" + "\n";
                continue;
            }
            else if (args[1] == "shl")
            {
                std::string DST = RegisterMapping[args[2]];

                int dist = std::stoi(args[3].erase(0, 1));
                std::string DIST = std::bitset<3>(dist).to_string();

                outputFile << start + ArithmeticOperationMapping[args[1]] + DST + DIST + "\n";
                continue;
            }
            else if (args[1] == "shr")
            {
                std::string DST = RegisterMapping[args[2]];

                int dist = std::stoi(args[3].erase(0, 1));
                std::string DIST = std::bitset<3>(dist).to_string();

                outputFile << start + ArithmeticOperationMapping[args[1]] + DST + DIST + "\n";
                continue;
            }
            else if (args[1] == "cmp")
            {
                std::string DST = RegisterMapping[args[2]];
                std::string SRC = RegisterMapping[args[3]];

                outputFile << start + ArithmeticOperationMapping[args[1]] + DST + SRC + "\n";
                continue;
            }
            else if (args[1] == "sar")
            {
                std::string DST = RegisterMapping[args[2]];

                int dist = std::stoi(args[3].erase(0, 1));
                std::string DIST = std::bitset<3>(dist).to_string();

                outputFile << start + ArithmeticOperationMapping[args[1]] + DST + DIST + "\n";
                continue;
            }
            else if (args[1] == "ror") { 
                std::string DST = RegisterMapping[args[2]];

                int dist = std::stoi(args[3].erase(0, 1));
                std::string DIST = std::bitset<3>(dist).to_string();

                outputFile << start + ArithmeticOperationMapping[args[1]] + DST + DIST + "\n";
                continue;
            }
        }
        else if (args[0] == "jmp")
        {
            outputFile << OpCodeMapping[args[0]] << "00000000000" << "\n";
            outputFile << CompilerVariables[args[1].erase(0, 1)] << "\n";
            continue;
        }
        else if (args[0] == "jz")
        {
            outputFile << OpCodeMapping[args[0]] << "00000000000" << "\n";
            outputFile << CompilerVariables[args[1].erase(0, 1)] << "\n";
            continue;
        }
        else if (args[0] == "label")
        {
            outputFile << OpCodeMapping[args[0]] << "00000000000" << "\n";
            continue;
        }
        else if (args[0] == "clear")
        {
            outputFile << OpCodeMapping[args[0]] << RegisterMapping[args[1]] << "00000000" << "\n";
        }
        else if (args[0] == "end")
        {
            outputFile << OpCodeMapping[args[0]] << "00000000000" << "\n";
            break;
        }
        else
        {
            std::cout << "Encountered an error";
            break;
        }
    }
}