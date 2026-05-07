#include "RiskMatrix.h"
#include "Base.h"

RiskMatrix::RiskMatrix() {
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            dangerZone[i][j] = 1.0;
        }
    }
    dangerZone[5][5] = 8.5;
}

float RiskMatrix::getRiskAt(int x, int y) {
    int sectorX = x / 100;
    int sectorY = y / 100;
    
    if(sectorX > 9) sectorX = 9;
    if(sectorY > 9) sectorY = 9;

    return dangerZone[sectorX][sectorY]; 
}

Location RiskMatrix::getMatrixLimits() const {
    if (dangerZone.size() < 1) return Location(0, 0);
    
    return Location(dangerZone.size(), dangerZone[0].size());
}