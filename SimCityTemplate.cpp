#include <iostream>
#include <fstream>
#include "CityTemplate.h"

using namespace std;

int main() {
    string config_file;
    bool has_changes;
    cout << "Simulation starting" << endl;
    cout << "Enter configuration filename: ";
    cin >> config_file;

    CityTemplate city(config_file);
    cout << "Initial Layout" << endl;
    city.show();

    for (int step = 1; step <= city.getTimeLimit(); ++step) {
        has_changes = city.runSimulationStep();
        cout << "Step: " << step << endl;
        cout << "Workers Available: " << city.fetchAvailableWorkers().size();
        cout << " Goods Available: " << city.fetchSellableProducts().size() << endl;

        if (step % city.getRefreshRate() == 0) {
            city.show();
        }

        if (!has_changes) break;
    }
    cout << "Final Layout" << endl;
    city.show();

    cout << "Pollution Levels" << endl;
    city.showPollution();

    return 0;
}
