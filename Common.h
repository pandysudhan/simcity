#pragma once
class SharedResources
{
public:
    virtual std::vector <Laborer*> getAvailLaborers() = 0;
    virtual std::vector <Product*> getSoldableProducts() = 0;
};

