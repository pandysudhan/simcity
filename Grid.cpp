#include "Grid.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

Grid::Grid(int r, int c) : rows(r), cols(c), availableWorkers(0), availableGoods(0), roadNetwork(r, c) {
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
    bool hasRoad = hasRoadAccess(x, y);
    
    if (ResidentialZone::canGrow(cell, adjPop, hasPower) && hasRoad) {
        ResidentialZone::grow(cell);
        return true;
    }
    return false;
}

bool Grid::updateIndustrialCell(int x, int y) {
    Cell& cell = cells[x][y];
    int adjPop = countAdjacentPopulation(x, y, cell.getPopulation());
    bool hasPower = hasAdjacentPowerline(x, y);
    bool hasRoad = hasRoadAccess(x, y);
    
    if (IndustrialZone::canGrow(cell, adjPop, hasPower, availableWorkers) && hasRoad) {
        IndustrialZone::grow(cell, availableWorkers);
        return true;
    }
    return false;
}

bool Grid::updateCommercialCell(int x, int y) {
    Cell& cell = cells[x][y];
    int adjPop = countAdjacentPopulation(x, y, cell.getPopulation());
    bool hasPower = hasAdjacentPowerline(x, y);
    bool hasRoad = hasRoadAccess(x, y);
    
    if (CommercialZone::canGrow(cell, adjPop, hasPower, availableWorkers, availableGoods) && hasRoad) {
        CommercialZone::grow(cell, availableWorkers, availableGoods);
        return true;
    }
    return false;
}

void Grid::loadFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::string line;
    int row = 0;
    
    while (std::getline(file, line) && row < rows) {
        std::stringstream ss(line);
        std::string cellStr;
        int col = 0;
        
        while (std::getline(ss, cellStr, ',') && col < cols) {
            // Remove whitespace
            cellStr.erase(std::remove_if(cellStr.begin(), cellStr.end(), ::isspace), cellStr.end());
            
            if (cellStr.empty()) {
                cells[row][col].setType(EMPTY);
                col++;
                continue;
            }
            
            char type = cellStr[0];
            Cell& currentCell = cells[row][col];
            
            switch (type) {
                case 'R':
                    currentCell.setType(RESIDENTIAL);
                    break;
                case 'I':
                    currentCell.setType(INDUSTRIAL);
                    break;
                case 'C':
                    currentCell.setType(COMMERCIAL);
                    break;
                case 'P':
                    currentCell.setType(POWERPLANT);
                    currentCell.setPower(true);
                    break;
                case 'T':
                    currentCell.setType(POWERLINE);
                    currentCell.setPower(true);
                    break;
                case '#':
                    currentCell.setType(ROAD);
                    roadNetwork.addRoad(row, col, RoadType::HIGHWAY);
                    break;
                case '-':
                    currentCell.setType(ROAD);
                    roadNetwork.addRoad(row, col, RoadType::TWO_LANE);
                    break;
                default:
                    currentCell.setType(EMPTY);
                    break;
            }
            col++;
        }
        row++;
    }
    
    // After loading, check road network connectivity
    if (!checkRoadConnectivity()) {
        std::cout << "Warning: Not all roads are connected!\n";
    }
}

void Grid::simulate() {
    updateAvailableWorkers();
    updateAvailableGoods();
    updateTraffic();
    
    // Randomly update cells
    std::vector<std::pair<int, int>> positions;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (cells[i][j].getType() != EMPTY) {
                positions.push_back({i, j});
            }
        }
    }
    std::random_shuffle(positions.begin(), positions.end());
    
    for (const auto& pos : positions) {
        updateCell(pos.first, pos.second);
    }
    
    calculatePollution();
}

void Grid::displayGrid() const {
    std::cout << "\nCurrent Grid State:\n";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            const Cell& cell = cells[i][j];
            if (cell.getType() == ROAD) {
                Road* road = roadNetwork.getRoad(i, j);
                if (road && road->getType() == RoadType::HIGHWAY) {
                    std::cout << "# ";
                } else {
                    std::cout << "- ";
                }
            } else {
                char display = ' ';
                switch (cell.getType()) {
                    case RESIDENTIAL: display = 'R'; break;
                    case INDUSTRIAL: display = 'I'; break;
                    case COMMERCIAL: display = 'C'; break;
                    case POWERPLANT: display = 'P'; break;
                    case POWERLINE: display = 'T'; break;
                    default: display = ' ';
                }
                std::cout << display << " ";
            }
        }
        std::cout << "\n";
    }
    
    // Display road connectivity status
    std::cout << "\nRoad Network Status: " 
              << (checkRoadConnectivity() ? "Connected" : "Disconnected") << "\n";
              
    // Display traffic information
    displayTraffic();
}

void Grid::displayTraffic() const {
    auto congestedRoads = getCongestedRoads();
    if (!congestedRoads.empty()) {
        std::cout << "\nTraffic Status:\n";
        std::cout << "Congested roads at locations:\n";
        for (const auto& pos : congestedRoads) {
            std::cout << "(" << pos.first << "," << pos.second << ") ";
        }
        std::cout << "\n";
    } else {
        std::cout << "\nNo road congestion detected.\n";
    }
}

bool Grid::hasRoadAccess(int x, int y) const {
    if (!isValidPosition(x, y)) return false;
    
    // Check adjacent cells for roads
    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};
    
    for (int i = 0; i < 4; ++i) {
        int newX = x + dx[i];
        int newY = y + dy[i];
        if (isValidPosition(newX, newY) && cells[newX][newY].getType() == ROAD) {
            return true;
        }
    }
    return false;
}

void Grid::updateTraffic() {
    roadNetwork.updateTraffic();
}

// Road network related methods
bool Grid::addRoad(int x, int y, RoadType type) {
    if (!isValidPosition(x, y)) return false;
    if (cells[x][y].getType() != EMPTY) return false;
    
    roadNetwork.addRoad(x, y, type);
    cells[x][y].setType(ROAD);
    return true;
}

bool Grid::removeRoad(int x, int y) {
    if (!isValidPosition(x, y)) return false;
    if (cells[x][y].getType() != ROAD) return false;
    
    roadNetwork.removeRoad(x, y);
    cells[x][y].setType(EMPTY);
    return true;
}

bool Grid::checkRoadConnectivity() const {
    return roadNetwork.checkConnectivity();
}

std::vector<std::pair<int, int>> Grid::findBestRoute(
    std::pair<int, int> start,
    std::pair<int, int> end
) const {
    return roadNetwork.findBestPath(start, end);
}

std::vector<std::pair<int, int>> Grid::getCongestedRoads() const {
    return roadNetwork.getCongestedRoads();
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

void Grid::displayPollution() const {
    std::cout << "\nPollution levels:\n";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << cells[i][j].getPollution() << " ";
        }
        std::cout << "\n";
    }
} 