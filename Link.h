#ifndef LINK_H
#define LINK_H

#include <cmath>
// #include "base.h" // Node file containing Base class 

using namespace std;

// Placeholders for Base entities

struct Location{int x; int y;};
struct Base{ Location location; };

class Link {
    Base* bases[2];
    float distance;

public:
    Link(Base* from, Base* to);
    ~Link();

private:
    float distanceFromCoordinates(Location from, Location to);
    
};

#endif