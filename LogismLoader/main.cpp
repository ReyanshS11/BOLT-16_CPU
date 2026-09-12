#include <iostream>
#include <cstdlib>

int main()
{
    unsigned long long binaryOutput = system("java -jar C:/Program Files/logisim-evolution/app/logisim-evolution-4.1.0-all.jar C:/Reyansh/CPU/CPU.circ -tty table > simulation_results.txt 2>&1 &");

    std::cout << "Binary Output" << binaryOutput << "\n";
}