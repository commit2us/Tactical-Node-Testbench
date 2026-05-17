#ifndef SCENERY_H
#define SCENERY_H

#include <list>
#include <string>
#include <vector>
#include "RiskMatrix.h"
#include "Base.h"
#include "Link.h"

using namespace std;

class Scenery{
    string description;
    RiskMatrix riskMatrix;
    list<Base*> bases;
    list<Link*> links;



public:

    // Accept bases as a non-owning reference (Scenery won't delete bases)
    Scenery(string description, RiskMatrix riskMatrix, const list<Base*>& bases);

    ~Scenery();

    string getDescription();
    void   displayBoard();
    static Scenery* createTutorialLevel();
    static Scenery* createMinefieldLevel();
    static Scenery* createBlockadeLevel();


private:
    void determineRisk();


};

#endif