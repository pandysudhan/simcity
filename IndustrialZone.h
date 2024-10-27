#include "ZoneTemplate.h"
#include "CommonInterface.h"
#include "WorkerEntity.h"
#include "ProductEntity.h"

#pragma once
#ifndef INDUSTRIAL_ZONE_H
#define INDUSTRIAL_ZONE_H

class IndustrialZone : public ZoneTemplate<IndustrialZone> {
public:
    IndustrialZone(int coord_x, int coord_y, bool adjacent_to_powerline, CommonInterface* common_area)
        : ZoneTemplate(coord_x, coord_y, adjacent_to_powerline), shared_resources(common_area) {}

    void advance() {
        auto available_workers = shared_resources->fetchAvailableWorkers();
        if (population == 0 && has_powerline && available_workers.size() >= 2) {
            ++population;
            available_workers[0]->assign();
            available_workers[1]->assign();
            products.push_back(new ProductEntity());
        } else if (population == 0 && hasEnoughNeighbors(1, 1) && available_workers.size() >= 2) {
            ++population;
            available_workers[0]->assign();
            available_workers[1]->assign();
            products.push_back(new ProductEntity());
        } else if (population == 1 && hasEnoughNeighbors(2, 1) && available_workers.size() >= 2) {
            ++population;
            available_workers[0]->assign();
            available_workers[1]->assign();
            products.push_back(new ProductEntity());
        } else if (population == 2 && hasEnoughNeighbors(4, 2) && available_workers.size() >= 2) {
            ++population;
            available_workers[0]->assign();
            available_workers[1]->assign();
            products.push_back(new ProductEntity());
        }
    }

    std::vector<ProductEntity*> fetchSellableProducts() {
        std::vector<ProductEntity*> sellable_products;
        for (auto& product : products) {
            if (product->isSellable()) {
                sellable_products.push_back(product);
            }
        }
        return sellable_products;
    }

private:
    CommonInterface* shared_resources;
    std::vector<ProductEntity*> products;
};

#endif
