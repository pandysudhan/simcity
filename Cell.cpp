#include "Cell.h"

Cell::Cell() : type(EMPTY), population(0), pollution(0), hasPower(false) {}

CellType Cell::getType() const { return type; }
void Cell::setType(CellType t) { type = t; }

int Cell::getPopulation() const { return population; }
void Cell::setPopulation(int p) { population = p; }
void Cell::incrementPopulation() { population++; }

bool Cell::getPower() const { return hasPower; }
void Cell::setPower(bool p) { hasPower = p; }

int Cell::getPollution() const { return pollution; }
void Cell::setPollution(int p) { pollution = p; }
void Cell::addPollution(int p) { pollution += p; }

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