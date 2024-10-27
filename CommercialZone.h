#include "ZoneTemplate.h"
#include "CommonInterface.h"
#include "ProductEntity.h"
#include "WorkerEntity.h"

#pragma once
#ifndef COMMERCIAL_ZONE_H
#define COMMERCIAL_ZONE_H

class CommercialZone : public ZoneTemplate<CommercialZone> {
public:
    CommercialZone(int coord_x, int coord_y, bool adjacent_to_powerline, CommonInterface* common_area)
        : ZoneTemplate(coord_x, coord_y, adjacent_to_powerline), shared_resources(common_area) {}

    void advance() {
        auto products = shared_resources->fetchSellableProducts();
        auto workers = shared_resources->fetchAvailableWorkers();
        if (population == 0 && has_powerline && workers.size() >= 1 && products.size() >= 1) {
            ++population;
            workers[0]->assign();
            products[0]->markSold();
        } else if (population == 0 && hasEnoughNeighbors(1, 1) && workers.size() >= 1 && products.size() >= 1) {
            ++population;
            workers[0]->assign();
            products[0]->markSold();
        } else if (population == 1 && hasEnoughNeighbors(2, 1) && workers.size() >= 1 && products.size() >= 1) {
            ++population;
            workers[0]->assign();
            products[0]->markSold();
        }
    }

private:
    CommonInterface* shared_resources;
};

#endif
