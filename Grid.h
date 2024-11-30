#ifndef GRID_H
#define GRID_H

#include <vector>
#include "Cell.h"
#include "ResidentialZone.h"
#include "CommercialZone.h"
#include "IndustrialZone.h"

class Grid {
private:
    std::vector<std::vector<Cell>> cells;
    int rows;
    int cols;
    int availableWorkers;
    int availableGoods;

public:
    Grid(int rows, int cols);
    
    void loadFromCSV(const std::string& filename);
    void simulate();
    bool updateCell(int x, int y);
    void calculatePollution();
    void displayGrid() const;
    void displayPollution() const;
    
    int getRows() const;
    int getCols() const;
    Cell& getCell(int x, int y);
    const Cell& getCell(int x, int y) const;
    
    int getTotalResidentialPopulation() const;
    int getTotalIndustrialPopulation() const;
    int getTotalCommercialPopulation() const;
    int getTotalPollution() const;
    
    void getAreaStatistics(int x1, int y1, int x2, int y2, 
                          int& resPop, int& indPop, int& comPop, int& pollution) const;
    
    int countAdjacentPopulation(int x, int y, int minPop) const;
    bool hasAdjacentPowerline(int x, int y) const;
    void updateAvailableWorkers();
    void updateAvailableGoods();

private:
    bool isValidPosition(int x, int y) const;
    bool updateResidentialCell(int x, int y);
    bool updateIndustrialCell(int x, int y);
    bool updateCommercialCell(int x, int y);
};

#endif 