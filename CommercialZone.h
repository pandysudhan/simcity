#ifndef COMMERCIAL_ZONE_H
#define COMMERCIAL_ZONE_H

#include "Cell.h"

class CommercialZone {
public:
    static bool canGrow(const Cell& cell, int adjacentPopulation, bool hasPower, 
                       int availableWorkers, int availableGoods);
    static void grow(Cell& cell, int& availableWorkers, int& availableGoods);
    static int getRequiredAdjacentPopulation(int currentPopulation);
    static int getMaxPopulation() { return 2; }
    static int getRequiredWorkers() { return 1; }
    static int getRequiredGoods() { return 1; }
};

#endif 