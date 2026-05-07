#ifndef SCENERY_H
#define SCENERY_H

#include <string>
#include <vector>
#include <optional>

#include "RiskMatrix.h"
#include "Base.h"

using namespace std;

class Scenery{
    string description;
    RiskMatrix riskMatrix;
    Location matrixLimits;
    vector<Base*> bases;

    Scenery(string description, RiskMatrix riskMatrix, vector<Base*> bases);

public:
    optional<Scenery*> buildScenery(const string& description, const RiskMatrix& riskMatrix, const vector<Base*>& bases);
    ~Scenery();

private:
    bool determineRisk();

};

#endif