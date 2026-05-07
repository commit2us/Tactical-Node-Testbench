#include "Scenery.h"

Scenery::Scenery(string description, RiskMatrix riskMatrix, vector<Base*> bases) :
description(description), riskMatrix(riskMatrix), bases(bases) {
    determineRisk();
}

Scenery::~Scenery() {
    for (auto &x : bases) {
        delete x;
    }
}

void Scenery::determineRisk() {
    for(auto &b : bases) {
        b->setRisk(riskMatrix.getRiskAt(b->getLocation().x, b->getLocation().y));
    }
}