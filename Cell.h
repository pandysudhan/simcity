#ifndef CELL_H
#define CELL_H

enum CellType {
    EMPTY,
    RESIDENTIAL,
    INDUSTRIAL,
    COMMERCIAL,
    ROAD,
    POWERLINE,
    POWERLINE_ROAD,
    POWERPLANT
};

class Cell {
private:
    CellType type;
    int population;
    int pollution;
    bool hasPower;

public:
    Cell();
    Cell(CellType t);
    
    CellType getType() const;
    int getPopulation() const;
    int getPollution() const;
    bool isPowered() const;
    
    void setType(CellType t);
    void setPopulation(int pop);
    void setPollution(int pol);
    void setPower(bool power);
    
    char getDisplayChar() const;
};

#endif 