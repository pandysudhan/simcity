#include <vector>

#pragma once
#ifndef ZONE_TEMPLATE_H
#define ZONE_TEMPLATE_H

template <typename ZoneType>
class ZoneTemplate {
public:
    ZoneTemplate(int coord_x, int coord_y, bool adjacent_to_powerline)
        : x(coord_x), y(coord_y), population(0), has_powerline(adjacent_to_powerline) {}

    void addNeighbor(ZoneType* neighbor_zone) {
        neighbors.push_back(neighbor_zone);
    }

    int getX() const { return x; }
    int getY() const { return y; }
    int getPopulation() const { return population; }

protected:
    int x, y;
    std::vector<ZoneType*> neighbors;
    int population;
    bool has_powerline;

    bool hasEnoughNeighbors(int required_count, int population_threshold) {
        int match_count = 0;
        for (auto& neighbor : neighbors) {
            if (neighbor->getPopulation() >= population_threshold) {
                ++match_count;
            }
        }
        return match_count >= required_count;
    }
};

#endif
