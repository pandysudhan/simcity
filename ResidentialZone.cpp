#include "ResidentialZone.h"

bool ResidentialZone::canGrow(const Cell& cell, int adjacentPopulation, bool hasPower) {
    if (cell.getPopulation() >= getMaxPopulation()) {
        return false;
    }
    
    int requiredPop = getRequiredAdjacentPopulation(cell.getPopulation());
    
    // Special case for initial growth
    if (cell.getPopulation() == 0) {
        return hasPower || adjacentPopulation >= requiredPop;
    }
    
    return adjacentPopulation >= requiredPop;
}

void ResidentialZone::grow(Cell& cell) {
    cell.setPopulation(cell.getPopulation() + 1);
}

int ResidentialZone::getRequiredAdjacentPopulation(int currentPopulation) {
    switch (currentPopulation) {
        case 0: return 1;  // Needs 1 adjacent population or power
        case 1: return 2;  // Needs 2 adjacent populations
        case 2: return 4;  // Needs 4 adjacent populations
        case 3: return 6;  // Needs 6 adjacent populations
        case 4: return 8;  // Needs 8 adjacent populations
        default: return 999; // Cannot grow further
    }
} 