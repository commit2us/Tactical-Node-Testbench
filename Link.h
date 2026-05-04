#ifndef LINK_H
#define LINK_H

#include <cmath>
#include "Base.h"

using namespace std;


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