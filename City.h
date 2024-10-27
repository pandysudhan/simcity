#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <cmath>
#include <queue>

#include "Good.h"
#include "Worker.h"
#include "Common.h"
#include "CZone.h"
#include "IZone.h"
#include "RZone.h"
using namespace std;

#pragma once

class City : protected SharedResources
{
public:

    City(string filename);
    ~City();

    int refresh_rate, time_limit;
    char* regionName;

    int getWidth() {return this->width;}
    int getLength() { return this->length; }
    void display();
    void displayContamination();
    vector<Laborer*> getAvailLaborers();
    vector<Product*> getSoldableProducts();

    int getRPopulation(int, int, int, int);
    int getCPopulation(int, int, int, int);
    int getIPopulation(int, int, int, int);
    
    int getTotalContamination(int, int, int, int);

    bool ProcessStep(int);

private:
    vector<CZone*> czones;
    vector<RZone*> rzones;
    vector<IZone*> izones;

    int length, width;
    const static int MAX_DIM = 50;
    char layout[MAX_DIM][MAX_DIM];
    int pollution[MAX_DIM][MAX_DIM];

    template<class Z>void PriorityActivateation(vector<Z*>);
    template<class Z>void PriorityActivateation2(vector<Z*>);
    template<class Z>void PriorityActivateation3(vector<Z*>);
    template<class Z>void PriorityActivateation4(vector<Z*>);

    void InitializeZones();
    void PrepareLayout();
    void InitContamination();
    void CreateAdjacency();
    void DisperseContamination();
    void DisperseContaminationNode(int, int, int);
    bool adjacentToPowerGrid(int, int);
    bool haschanged(char[MAX_DIM][MAX_DIM]);

};

