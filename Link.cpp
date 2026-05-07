#include "Link.h"

Link::Link(Base* from, Base* to) : 
bases{from, to}, distance(0.0){
    distance = distanceFromCoordinates(from->getLocation(), to->getLocation());
}
    
Link::~Link() {
    // No use for now
}

float Link::distanceFromCoordinates(Location from, Location to) {
    return hypot(from.x - to.x, from.y - to.y);
}