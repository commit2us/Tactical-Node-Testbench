#ifndef RISK_MATRIX_H
#define RISK_MATRIX_H

#include <vector>

#include "Base.h"

// Namespace in headers is not recommended
// using namespace std;

class RiskMatrix {
    std::vector<std::vector<float>> dangerZone;

public:
    RiskMatrix();

    Location getMatrixLimits() const;
    
    float getRiskAt(int x, int y);

};

#endif