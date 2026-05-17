#ifndef LINK_H
#define LINK_H

#include <cmath>
#include "Base.h"

using namespace std;

// Generic structure that abstracts data for algorithm graph
struct Edge {
    Base* target;
    float cost;
};

// Enum to select mode
enum Mode {
    Safest,
    Shortest,
    Optimal
};

struct Link {
    Base* nodes[2];
    float distance;

    Link(Base* from, Base* to);
    float distanceFromCoordinates(Location from, Location to);
};

#endif