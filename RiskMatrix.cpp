#include "RiskMatrix.h"

RiskMatrix::RiskMatrix() {
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            dangerZone[i][j] = 1.0;
        }
    }
    dangerZone[5][5] = 8.5;
}

float RiskMatrix::getRiskAt(int x, int y) {
    
    if (x > 9) x = 9;
    if (y > 9) y = 9;
    
    
    if (x < 0) x = 0;
    if (y < 0) y = 0;

    
    return dangerZone[x][y]; 
}