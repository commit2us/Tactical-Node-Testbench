#ifndef BASE_H
#define BASE_H

#include <string>
#include <iostream>

using namespace std;

// Estructura para la ubicación
struct Location {
    int x;
    int y;
};

class Base {
private:
    // Atributos
    string id;
    Location location;
    string frequency;
    float risk;
    string bandWidth;

public:
    // Constructores y Destructor
    Base();
    Base(string i, int lx, int ly, string freq, float r, string bW);
    ~Base();

    // Setters
    void setId(string i);
    void setLocation(int lx, int ly);
    void setFrequency(string freq);
    void setRisk(float r);
    void setBandWidth(string bW);

    // Getters
    string getId();
    Location getLocation();
    string getFrequency();
    float getRisk();
    string getBandWidth();
};

#endif