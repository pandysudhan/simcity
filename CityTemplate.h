#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "ProductEntity.h"
#include "WorkerEntity.h"
#include "CommonInterface.h"
#include "CommercialZone.h"
#include "IndustrialZone.h"
#include "ResidentialZone.h"

#pragma once
#ifndef CITY_TEMPLATE_H
#define CITY_TEMPLATE_H

class CityTemplate : protected CommonInterface {
public:
    CityTemplate(std::string filename);
    ~CityTemplate();

    int getRefreshRate() const { return refresh_rate; }
    int getTimeLimit() const { return time_limit; }

    void show();
    void showPollution();
    std::vector<WorkerEntity*> fetchAvailableWorkers();
    std::vector<ProductEntity*> fetchSellableProducts();

    bool runSimulationStep();

private:
    std::vector<CommercialZone*> commercial_zones;
    std::vector<ResidentialZone*> residential_zones;
    std::vector<IndustrialZone*> industrial_zones;

    int length, width;
    static const int MAX_DIMENSION = 50;
    char layout[MAX_DIMENSION][MAX_DIMENSION];
    int pollution[MAX_DIMENSION][MAX_DIMENSION];

    int refresh_rate, time_limit;
    std::string region_name;

    void initializeZones();
    void prepareLayout();
    void initPollution();
    void setAdjacencies();
    void spreadPollution();
    void applyPollutionLevel(int, int, int);
    bool isAdjacentToPowerLine(int, int);
    bool hasLayoutChanged(char[MAX_DIMENSION][MAX_DIMENSION]);
};

#endif
