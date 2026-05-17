#ifndef LINK_H
#define LINK_H

#include <cmath>
#include "Base.h"

using namespace std;

// Generic structure that abstracts data for algorithm graph
struct Edge {
    int target;
    float cost;
};

struct Link {
    Base* nodes[2];
    float distance;

    Link(Base* from, Base* to);
    float distanceFromCoordinates(Location from, Location to);
};

#endif