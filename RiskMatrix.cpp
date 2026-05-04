class RiskMatrix
{
    private:
        float dangerZone[10][10];

    public:

        RiskMatrix()
        {
            for(int i = 0; i < 10; i++)
                for(int j = 0; j < 10; j++)
                    dangerZone[i][j] = 1.0;

            dangerZone[5][5] = 8.5;

        }

        float getRiskAt(int x, int y)
        {
            int sectorX = x / 100;
            int sectorY =  y /100;
            if(sectorX > 9)sectorX = x;
            if (sectorY > 9) sectorY = y;

        }
        
};