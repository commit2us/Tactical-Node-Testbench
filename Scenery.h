#ifndef SCENERY_H
#define SCENERY_H

#include <string>
#include <vector>

// #include "RiskMatrix.h"
struct RiskMatrix {};
// #include "Base.h"
struct Base {};


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