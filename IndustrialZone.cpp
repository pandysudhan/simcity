#include "IndustrialZone.h"

bool IndustrialZone::canGrow(const Cell& cell, int adjacentPopulation, bool hasPower, int availableWorkers) {
    if (cell.getPopulation() >= getMaxPopulation()) {
        return false;
    }
    
    // Check workers
    if (availableWorkers < getRequiredWorkers()) {
        return false;
    }
    
    int requiredPop = getRequiredAdjacentPopulation(cell.getPopulation());
    
    // Special case for initial growth
    if (cell.getPopulation() == 0) {
        return hasPower || adjacentPopulation >= requiredPop;
    }
    
    return adjacentPopulation >= requiredPop;
}

void IndustrialZone::grow(Cell& cell, int& availableWorkers) {
    cell.setPopulation(cell.getPopulation() + 1);
    availableWorkers -= getRequiredWorkers();
}

int IndustrialZone::getRequiredAdjacentPopulation(int currentPopulation) {
    switch (currentPopulation) {
        case 0: return 1;  // Needs 1 adjacent population or power
        case 1: return 2;  // Needs 2 adjacent populations
        case 2: return 4;  // Needs 4 adjacent populations
        default: return 999; // Cannot grow further
    }
} 