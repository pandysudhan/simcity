#ifndef INDUSTRIAL_ZONE_H
#define INDUSTRIAL_ZONE_H

#include "Cell.h"

class IndustrialZone {
public:
    static bool canGrow(const Cell& cell, int adjacentPopulation, bool hasPower, int availableWorkers);
    static void grow(Cell& cell, int& availableWorkers);
    static int getRequiredAdjacentPopulation(int currentPopulation);
    static int getMaxPopulation() { return 3; }
    static int getRequiredWorkers() { return 2; }
    static int getGoodsProduced(int population) { return population; }
    static int getPollutionLevel(int population) { return population; }
};

#endif 