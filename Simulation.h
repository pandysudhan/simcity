#ifndef SIMULATION_H
#define SIMULATION_H

#include "Grid.h"
#include <string>

class Simulation {
private:
    Grid grid;
    int maxTimeSteps;
    int refreshRate;
    int currentTimeStep;

public:
    Simulation();
    
    void loadConfig(const std::string& filename);
    void run();
    void displayFinalStats();
    void analyzeArea();

private:
    bool simulateTimeStep();
    void displayCurrentState() const;
};

#endif 