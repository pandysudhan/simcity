#ifndef RESIDENTIAL_ZONE_H
#define RESIDENTIAL_ZONE_H

#include "Cell.h"

class ResidentialZone {
public:
    static bool canGrow(const Cell& cell, int adjacentPopulation, bool hasPower);
    static void grow(Cell& cell);
    static int getRequiredAdjacentPopulation(int currentPopulation);
    static int getMaxPopulation() { return 5; }
    static int getWorkerCount(int population) { return population; }
};

#endif 