#ifndef ROAD_H
#define ROAD_H

#include <vector>

enum class RoadType {
    TWO_LANE,
    HIGHWAY
};

class Road {
private:
    RoadType type;
    int capacity;
    int currentTraffic;
    bool isCongested;
    std::pair<int, int> position; // x, y coordinates

public:
    Road(RoadType type, std::pair<int, int> pos);
    
    // Getters
    RoadType getType() const { return type; }
    int getCapacity() const { return capacity; }
    int getCurrentTraffic() const { return currentTraffic; }
    bool getIsCongested() const { return isCongested; }
    std::pair<int, int> getPosition() const { return position; }
    
    // Traffic management
    void addTraffic(int amount);
    void removeTraffic(int amount);
    void updateCongestionStatus();
    
    // Static capacity values
    static const int TWO_LANE_CAPACITY = 100;
    static const int HIGHWAY_CAPACITY = 300;
};

#endif // ROAD_H
