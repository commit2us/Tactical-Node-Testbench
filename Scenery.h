#ifndef SCENERY_H
#define SCENERY_H

#include <list>
#include <string>
#include <vector>
#include "Network.h"
#include "RiskMatrix.h"
#include "Base.h"
#include "Link.h"

using namespace std;

class Scenery{
    string description;
    RiskMatrix riskMatrix;
    list<Base*> bases;
    list<Link*> links;
    Network* graph = nullptr;

public:

    // Accept bases as a non-owning reference (Scenery won't delete bases)
    Scenery(string description, RiskMatrix riskMatrix, const list<Base*>& bases, const list<Link*>& links);

    ~Scenery();

    string getDescription();
    void   displayBoard();
    static Scenery* createTutorialLevel(const list<Base*>& bases, const list<Link*>& links);
    static Scenery* createMinefieldLevel(const list<Base*>& bases, const list<Link*>& links);
    static Scenery* createBlockadeLevel(const list<Base*>& bases, const list<Link*>& links);
    void run(Base* from, Base* to);

private:
    void determineRisk();


};

#endif