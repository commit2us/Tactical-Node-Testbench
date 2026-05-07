#ifndef SCENERY_H
#define SCENERY_H

#include <string>
#include <vector>
#include <optional>

#include "RiskMatrix.h"
#include "Base.h"

class Scenery{
    std::string description;
    RiskMatrix riskMatrix;
    Location matrixLimits;
    std::vector<Base*> bases;

    Scenery(std::string description, RiskMatrix riskMatrix, std::vector<Base*> bases);

public:
    std::optional<Scenery*> buildScenery(const std::string& description, const RiskMatrix& riskMatrix, const std::vector<Base*>& bases);
    ~Scenery();

    std::vector<Base*> run();

private:
    bool determineRisk();

};

#endif