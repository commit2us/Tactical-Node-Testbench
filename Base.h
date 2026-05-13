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
    string name;
    Location location; // Dos atributos
    float risk;


public:
    // Constructores y Destructor
    Base();
    Base(string i, int lx, int ly, string name, float r);
    ~Base();

    // Setters
    void setId(string i);
    void setLocation(int lx, int ly);
    void setName(string name);
    void setRisk(float r);

    // Getters
    string getId();
    Location getLocation();
    string getName();
    float getRisk();
};

#endif