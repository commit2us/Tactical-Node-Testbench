#ifndef SCENERY_H
#define SCENERY_H

#include <string>
#include <vector>

#include "RiskMatrix.cpp"
#include "Base.h"

using namespace std;

class Scenery{
    string description;
    RiskMatrix riskMatrix;
    vector<Base*> bases;

public:
    Scenery(string description, RiskMatrix riskMatrix, vector<Base*> bases);

    ~Scenery();

private:
    void determineRisk();

};

#endif