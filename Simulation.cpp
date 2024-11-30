#include "Simulation.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

Simulation::Simulation() : grid(0, 0), maxTimeSteps(0), refreshRate(0), currentTimeStep(0) {}

void Simulation::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::string regionFile;
    
    while (std::getline(file, line)) {
        // Skip empty lines
        if (line.empty()) continue;
        
        // Find the colon separator
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) continue;
        
        // Extract label and value
        std::string label = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);
        
        // Remove any whitespace
        value.erase(0, value.find_first_not_of(" \t\r\n"));
        value.erase(value.find_last_not_of(" \t\r\n") + 1);
        
        if (label == "Region Layout") {
            regionFile = value;
        } else if (label == "Time Limit") {
            maxTimeSteps = std::stoi(value);
        } else if (label == "Refresh Rate") {
            refreshRate = std::stoi(value);
        }
    }
    
    // Count rows and columns in region file
    std::ifstream regionF(regionFile);
    std::string regionLine;
    int rows = 0, cols = 0;
    while (std::getline(regionF, regionLine)) {
        rows++;
        if (rows == 1) {
            int count = 1;  // Start with 1 for the first column
            for (char c : regionLine) {
                if (c == ',') count++;
            }
            cols = count;
        }
    }
    
    // Initialize grid with correct dimensions
    grid = Grid(rows, cols);
    grid.loadFromCSV(regionFile);
}

bool Simulation::simulateTimeStep() {
    bool changed = false;
    
    // Update available workers and goods
    grid.updateAvailableWorkers();
    grid.updateAvailableGoods();
    
    // Update cells in priority order
    // First commercial, then industrial
    for (int priority = 0; priority < 2; priority++) {
        for (int pop = 5; pop >= 0; pop--) {
            for (int i = 0; i < grid.getRows(); i++) {
                for (int j = 0; j < grid.getCols(); j++) {
                    Cell& cell = grid.getCell(i, j);
                    if ((priority == 0 && cell.getType() == COMMERCIAL) ||
                        (priority == 1 && cell.getType() == INDUSTRIAL)) {
                        if (cell.getPopulation() == pop) {
                            changed |= grid.updateCell(i, j);
                        }
                    }
                }
            }
        }
    }
    
    // Update residential zones
    for (int pop = 5; pop >= 0; pop--) {
        for (int i = 0; i < grid.getRows(); i++) {
            for (int j = 0; j < grid.getCols(); j++) {
                Cell& cell = grid.getCell(i, j);
                if (cell.getType() == RESIDENTIAL && cell.getPopulation() == pop) {
                    changed |= grid.updateCell(i, j);
                }
            }
        }
    }
    
    // Calculate pollution
    grid.calculatePollution();
    
    return changed;
}

void Simulation::run() {
    std::cout << "Initial region state:" << std::endl;
    grid.displayGrid();
    std::cout << std::endl;
    
    bool changed = true;
    while (changed && currentTimeStep < maxTimeSteps) {
        currentTimeStep++;
        changed = simulateTimeStep();
        
        if (currentTimeStep % refreshRate == 0 || !changed || currentTimeStep == maxTimeSteps) {
            std::cout << "Time step: " << currentTimeStep << std::endl;
            std::cout << "Available workers: " << grid.getTotalResidentialPopulation() << std::endl;
            std::cout << "Available goods: " << grid.getTotalIndustrialPopulation() << std::endl;
            grid.displayGrid();
            std::cout << std::endl;
        }
    }
}

void Simulation::displayFinalStats() {
    std::cout << "\nFinal region state:" << std::endl;
    grid.displayGrid();
    
    std::cout << "\nFinal regional population:" << std::endl;
    std::cout << "Residential: " << grid.getTotalResidentialPopulation() << std::endl;
    std::cout << "Industrial: " << grid.getTotalIndustrialPopulation() << std::endl;
    std::cout << "Commercial: " << grid.getTotalCommercialPopulation() << std::endl;
    
    std::cout << "\nPollution state:" << std::endl;
    grid.displayPollution();
    std::cout << "Total pollution: " << grid.getTotalPollution() << std::endl;
}

void Simulation::analyzeArea() {
    int x1, y1, x2, y2;
    bool validInput = false;
    
    while (!validInput) {
        std::cout << "\nEnter coordinates for area analysis (x1 y1 x2 y2): ";
        std::cin >> x1 >> y1 >> x2 >> y2;
        
        if (x1 >= 0 && x1 < grid.getRows() && y1 >= 0 && y1 < grid.getCols() &&
            x2 >= 0 && x2 < grid.getRows() && y2 >= 0 && y2 < grid.getCols()) {
            validInput = true;
        } else {
            std::cout << "Invalid coordinates. Please try again." << std::endl;
        }
    }
    
    int resPop, indPop, comPop, pollution;
    grid.getAreaStatistics(x1, y1, x2, y2, resPop, indPop, comPop, pollution);
    
    std::cout << "\nArea analysis:" << std::endl;
    std::cout << "Residential population: " << resPop << std::endl;
    std::cout << "Industrial population: " << indPop << std::endl;
    std::cout << "Commercial population: " << comPop << std::endl;
    std::cout << "Total pollution: " << pollution << std::endl;
} 