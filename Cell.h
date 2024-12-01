#ifndef CELL_H
#define CELL_H

enum CellType {
    EMPTY,
    RESIDENTIAL,
    INDUSTRIAL,
    COMMERCIAL,
    POWERPLANT,
    POWERLINE,
    POWERLINE_ROAD,
    ROAD
};

class Cell {
private:
    CellType type;
    int population;
    bool hasPower;
    int pollution;

public:
    Cell();
    
    CellType getType() const;
    void setType(CellType t);
    
    int getPopulation() const;
    void setPopulation(int p);
    void incrementPopulation();
    
    bool getPower() const;
    void setPower(bool p);
    
    int getPollution() const;
    void setPollution(int p);
    void addPollution(int p);
    
    char getDisplayChar() const;
};

#endif 