#include <iostream>
#include <string>
#include "Simulation.h"

int main() {
    std::string configFile;
    std::cout << "Enter configuration file name: ";
    std::cin >> configFile;
    
    Simulation sim;
    sim.loadConfig(configFile);
    sim.run();
    sim.displayFinalStats();
    sim.analyzeArea();
    
    return 0;
} 