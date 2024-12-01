#ifndef ROADNETWORK_H
#define ROADNETWORK_H

#include "Road.h"
#include <vector>
#include <map>
#include <set>
#include <queue>

class RoadNetwork {
private:
    using Node = std::pair<int, int>;
    std::vector<std::vector<Road*>> roadGrid;
    int width;
    int height;
    
    // Helper methods for pathfinding
    bool isValidPosition(int x, int y) const;
    std::vector<Node> getNeighbors(int x, int y) const;
    double calculateCongestionWeight(const Road* road) const;

public:
    RoadNetwork(int width, int height);
    ~RoadNetwork();
    
    // Road management
    void addRoad(int x, int y, RoadType type);
    void removeRoad(int x, int y);
    Road* getRoad(int x, int y) const;
    
    // Path finding and connectivity
    std::vector<Node> findBestPath(Node start, Node end) const;
    bool checkConnectivity() const;
    bool areZonesConnected(Node zone1, Node zone2) const;
    
    // Traffic management
    void updateTraffic();
    std::vector<Node> getCongestedRoads() const;
};

#endif // ROADNETWORK_H
