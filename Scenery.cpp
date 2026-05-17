#include "Scenery.h"

Scenery::Scenery(string description, RiskMatrix riskMatrix, const list<Base*>& bases) :
description(description), riskMatrix(riskMatrix), bases(bases) {
    determineRisk();
}

Scenery::~Scenery() {
    // Scenery does not own the Base pointers; do not delete them here.
}

void Scenery::determineRisk() {
    for(auto &b : bases) {
        b->setRisk(riskMatrix.getRiskAt(b->getLocation().x, b->getLocation().y));
    }
}

string Scenery::getDescription() {
    return description;
}