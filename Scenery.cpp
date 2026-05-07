#include "Scenery.h"
#include <optional>
#include <vector>

using namespace std;

Scenery::Scenery(string description, RiskMatrix riskMatrix, vector<Base*> bases) :
description(description), riskMatrix(riskMatrix), matrixLimits(riskMatrix.getMatrixLimits()), bases(bases) {}

optional<Scenery*> Scenery::buildScenery(const string& description, const RiskMatrix& riskMatrix, const vector<Base*>& bases) {
    // Check all attributes BEFORE creating the object
    Location lim = riskMatrix.getMatrixLimits();

    if (
        description.empty() or
        lim.x < 1 or
        lim.y < 1 or
        bases.empty()
    ) return nullopt;

    // Create a preliminary new Scenery object
    Scenery* scenery = new Scenery(description, riskMatrix, bases);

    // Run bases validation
    if(scenery->determineRisk()) return scenery;
    else {
        delete scenery;
        return nullopt;
    }
}

Scenery::~Scenery() {
    for (auto &x : bases) {
        delete x;
    }
}

bool Scenery::determineRisk() {
    for(auto &b : bases) {
        Location loc = b->getLocation();
        if (matrixLimits.x < loc.x || matrixLimits.y < loc.y) return false;
        b->setRisk(riskMatrix.getRiskAt(loc.x, loc.y));
    }
    return true;
}

vector<Base*> run() {
    
}