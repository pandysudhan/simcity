#include "Grid.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

Grid::Grid(int r, int c) : rows(r), cols(c), availableWorkers(0), availableGoods(0) {
    cells.resize(rows, std::vector<Cell>(cols));
}

bool Grid::updateCell(int x, int y) {
    Cell& cell = cells[x][y];
    
    switch (cell.getType()) {
        case RESIDENTIAL:
            return updateResidentialCell(x, y);
        case INDUSTRIAL:
            return updateIndustrialCell(x, y);
        case COMMERCIAL:
            return updateCommercialCell(x, y);
        default:
            return false;
    }
}

bool Grid::updateResidentialCell(int x, int y) {
    Cell& cell = cells[x][y];
    int adjPop = countAdjacentPopulation(x, y, cell.getPopulation());
    bool hasPower = hasAdjacentPowerline(x, y);
    
    if (ResidentialZone::canGrow(cell, adjPop, hasPower)) {
        ResidentialZone::grow(cell);
        return true;
    }
    return false;
}

bool Grid::updateIndustrialCell(int x, int y) {
    Cell& cell = cells[x][y];
    int adjPop = countAdjacentPopulation(x, y, cell.getPopulation());
    bool hasPower = hasAdjacentPowerline(x, y);
    
    if (IndustrialZone::canGrow(cell, adjPop, hasPower, availableWorkers)) {
        IndustrialZone::grow(cell, availableWorkers);
        return true;
    }
    return false;
}

bool Grid::updateCommercialCell(int x, int y) {
    Cell& cell = cells[x][y];
    int adjPop = countAdjacentPopulation(x, y, cell.getPopulation());
    bool hasPower = hasAdjacentPowerline(x, y);
    
    if (CommercialZone::canGrow(cell, adjPop, hasPower, availableWorkers, availableGoods)) {
        CommercialZone::grow(cell, availableWorkers, availableGoods);
        return true;
    }
    return false;
}

void Grid::loadFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    int row = 0;
    
    while (std::getline(file, line) && row < rows) {
        std::stringstream ss(line);
        std::string cell;
        int col = 0;
        
        while (std::getline(ss, cell, ',') && col < cols) {
            char type = cell[cell.find_first_not_of(" \t")];
            CellType cellType;
            
            switch (type) {
                case 'R': cellType = RESIDENTIAL; break;
                case 'I': cellType = INDUSTRIAL; break;
                case 'C': cellType = COMMERCIAL; break;
                case '-': cellType = ROAD; break;
                case 'T': cellType = POWERLINE; break;
                case '#': cellType = POWERLINE_ROAD; break;
                case 'P': cellType = POWERPLANT; break;
                default: cellType = EMPTY;
            }
            
            cells[row][col] = Cell(cellType);
            if (cellType == POWERPLANT || cellType == POWERLINE || cellType == POWERLINE_ROAD) {
                cells[row][col].setPower(true);
            }
            col++;
        }
        row++;
    }
}

void Grid::calculatePollution() {
    std::vector<std::vector<int>> newPollution(rows, std::vector<int>(cols, 0));
    
    // Calculate pollution from industrial zones
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (cells[i][j].getType() == INDUSTRIAL) {
                int pollution = IndustrialZone::getPollutionLevel(cells[i][j].getPopulation());
                
                // Spread pollution to adjacent cells
                for (int dx = -2; dx <= 2; dx++) {
                    for (int dy = -2; dy <= 2; dy++) {
                        int newX = i + dx;
                        int newY = j + dy;
                        if (isValidPosition(newX, newY)) {
                            int distance = std::max(std::abs(dx), std::abs(dy));
                            newPollution[newX][newY] += std::max(0, pollution - distance);
                        }
                    }
                }
            }
        }
    }
    
    // Update pollution levels
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cells[i][j].setPollution(newPollution[i][j]);
        }
    }
}

void Grid::updateAvailableWorkers() {
    availableWorkers = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (cells[i][j].getType() == RESIDENTIAL) {
                availableWorkers += ResidentialZone::getWorkerCount(cells[i][j].getPopulation());
            }
        }
    }
}

void Grid::updateAvailableGoods() {
    availableGoods = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (cells[i][j].getType() == INDUSTRIAL) {
                availableGoods += IndustrialZone::getGoodsProduced(cells[i][j].getPopulation());
            }
        }
    }
}

bool Grid::isValidPosition(int x, int y) const {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

int Grid::countAdjacentPopulation(int x, int y, int minPop) const {
    int count = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int newX = x + dx;
            int newY = y + dy;
            if (isValidPosition(newX, newY) && cells[newX][newY].getPopulation() >= minPop) {
                count++;
            }
        }
    }
    return count;
}

bool Grid::hasAdjacentPowerline(int x, int y) const {
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int newX = x + dx;
            int newY = y + dy;
            if (isValidPosition(newX, newY)) {
                CellType type = cells[newX][newY].getType();
                if (type == POWERLINE || type == POWERLINE_ROAD || type == POWERPLANT) {
                    return true;
                }
            }
        }
    }
    return false;
}

void Grid::displayGrid() const {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << cells[i][j].getDisplayChar() << " ";
        }
        std::cout << std::endl;
    }
}

void Grid::displayPollution() const {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << cells[i][j].getPollution() << " ";
        }
        std::cout << std::endl;
    }
}

int Grid::getRows() const { return rows; }
int Grid::getCols() const { return cols; }
Cell& Grid::getCell(int x, int y) { return cells[x][y]; }
const Cell& Grid::getCell(int x, int y) const { return cells[x][y]; }

int Grid::getTotalResidentialPopulation() const {
    int total = 0;
    for (const auto& row : cells) {
        for (const auto& cell : row) {
            if (cell.getType() == RESIDENTIAL) {
                total += cell.getPopulation();
            }
        }
    }
    return total;
}

int Grid::getTotalIndustrialPopulation() const {
    int total = 0;
    for (const auto& row : cells) {
        for (const auto& cell : row) {
            if (cell.getType() == INDUSTRIAL) {
                total += cell.getPopulation();
            }
        }
    }
    return total;
}

int Grid::getTotalCommercialPopulation() const {
    int total = 0;
    for (const auto& row : cells) {
        for (const auto& cell : row) {
            if (cell.getType() == COMMERCIAL) {
                total += cell.getPopulation();
            }
        }
    }
    return total;
}

int Grid::getTotalPollution() const {
    int total = 0;
    for (const auto& row : cells) {
        for (const auto& cell : row) {
            total += cell.getPollution();
        }
    }
    return total;
}

void Grid::getAreaStatistics(int x1, int y1, int x2, int y2, 
                           int& resPop, int& indPop, int& comPop, int& pollution) const {
    resPop = indPop = comPop = pollution = 0;
    
    for (int i = x1; i <= x2; i++) {
        for (int j = y1; j <= y2; j++) {
            if (!isValidPosition(i, j)) continue;
            
            const Cell& cell = cells[i][j];
            switch (cell.getType()) {
                case RESIDENTIAL:
                    resPop += cell.getPopulation();
                    break;
                case INDUSTRIAL:
                    indPop += cell.getPopulation();
                    break;
                case COMMERCIAL:
                    comPop += cell.getPopulation();
                    break;
            }
            pollution += cell.getPollution();
        }
    }
} 