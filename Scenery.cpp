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
    // Determine risk for each base with riskMatriz as reference
}