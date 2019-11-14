#include "pathfinder.h"

// Pathfinder find the best move to reach the X,Y square
int pathfinder(char * * map, int x, int y, int sizeX, int sizeY) {
    
    int matCost[sizeY][sizeX];
    int matVect[sizeY][sizeX];

    int start[2];
    int end[2];
    end[0] = x;
    end[1] = y;

    char c;
    for (int i=0; i<sizeY; i++)
    {
        for (int j=0; j<sizeX; j++)
        {
            c = map[i][j];
            if (c=='-' || c=='*')
            {
                matCost[i][j] = -1;
                matVect[i][j] = -1;
            }
            else
            {
                matCost[i][j] = 0;
                matVect[i][j] = 0;
            }

            if (c=='@')
            {
                start[0] = j;
                start[1] = i;
            }
            /*if (c=='?')
            {
                end[0] = j;
                end[1] = i;
            }*/
        }
    }

    for (int i=0; i<sizeY; i++)
    {
        for (int j=0; j<sizeX; j++)
        {
            printf("%d",matCost[i][j]);
        }
        printf("\n");
    }

    return 0;
}