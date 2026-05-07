#ifndef BASE_H
#define BASE_H

#include <string>
#include <iostream>

// Estructura para la ubicación
struct Location {
    int x;
    int y;

    Location(int x, int y) : x(x), y(y) {}
};

class Base {
private:
    // Atributos
    std::string id;
    Location location;
    std::string frequency;
    float risk;
    std::string bandWidth;

public:
    // Constructores y Destructor
    Base();
    Base(std::string i, int lx, int ly, std::string freq, float r, std::string bW);
    ~Base();

    // Setters
    void setId(std::string i);
    void setLocation(int lx, int ly);
    void setFrequency(std::string freq);
    void setRisk(float r);
    void setBandWidth(std::string bW);

    // Getters
    std::string getId();
    Location getLocation();
    std::string getFrequency();
    float getRisk();
    std::string getBandWidth();

    bool checkBaseLocatio(Location matrixLimits);
};

#endif