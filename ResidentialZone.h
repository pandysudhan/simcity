#include "ZoneTemplate.h"
#include "WorkerEntity.h"

#pragma once
#ifndef RESIDENTIAL_ZONE_H
#define RESIDENTIAL_ZONE_H

class ResidentialZone : public ZoneTemplate<ResidentialZone> {
public:
    ResidentialZone(int coord_x, int coord_y, bool adjacent_to_powerline)
        : ZoneTemplate(coord_x, coord_y, adjacent_to_powerline) {}

    std::vector<WorkerEntity*> getAvailableWorkers() {
        std::vector<WorkerEntity*> available_workers;
        for (auto& worker : workers) {
            if (worker->isAvailable()) {
                available_workers.push_back(worker);
            }
        }
        return available_workers;
    }

    void advance() {
        if (population == 0 && has_powerline) {
            ++population;
            workers.push_back(new WorkerEntity());
        } else if (population == 0 && hasEnoughNeighbors(1, 1)) {
            ++population;
            workers.push_back(new WorkerEntity());
        } else if (population == 1 && hasEnoughNeighbors(2, 1)) {
            ++population;
            workers.push_back(new WorkerEntity());
        } else if (population == 2 && hasEnoughNeighbors(4, 2)) {
            ++population;
            workers.push_back(new WorkerEntity());
        } else if (population == 3 && hasEnoughNeighbors(6, 3)) {
            ++population;
            workers.push_back(new WorkerEntity());
        } else if (population == 4 && hasEnoughNeighbors(8, 4)) {
            ++population;
            workers.push_back(new WorkerEntity());
        }
    }

private:
    std::vector<WorkerEntity*> workers;
};

#endif
