#include "Scenery.h"
#include <iostream>
#include <cmath>

Scenery::Scenery(string description, RiskMatrix riskMatrix,const list<Base*>& bases):
    description(description), riskMatrix(riskMatrix), bases(bases) {
    determineRisk();

}

Scenery::~Scenery() {
    for (auto &x : bases) delete x;
    for (auto &l : links) delete l;
}

void Scenery::determineRisk() {
    for(auto &b : bases) {
        b->setRisk(riskMatrix.getRiskAt(b->getLocation().x, b->getLocation().y));
    }
}

string Scenery::getDescription() {
    return description;
}
Scenery* Scenery::createTutorialLevel() {
    RiskMatrix rm;
    list<Base*> bps; 
    
    for(int i = 0; i < 10; i++)
        for(int j = 0; j < 10; j++)
            bps.push_back(new Base(to_string(i) + "," + to_string(j), i, j, "Unknown", 1.0f));

    return new Scenery("Level 1: Tutorial Mode (Safe Board with No Mines)", rm, bps);
}
Scenery* Scenery::createMinefieldLevel() {
    RiskMatrix rm;
    rm.setRiskAt(2, 3, 9.0f);
    rm.setRiskAt(4, 5, 9.0f);
    rm.setRiskAt(7, 2, 9.0f);

    list<Base*> bps; 
    for(int i = 0; i < 10; i++)
        for(int j = 0; j < 10; j++)
            bps.push_back(new Base(to_string(i) + "," + to_string(j), i, j, "Unknown", 1.0f));

    return new Scenery("Level 2: Minefield Match (Evasion of high danger cells)", rm, bps);
}

Scenery* Scenery::createBlockadeLevel() {
    RiskMatrix rm;
    for(int row = 0; row < 10; row++) {
        rm.setRiskAt(row, 5, 10.0f);
    }

    list<Base*> bps;
    for(int i = 0; i < 10; i++)
        for(int j = 0; j < 10; j++)
            bps.push_back(new Base(to_string(i) + "," + to_string(j), i, j, "Unknown", 1.0f));

    return new Scenery("Level 3: Blockade Course (Forcing peripheral rerouting around the edges)", rm, bps);
}

void Scenery::determineRisk() {
    
}

void Scenery::displayBoard() {
    cout << "\n=============================================" << endl;
    cout << "  SCREEN: " << description << endl;
    cout << "=============================================" << endl;
    
    cout << "    ";
    for(int j = 0; j < 10; j++) cout << j << "  ";
    cout << "\n    ------------------------------\n";

    for (int i = 0; i < 10; i++) {
        cout << i << " | ";
        for (int j = 0; j < 10; j++) {
            float risk = riskMatrix.getRiskAt((float)i, (float)j);
            
            if (risk >= 10.0f) {
                cout << "X  ";   // X = Bloqueo total
            } 
            else if (risk >= 5.0f) {
                cout << "M  ";   // M = Mina
            } 
            else {
                cout << ".  ";   // . = Limpio
            }
        }
        cout << "|\n";
    }
    cout << "    ------------------------------\n";
}