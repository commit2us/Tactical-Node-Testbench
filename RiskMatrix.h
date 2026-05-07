#ifndef RISK_MATRIX_H
#define RISK_MATRIX_H

class RiskMatrix {
    float dangerZone[10][10];

public:
    RiskMatrix();

    float getRiskAt(int x, int y);

};

#endif