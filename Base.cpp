using namespace std;
#include "Base.h"
#include <string>

Base :: Base ()
{
    id = "unknown";
    name = "unknown";
    location.x = 0;
    location.y = 0;
    risk = 1.0f;
}

Base :: Base (string i,int lx, int ly, string name, float r )
{
    id = i;
    if (lx >= 0) location.x = lx; else location.x = 0;
    if (ly >= 0) location.y = ly; else location.y = 0;
    name = name;
    if (r > 0) risk = r; else risk = 1.0f;
}

Base::~Base(){};

  //setters 
void Base::setId(string i) {
    id = i;
}

void Base::setLocation(int lx, int ly) {
  location.x = (lx >= 0) ? lx : 0;
    location.y = (ly >= 0) ? ly : 0;
}

void Base::setName(string name) {
    name = name;
}

void Base::setRisk(float r) {
    
    if (r > 0) risk = r;
    else risk = 1.0f;
}

// --- GETTERS---

string Base::getId() {
    return id;
}

Location Base::getLocation() {
    return location;
}

string Base::getName() {
    return name;
}

float Base::getRisk() {
    return risk;
}




