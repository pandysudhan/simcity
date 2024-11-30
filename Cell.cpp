#include "Cell.h"

Cell::Cell() : type(EMPTY), population(0), pollution(0), hasPower(false) {}

Cell::Cell(CellType t) : type(t), population(0), pollution(0), hasPower(false) {}

CellType Cell::getType() const { return type; }
int Cell::getPopulation() const { return population; }
int Cell::getPollution() const { return pollution; }
bool Cell::isPowered() const { return hasPower; }

void Cell::setType(CellType t) { type = t; }
void Cell::setPopulation(int pop) { population = pop; }
void Cell::setPollution(int pol) { pollution = pol; }
void Cell::setPower(bool power) { hasPower = power; }

char Cell::getDisplayChar() const {
    if (population > 0) {
        return '0' + population;
    }
    
    switch (type) {
        case RESIDENTIAL: return 'R';
        case INDUSTRIAL: return 'I';
        case COMMERCIAL: return 'C';
        case ROAD: return '-';
        case POWERLINE: return 'T';
        case POWERLINE_ROAD: return '#';
        case POWERPLANT: return 'P';
        default: return ' ';
    }
} 