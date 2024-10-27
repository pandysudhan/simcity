#include <iostream>
#include <fstream>
#include "City.h"

using namespace std;

int main()
{
    string config_filename;
    bool isdiff;
    cout << "Beginning simulation" << endl;
    cout << "Please enter the name of the configuration file: ";
    cin >> config_filename;

    City city(config_filename);
    
    cout << "Initial Region State" << endl;
    city.display();

    for (int s = 1; s <= city.time_limit; s++) {
        
        isdiff = city.ProcessStep(s);

        if (!isdiff)break;
        cout << endl;
    }
    cout << "Final Region State" << endl;
    city.display();

    cout << "Contamination" << endl;
    city.displayContamination();

    cout << "The total inhabitantss for the region are :" << endl;
    cout << "Residential: " << city.getRPopulation(0, 0, city.getWidth() - 1, city.getLength() - 1) << endl;
    cout << "Industrial: " << city.getIPopulation(0, 0, city.getWidth() - 1, city.getLength() - 1) << endl;
    cout << "Commercial: " << city.getCPopulation(0, 0, city.getWidth() - 1, city.getLength() - 1) << endl;
        
    cout << "The total amount of pollution in the region is " 
        << city.getTotalContamination(0, 0, city.getWidth() - 1, city.getLength() - 1) 
        << " units." << endl;

    cout << "Please enter the diagonal corners of the area you wish to have more information about. (";
    cout<<"MinX = 0, MinY = 0, MaxX = "<<city.getLength() - 1<<", MaxY = "<<city.getWidth() - 1<<")"<<endl;

    int x1, y1, x2, y2;
    cout << "X1: "; cin >> x1;
    cout << "Y1: "; cin >> y1;
    cout << "X2: "; cin >> x2;
    cout << "Y2: "; cin >> y2;

    cout << "The total inhabitantss for the requested area are: " << endl;
    cout << "Residential: " << city.getRPopulation(x1, y1, x2, y2) << endl;
    cout << "Industrial: " << city.getIPopulation(x1, y1, x2, y2) << endl;
    cout << "Commercial: " << city.getCPopulation(x1, y1, x2, y2) << endl;

    cout << "The total amount of pollution for the requested area is "
        << city.getTotalContamination(x1, y1, x2, y2) << " units." << endl;

    cout << "Simulation Complete" << endl;
    return 0;
}

