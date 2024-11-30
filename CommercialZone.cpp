#include "CommercialZone.h"

bool CommercialZone::canGrow(const Cell& cell, int adjacentPopulation, bool hasPower,
                            int availableWorkers, int availableGoods) {
    if (cell.getPopulation() >= getMaxPopulation()) {
        return false;
    }
    
    // Check resources
    if (availableWorkers < getRequiredWorkers() || availableGoods < getRequiredGoods()) {
        return false;
    }
    
    int requiredPop = getRequiredAdjacentPopulation(cell.getPopulation());
    
    // Special case for initial growth
    if (cell.getPopulation() == 0) {
        return hasPower || adjacentPopulation >= requiredPop;
    }
    
    return adjacentPopulation >= requiredPop;
}

void CommercialZone::grow(Cell& cell, int& availableWorkers, int& availableGoods) {
    cell.setPopulation(cell.getPopulation() + 1);
    availableWorkers -= getRequiredWorkers();
    availableGoods -= getRequiredGoods();
}

int CommercialZone::getRequiredAdjacentPopulation(int currentPopulation) {
    switch (currentPopulation) {
        case 0: return 1;  // Needs 1 adjacent population or power
        case 1: return 2;  // Needs 2 adjacent populations
        default: return 999; // Cannot grow further
    }
} 