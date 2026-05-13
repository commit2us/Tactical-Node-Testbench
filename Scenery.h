#ifndef SCENERY_H
#define SCENERY_H

#include <list>
#include <string>
#include <vector>

#include "RiskMatrix.h"
#include "Base.h"

using namespace std;

class Scenery{
    string description;
    RiskMatrix riskMatrix;
    list<Base*> bases;

public:
    Scenery(string description, RiskMatrix riskMatrix, list<Base*> bases);

    ~Scenery();

    string getDescription();

private:
    void determineRisk();

};

#endif