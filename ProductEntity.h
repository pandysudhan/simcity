#pragma once
#ifndef PRODUCT_ENTITY_H
#define PRODUCT_ENTITY_H

class ProductEntity {
private:
    bool sellable;
public:
    ProductEntity() : sellable(true) {}

    bool markSold() {
        if (!sellable) return false;
        sellable = false;
        return true;
    }

    bool isSellable() const { return sellable; }
};

#endif
