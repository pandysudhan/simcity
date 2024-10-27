#include "CityTemplate.h"

CityTemplate::CityTemplate(std::string filename) {
    for (int i = 0; i < MAX_DIMENSION; i++) {
        for (int j = 0; j < MAX_DIMENSION; j++) {
            layout[i][j] = ' ';
        }
    }

    std::ifstream config_file(filename);
    if (!config_file) {
        exit(0);
    }

    config_file.seekg(14);
    getline(config_file, region_name);
    config_file.seekg(37);
    config_file >> time_limit;
    config_file.seekg(53);
    config_file >> refresh_rate;

    config_file.close();
    config_file.open(region_name);
    if (!config_file) {
        exit(0);
    }

    int row = 0, col = 0;
    while (config_file) {
        char ch;
        config_file >> ch;
        if (ch == '\n') {
            row++;
            col = 0;
        } else {
            layout[row][col++] = ch;
        }
    }

    length = col;
    width = row + 1;

    initializeZones();
    setAdjacencies();
    initPollution();
}

CityTemplate::~CityTemplate() {}

void CityTemplate::show() {
    // for (int i = 0; i < 2 * length + 1; i++) std::cout << "-";
    // std::cout << "\n";
    // for (int i = 0; i < width; i++) {
    //     std::cout << "|";
    //     for (int j = 0; j < length; j++) {
    //         std::cout << (j > 0 ? " " : "") << layout[i][j];
    //     }
    //     std::cout << "|\n";
    // }
    // for (int i = 0; i < 2 * length + 1; i++) std::cout << "-";
    // std::cout << "\n";

        int i, j;
    for (i = 0; i < 2 * length + 1; i++)std::cout << "-";
    std::cout << "\n";
    for (i = 0; i < width; i++) {
        std::cout << "|";
        for (j = 0; j < length; j++) {
            if (j > 0)std::cout << " ";
            std::cout << layout[i][j];
        }
        std::cout << "|";
        std::cout << "\n";
    }
    for (i = 0; i < 2 * length + 1; i++)std::cout << "-";
    std::cout << "\n";
}

void CityTemplate::showPollution() {
    int i, j;
    for (i = 0; i < 2 * length + 1; i++)std::cout << "-";
    std::cout << "\n";
    for (i = 0; i < width; i++) {
        std::cout << "|";
        for (j = 0; j < length; j++) {
            if (j > 0)std::cout << " ";
            std::cout << pollution[i][j];
        }
        std::cout << "|";
        std::cout << "\n";
    }
    for (i = 0; i < 2 * length + 1; i++)std::cout << "-";
    std::cout << "\n";
}

void CityTemplate::initializeZones() {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < length; j++) {
            switch (layout[i][j]) {
                case 'C':
                    commercial_zones.push_back(new CommercialZone(i, j, isAdjacentToPowerLine(i, j), this));
                    break;
                case 'R':
                    residential_zones.push_back(new ResidentialZone(i, j, isAdjacentToPowerLine(i, j)));
                    break;
                case 'I':
                    industrial_zones.push_back(new IndustrialZone(i, j, isAdjacentToPowerLine(i, j), this));
                    break;
            }
        }
    }
}

void CityTemplate::prepareLayout() {
    for (auto zone : commercial_zones) {
        if (zone->getPopulation() > 0) layout[zone->getX()][zone->getY()] = '0' + zone->getPopulation();
    }
    for (auto zone : residential_zones) {
        if (zone->getPopulation() > 0) layout[zone->getX()][zone->getY()] = '0' + zone->getPopulation();
    }
    for (auto zone : industrial_zones) {
        if (zone->getPopulation() > 0) layout[zone->getX()][zone->getY()] = '0' + zone->getPopulation();
    }
}

void CityTemplate::initPollution() {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < length; j++) {
            pollution[i][j] = 0;
        }
    }
}

void CityTemplate::setAdjacencies() {
    for (auto zone1 = commercial_zones.begin(); zone1 != commercial_zones.end(); ++zone1) {
        for (auto zone2 = zone1; zone2 != commercial_zones.end(); ++zone2) {
            if (std::abs((*zone1)->getX() - (*zone2)->getX()) <= 1 && std::abs((*zone1)->getY() - (*zone2)->getY()) <= 1) {
                (*zone1)->addNeighbor(*zone2);
                (*zone2)->addNeighbor(*zone1);
            }
        }
    }
}

bool CityTemplate::isAdjacentToPowerLine(int x, int y) {
    for (int i = x - 1; i <= x + 1; ++i) {
        for (int j = y - 1; j <= y + 1; ++j) {
            if (layout[i][j] == '#' || layout[i][j] == 'T') return true;
        }
    }
    return false;
}

bool CityTemplate::hasLayoutChanged(char previous[MAX_DIMENSION][MAX_DIMENSION]) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < length; j++) {
            if (layout[i][j] != previous[i][j]) return true;
        }
    }
    return false;
}

std::vector<WorkerEntity*> CityTemplate::fetchAvailableWorkers() {
    std::vector<WorkerEntity*> available_workers;
    for (auto& zone : residential_zones) {
        auto workers = zone->getAvailableWorkers();
        available_workers.insert(available_workers.end(), workers.begin(), workers.end());
    }
    return available_workers;
}

std::vector<ProductEntity*> CityTemplate::fetchSellableProducts() {
    std::vector<ProductEntity*> sellable_products;
    for (auto& zone : industrial_zones) {
        auto products = zone->fetchSellableProducts();
        sellable_products.insert(sellable_products.end(), products.begin(), products.end());
    }
    return sellable_products;
}

bool CityTemplate::runSimulationStep() {
    char previous_layout[MAX_DIMENSION][MAX_DIMENSION];
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < length; j++) {
            previous_layout[i][j] = layout[i][j];
        }
    }

    for (auto& zone : residential_zones) zone->advance();
    for (auto& zone : commercial_zones) zone->advance();
    for (auto& zone : industrial_zones) zone->advance();

    spreadPollution();
    prepareLayout();

    return hasLayoutChanged(previous_layout);
}

void CityTemplate::spreadPollution() {
    for (auto& zone : industrial_zones) {
        applyPollutionLevel(zone->getPopulation(), zone->getX(), zone->getY());
    }
}

void CityTemplate::applyPollutionLevel(int level, int x, int y) {
    if (level == 0) return;
    for (int i = x - 1; i <= x + 1; ++i) {
        for (int j = y - 1; j <= y + 1; ++j) {
            if (i == x && j == y && pollution[i][j] < level) {
                pollution[i][j] = level;
            } else {
                applyPollutionLevel(level - 1, i, j);
            }
        }
    }
}
