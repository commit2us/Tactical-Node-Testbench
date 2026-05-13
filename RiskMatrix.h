#ifndef RISK_MATRIX_H
#define RISK_MATRIX_H

class RiskMatrix {
public:
    float dangerZone[10][10];

    RiskMatrix();

    float getRiskAt(int x, int y);

};

#endif