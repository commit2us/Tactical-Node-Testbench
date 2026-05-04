using namespace std;
#include "base.h"
#include <string>
Base :: Base ()
{
    id = "unknown";
    location.x = 0;
    location.y = 0;
    frequency = "000.0MHz";
    risk = 1.0f;
    bandWidth = "Media";
}

Base :: Base (string i,int lx, int ly, string freq, float r, string bW );
{
    id = i;
    if (lx >= 0) location.x = lx; else location.x = 0;
    if (ly >= 0) location.y = ly; else location.y = 0;
    frequency = freq;
    if (r > 0) risk = r; else risk = 1.0f;
    bandWidth = bW;
}

~Base();

  //setters 
void Base::setId(string i) {
    id = i;
}

void Base::setLocation(int lx, int ly) {
  location.x = (lx >= 0) ? lx : 0;
    location.y = (ly >= 0) ? ly : 0;
}

void Base::setFrequency(string freq) {
    frequency = freq;
}

void Base::setRisk(float r) {
    
    if (r > 0) risk = r;
    else risk = 1.0f;
}

void Base::setBandWidth(string bW) {
    bandWidth = bW;
}

// --- GETTERS---

string Base::getId() {
    return id;
}

Location Base::getLocation() {
    return location;
}

string Base::getFrequency() {
    return frequency;
}

float Base::getRisk() {
    return risk;
}

string Base::getBandWidth() {
    return bandWidth;
}




