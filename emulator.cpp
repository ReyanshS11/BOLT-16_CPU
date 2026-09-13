#include <iostream>

#include <cstdlib>
#include <cstdint>

#include <fstream>
#include <sstream>

#include <vector>
#include <unordered_map>

#include <bitset>
#include <bit>

std::ifstream inputFile {"Compiler/Assembly/main.rasm"};

std::unordered_map<std::string, std::string> RegisterMapping {
    {"R0", "0000000000000000"},
    {"R1", "0000000000000000"},
    {"R2", "0000000000000000"},
    {"R3", "0000000000000000"},
    {"R4", "0000000000000000"},
    {"R5", "0000000000000000"},
    {"R6", "0000000000000000"},
    {"R7", "0000000000000000"}    
};

std::unordered_map<int, std::string> DataMemory;

std::unordered_map<std::string, std::string> CompilerVariables;

int main()
{
    if (!inputFile.is_open())
    {
        std::cout << "Failed to open assembly file\n";
        return 1;
    }

    std::cout << "Assembly file opened\n";

    int lineAddress = 0;
    std::string line;

    bool jumptoAddress = false;
    std::string labeltoJump;

    std::string previousOperationOutput;
    
    while (std::getline(inputFile, line))
    {
        std::istringstream ss {line};

        std::string arg;
        std::vector<std::string> args;

        while (ss >> arg) {
            args.push_back(arg);
        }

        if (args.empty())
        {
            continue;
        }

        if (jumptoAddress)
        {
            if (labeltoJump != args[1].erase(0, 1))
            {
                continue;
            }
            else
            {
                jumptoAddress = false;
            }
        }

        if (args[0] == "ldr")
        {
            int address = std::stoi(RegisterMapping[args[2]], nullptr, 2);
            RegisterMapping[args[1]] = DataMemory[address];
        }
        else if (args[0] == "str")
        {
            int address = std::stoi(RegisterMapping[args[2]], nullptr, 2);
            DataMemory[address] = RegisterMapping[args[1]];
        }
        else if (args[0] == "db")
        {
            std::bitset<16> binaryWord {std::stoi(args[2].erase(0, 1))};
            RegisterMapping[args[1]] = binaryWord.to_string();
        }
        else if (args[0] == "swp")
        {
            std::string temp = RegisterMapping[args[1]];
            RegisterMapping[args[1]] = RegisterMapping[args[2]];
            RegisterMapping[args[2]] = temp;
        }
        else if (args[0] == "arithmetic")
        {
            if (args[1] == "and")
            {
                std::bitset<16> A {RegisterMapping[args[2]]};
                std::bitset<16> B {RegisterMapping[args[3]]};

                previousOperationOutput = (A & B).to_string();
                
                RegisterMapping[args[2]] = previousOperationOutput;
            }
            else if (args[1] == "or")
            {
                std::bitset<16> A {RegisterMapping[args[2]]};
                std::bitset<16> B {RegisterMapping[args[3]]};

                previousOperationOutput = (A | B).to_string();
                
                RegisterMapping[args[2]] = previousOperationOutput;
            }
            else if (args[1] == "xor")
            {
                std::bitset<16> A {RegisterMapping[args[2]]};
                std::bitset<16> B {RegisterMapping[args[3]]};

                previousOperationOutput = (A ^ B).to_string();
                
                RegisterMapping[args[2]] = previousOperationOutput;
            }
            else if (args[1] == "not")
            {
                std::bitset<16> A {RegisterMapping[args[2]]};

                previousOperationOutput = (~A).to_string();
                
                RegisterMapping[args[2]] = previousOperationOutput;
            }
            else if (args[1] == "neg")
            {
                uint16_t A = static_cast<uint16_t>(std::stoul(RegisterMapping[args[2]], nullptr, 2));

                previousOperationOutput = std::bitset<16>((-A - 1)).to_string();
                
                RegisterMapping[args[2]] = previousOperationOutput;
            }
            else if (args[1] == "add")
            {
                uint16_t A = static_cast<uint16_t>(std::stoul(RegisterMapping[args[2]], nullptr, 2));
                uint16_t B = static_cast<uint16_t>(std::stoul(RegisterMapping[args[3]], nullptr, 2));

                previousOperationOutput = std::bitset<16>((A + B)).to_string();
                
                RegisterMapping[args[2]] = previousOperationOutput;
            }
            else if (args[1] == "sub")
            {
                uint16_t A = static_cast<uint16_t>(std::stoul(RegisterMapping[args[2]], nullptr, 2));
                uint16_t B = static_cast<uint16_t>(std::stoul(RegisterMapping[args[3]], nullptr, 2));

                previousOperationOutput = std::bitset<16>((A - B)).to_string();
                
                RegisterMapping[args[2]] = previousOperationOutput;
            }
            else if (args[1] == "mul")
            {
                uint16_t A = static_cast<uint16_t>(std::stoul(RegisterMapping[args[2]], nullptr, 2));
                uint16_t B = static_cast<uint16_t>(std::stoul(RegisterMapping[args[3]], nullptr, 2));

                previousOperationOutput = std::bitset<16>((A * B)).to_string();
                
                RegisterMapping[args[2]] = previousOperationOutput;
            }
            else if (args[1] == "div")
            {
                uint16_t A = static_cast<uint16_t>(std::stoul(RegisterMapping[args[2]], nullptr, 2));
                uint16_t B = static_cast<uint16_t>(std::stoul(RegisterMapping[args[3]], nullptr, 2));

                previousOperationOutput = std::bitset<16>((A / B)).to_string();
                
                RegisterMapping[args[2]] = previousOperationOutput;
            }
            else if (args[1] == "inc")
            {
                uint16_t A = static_cast<uint16_t>(std::stoul(RegisterMapping[args[2]], nullptr, 2));

                previousOperationOutput = std::bitset<16>((A + 1)).to_string();
                
                RegisterMapping[args[2]] = previousOperationOutput;
            }
            else if (args[1] == "dec")
            {
                uint16_t A = static_cast<uint16_t>(std::stoul(RegisterMapping[args[2]], nullptr, 2));
                uint16_t B = static_cast<uint16_t>(std::stoul(RegisterMapping[args[3]], nullptr, 2));

                previousOperationOutput = std::bitset<16>((A - 1)).to_string();
                
                RegisterMapping[args[2]] = previousOperationOutput;
            }
            else if (args[1] == "shl")
            {
                std::bitset<16> A {RegisterMapping[args[2]]};
                std::bitset<16> B {RegisterMapping[args[3]]};
                unsigned long last_four = B.to_ulong() & 0x0F;

                previousOperationOutput = (A << last_four).to_string();
                
                RegisterMapping[args[2]] = previousOperationOutput;
            }
            else if (args[1] == "shr")
            {
                std::bitset<16> A {RegisterMapping[args[2]]};
                std::bitset<16> B {RegisterMapping[args[3]]};
                unsigned long last_four = B.to_ulong() & 0x0F;

                previousOperationOutput = (A >> last_four).to_string();
                
                RegisterMapping[args[2]] = previousOperationOutput;
            }
            else if (args[1] == "cmp")
            {
                std::bitset<16> A {RegisterMapping[args[2]]};
                std::bitset<16> B {RegisterMapping[args[3]]};

                A == B ? previousOperationOutput = "0000000000000000" : previousOperationOutput = "0000000000000001";
                
                RegisterMapping[args[2]] = previousOperationOutput;
            }
            else if (args[1] == "sar")
            {
                std::bitset<16> BinaryA {RegisterMapping[args[2]]};
                int A = static_cast<int>(BinaryA.to_ulong());

                std::bitset<16> B {RegisterMapping[args[3]]};
                unsigned long last_four = B.to_ulong() & 0x0F;

                previousOperationOutput = std::bitset<16>(A >> last_four).to_string();
                
                RegisterMapping[args[2]] = previousOperationOutput;
            }
            else if (args[1] == "ror") { 
                std::bitset<16> A {RegisterMapping[args[2]]}; 
                std::bitset<16> B {RegisterMapping[args[3]]}; 
                
                unsigned long shift = B.to_ulong() & 0x0F; 

                if (shift == 0) {
                    previousOperationOutput = A.to_string();
                } else {
                    previousOperationOutput = ((A >> shift) | (A << (16 - shift))).to_string();
                }

                RegisterMapping[args[2]] = previousOperationOutput; 
            }
        }
        else if (args[0] == "jmp")
        {
            labeltoJump = args[1];
            jumptoAddress = true;
        }
        else if (args[0] == "jz")
        {
            if (previousOperationOutput == "0000000000000000")
            {
                labeltoJump = args[1];
                jumptoAddress = true;
            }
        }
        else if (args[0] == "label")
        {
            std::bitset<15> address {lineAddress};
            CompilerVariables[args[1].erase(0, 1)] = address.to_string();
        }
        else
        {
            if (args[0] == "end")
            {
                break;
            }
            else
            {
                std::cout << "Encountered an error";
                break;
            }
        }

        lineAddress++;
    }

    for (const auto& [key, value] : DataMemory) {
        std::cout << key << ": " << value << "\n";
    }
}