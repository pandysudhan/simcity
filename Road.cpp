#include "Road.h"
#include <algorithm>

Road::Road(RoadType type, std::pair<int, int> pos) : type(type), position(pos), currentTraffic(0), isCongested(false) {
    capacity = (type == RoadType::HIGHWAY) ? HIGHWAY_CAPACITY : TWO_LANE_CAPACITY;
}

void Road::addTraffic(int amount) {
    currentTraffic = std::min(currentTraffic + amount, capacity);
    updateCongestionStatus();
}

void Road::removeTraffic(int amount) {
    currentTraffic = std::max(currentTraffic - amount, 0);
    updateCongestionStatus();
}

void Road::updateCongestionStatus() {
    // A road is considered congested if traffic exceeds 80% of its capacity
    isCongested = (currentTraffic > capacity * 0.8);
}
