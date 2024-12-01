#include "RoadNetwork.h"
#include <queue>
#include <unordered_map>
#include <cmath>
#include <random>

RoadNetwork::RoadNetwork(int width, int height) : width(width), height(height) {
    roadGrid.resize(height, std::vector<Road*>(width, nullptr));
}

RoadNetwork::~RoadNetwork() {
    for (auto& row : roadGrid) {
        for (auto& road : row) {
            delete road;
        }
    }
}

void RoadNetwork::addRoad(int x, int y, RoadType type) {
    if (isValidPosition(x, y)) {
        if (roadGrid[x][y]) delete roadGrid[x][y];
        roadGrid[x][y] = new Road(type, std::make_pair(x, y));
    }
}

void RoadNetwork::removeRoad(int x, int y) {
    if (isValidPosition(x, y)) {
        delete roadGrid[x][y];
        roadGrid[x][y] = nullptr;
    }
}

Road* RoadNetwork::getRoad(int x, int y) const {
    if (isValidPosition(x, y)) {
        return roadGrid[x][y];
    }
    return nullptr;
}

bool RoadNetwork::isValidPosition(int x, int y) const {
    return x >= 0 && x < height && y >= 0 && y < width;
}

std::vector<std::pair<int, int>> RoadNetwork::getNeighbors(int x, int y) const {
    std::vector<std::pair<int, int>> neighbors;
    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};
    
    for (int i = 0; i < 4; ++i) {
        int newX = x + dx[i];
        int newY = y + dy[i];
        if (isValidPosition(newX, newY) && roadGrid[newX][newY]) {
            neighbors.push_back({newX, newY});
        }
    }
    return neighbors;
}

double RoadNetwork::calculateCongestionWeight(const Road* road) const {
    if (!road) return INFINITY;
    
    // Weight increases exponentially with congestion
    double congestionRatio = static_cast<double>(road->getCurrentTraffic()) / road->getCapacity();
    return 1.0 + std::exp(5 * (congestionRatio - 0.8)); // Higher weight when congestion > 80%
}

std::vector<std::pair<int, int>> RoadNetwork::findBestPath(
    std::pair<int, int> start,
    std::pair<int, int> end
) const {
    using Node = std::pair<int, int>;
    auto compare = [](const std::pair<double, Node>& a, const std::pair<double, Node>& b) {
        return a.first > b.first;
    };
    
    std::priority_queue<std::pair<double, Node>,
                       std::vector<std::pair<double, Node>>,
                       decltype(compare)> pq(compare);
    
    std::unordered_map<int, Node> cameFrom;
    std::unordered_map<int, double> costSoFar;
    
    pq.push({0, start});
    cameFrom[start.first * height + start.second] = start;
    costSoFar[start.first * height + start.second] = 0;
    
    while (!pq.empty()) {
        Node current = pq.top().second;
        pq.pop();
        
        if (current == end) break;
        
        for (const auto& next : getNeighbors(current.first, current.second)) {
            Road* nextRoad = roadGrid[next.first][next.second];
            double newCost = costSoFar[current.first * height + current.second] +
                           calculateCongestionWeight(nextRoad);
            
            int nextKey = next.first * height + next.second;
            if (!costSoFar.count(nextKey) || newCost < costSoFar[nextKey]) {
                costSoFar[nextKey] = newCost;
                double priority = newCost + std::hypot(end.first - next.first,
                                                   end.second - next.second);
                pq.push({priority, next});
                cameFrom[nextKey] = current;
            }
        }
    }
    
    // Reconstruct path
    std::vector<Node> path;
    Node current = end;
    while (current != start) {
        path.push_back(current);
        int key = current.first * height + current.second;
        if (!cameFrom.count(key)) return {}; // No path found
        current = cameFrom[key];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

bool RoadNetwork::checkConnectivity() const {
    if (roadGrid.empty()) return true;
    
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    std::queue<std::pair<int, int>> q;
    
    // Find first road
    bool foundFirst = false;
    for (int y = 0; y < height && !foundFirst; ++y) {
        for (int x = 0; x < width && !foundFirst; ++x) {
            if (roadGrid[y][x]) {
                q.push({y, x});
                visited[y][x] = true;
                foundFirst = true;
            }
        }
    }
    
    // BFS to check connectivity
    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        
        for (const auto& next : getNeighbors(current.first, current.second)) {
            if (!visited[next.first][next.second]) {
                visited[next.first][next.second] = true;
                q.push(next);
            }
        }
    }
    
    // Check if all roads are visited
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (roadGrid[y][x] && !visited[y][x]) {
                return false;
            }
        }
    }
    return true;
}

bool RoadNetwork::areZonesConnected(
    std::pair<int, int> zone1,
    std::pair<int, int> zone2
) const {
    return !findBestPath(zone1, zone2).empty();
}

void RoadNetwork::updateTraffic() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-10, 10);
    
    for (auto& row : roadGrid) {
        for (auto& road : row) {
            if (road) {
                int trafficChange = dis(gen);
                if (trafficChange > 0) {
                    road->addTraffic(trafficChange);
                } else {
                    road->removeTraffic(-trafficChange);
                }
            }
        }
    }
}

std::vector<std::pair<int, int>> RoadNetwork::getCongestedRoads() const {
    std::vector<std::pair<int, int>> congestedRoads;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (roadGrid[y][x] && roadGrid[y][x]->getIsCongested()) {
                congestedRoads.push_back({y, x});
            }
        }
    }
    return congestedRoads;
}
