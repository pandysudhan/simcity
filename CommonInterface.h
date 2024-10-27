#pragma once
#ifndef COMMON_INTERFACE_H
#define COMMON_INTERFACE_H

#include <vector>
#include "WorkerEntity.h"
#include "ProductEntity.h"

class CommonInterface {
public:
    virtual std::vector<WorkerEntity*> fetchAvailableWorkers() = 0;
    virtual std::vector<ProductEntity*> fetchSellableProducts() = 0;
};

#endif
