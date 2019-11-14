#include "pathfinder.h"
#include "array.h"


// Pathfinder find the best move to reach the X,Y square
dir pathfinder(char * * map, int targetX, int targetY, int sizeX, int sizeY, bool en) {
    
    int matCost[sizeY][sizeX];
    int matVect[sizeY][sizeX];

    int start[2];
    int end[2];
    end[0] = targetX;
    end[1] = targetY;

    char c;
    for (int i=0; i<sizeY; i++)
    {
        for (int j=0; j<sizeX; j++)
        {
            c = map[i][j];
            if (c=='-' || c=='*' || !en && (c=='&' || c=='%' || c=='#' || c=='$'))
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

    /*for (int i=0; i<sizeY; i++)
    {
        for (int j=0; j<sizeX; j++)
        {
            printf("%d",matCost[i][j]);
        }
        printf("\n");
    } */

    //====================
    // FINDING THE BEST PATH
    //====================

    //Case to fill
    Array filling1X = NULL;
    Array filling1Y = NULL;

    //Case to fill the next loop
    Array filling2X = NULL;
    Array filling2Y = NULL;

    //Add the first case to fill : The start cell
    append(&filling1X,start[0]);
    append(&filling1Y,start[1]);

    //Filling the start cell
    matCost[start[1]][start[0]] = 1;

    // Filling cells of MatCost and MatVect
    // While the end cell is not filled, fill cells which are in the array up there
    // OR while filling arrays are not empty, if they are : No solution found
    int cellValue = 2;
    while(matCost[end[1]][end[0]] == 0 && filling1X != NULL)
    {
        /*for (int i=0; i<sizeY; i++)
        {
        for (int j=0; j<sizeX; j++)
        {
            printf("%d,",matCost[i][j]);
        }
        printf("\n");
        }
        printf("\n");*/

        for (int i=0; i<getLen(filling1X); i++)
        {
            //Top cell
            if (filling1Y->t[i]-1 >= 0 && matCost[filling1Y->t[i]-1][filling1X->t[i]] == 0)
            {
                matCost[filling1Y->t[i]-1][filling1X->t[i]] = cellValue;
                append(&filling2X,filling1X->t[i]);
                append(&filling2Y,filling1Y->t[i]-1);

                matVect[filling1Y->t[i]-1][filling1X->t[i]] = 2;
            }
            //Bottom cell
            if (filling1Y->t[i]+1 < sizeY && matCost[filling1Y->t[i]+1][filling1X->t[i]] == 0)
            {
                matCost[filling1Y->t[i]+1][filling1X->t[i]] = cellValue;
                append(&filling2X,filling1X->t[i]);
                append(&filling2Y,filling1Y->t[i]+1);

                matVect[filling1Y->t[i]+1][filling1X->t[i]] = 0;
            }
                //printf("%d - %d\n",i,getLen(filling2X));
            //Right cell
            if (filling1X->t[i]-1 >= 0 && matCost[filling1Y->t[i]][filling1X->t[i]-1] == 0)
            {
                matCost[filling1Y->t[i]][filling1X->t[i]-1] = cellValue;
                append(&filling2X,filling1X->t[i]-1);
                append(&filling2Y,filling1Y->t[i]);

                matVect[filling1Y->t[i]][filling1X->t[i]-1] = 1;
            }
            //Left cell
            if (filling1X->t[i]+1 < sizeX && matCost[filling1Y->t[i]][filling1X->t[i]+1] == 0)
            {
                matCost[filling1Y->t[i]][filling1X->t[i]+1] = cellValue;
                append(&filling2X,filling1X->t[i]+1);
                append(&filling2Y,filling1Y->t[i]);

                matVect[filling1Y->t[i]][filling1X->t[i]+1] = 3;
            }
        }

        // The next cost will be the current + 1
        cellValue++;

        // Empty the previous list of cells
        delete(&filling1X);
        delete(&filling1Y);

        // Filling the list with the next cells to fill
        for (int i=0; i<getLen(filling2X); i++)
        {
            append(&filling1X,filling2X->t[i]);
            append(&filling1Y,filling2Y->t[i]);
        }

        // Delete the saved list of cells, because it's now in filling1
        // We will refill it with the next list for the next itteration
        delete(&filling2X);
        delete(&filling2Y);
    }

    delete(&filling1X);
    delete(&filling1Y);

    //Filling the way

    Array wayX = NULL;
    Array wayY = NULL;

    if (!(matCost[end[1]][end[0]] <= 0)) {
        append(&wayX, end[0]);
        append(&wayY, end[1]);
    }

    int x = end[0];
    int y = end[1];

    //On trace le chemin sauf si la case de début est aussi la case d'arrivé OU case innateignable
    int isDone = ((end[0] == start[0]) && (end[1] == start[1])) || (matCost[end[1]][end[0]] <= 0);

    //printf("%d",isDone);

    while (!isDone)
    {
        switch(matVect[y][x])
        {
        case 0:
            append(&wayX, x);
            append(&wayY, --y);
            break;
        case 1:
            append(&wayX, ++x);
            append(&wayY, y);
            break;
        case 2:
            append(&wayX, x);
            append(&wayY, ++y);
            break;
        case 3:
            append(&wayX, --x);
            append(&wayY, y);
            break;
        }

        //Si la case actuel est la case d'arrivé, alors on a le chemin
        isDone = ((x == start[0]) && (y == start[1]));
    }

    //display(wayX);
    //display(wayY);

    //printf("%d - %d\n",getLast(wayX), getLast(wayY));

    for (int i=0; i<getLen(wayX); i++) {
        matCost[wayY->t[i]][wayX->t[i]] = 777;
    }


    if (wayX == NULL) {
        // printf("No way found\n");
        sleep(2);
        return NONE;
    } else {
        int xD = getFromEnd(wayX, 1)-start[0];
        int yD = getFromEnd(wayY, 1)-start[1];
        delete(&wayX);
        delete(&wayY);
        // printf("%d, %d\n",xD, yD);

        if (xD == 1) {
            return E;
        } else if (xD == -1) {
            return W;
        } else if (yD == 1) {
            return S;
        } else if (yD == -1) {
            return N;
        } else {
            return NONE;
        }
    }

    return 0;
}

// Pathfinder find the best move to reach the X,Y square
int pathfinderLen(char * * map, int targetX, int targetY, int sizeX, int sizeY, bool en) {
    
    int matCost[sizeY][sizeX];
    int matVect[sizeY][sizeX];

    int start[2];
    int end[2];
    end[0] = targetX;
    end[1] = targetY;

    char c;
    for (int i=0; i<sizeY; i++)
    {
        for (int j=0; j<sizeX; j++)
        {
            c = map[i][j];
            if (c=='-' || c=='*' || !en && (c=='&' || c=='%' || c=='#' || c=='$'))
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

    /*for (int i=0; i<sizeY; i++)
    {
        for (int j=0; j<sizeX; j++)
        {
            printf("%d",matCost[i][j]);
        }
        printf("\n");
    } */

    //====================
    // FINDING THE BEST PATH
    //====================

    //Case to fill
    Array filling1X = NULL;
    Array filling1Y = NULL;

    //Case to fill the next loop
    Array filling2X = NULL;
    Array filling2Y = NULL;

    //Add the first case to fill : The start cell
    append(&filling1X,start[0]);
    append(&filling1Y,start[1]);

    //Filling the start cell
    matCost[start[1]][start[0]] = 1;

    // Filling cells of MatCost and MatVect
    // While the end cell is not filled, fill cells which are in the array up there
    // OR while filling arrays are not empty, if they are : No solution found
    int cellValue = 2;
    while(matCost[end[1]][end[0]] == 0 && filling1X != NULL)
    {
        /*for (int i=0; i<sizeY; i++)
        {
        for (int j=0; j<sizeX; j++)
        {
            printf("%d,",matCost[i][j]);
        }
        printf("\n");
        }
        printf("\n");*/

        for (int i=0; i<getLen(filling1X); i++)
        {
            //Top cell
            if (filling1Y->t[i]-1 >= 0 && matCost[filling1Y->t[i]-1][filling1X->t[i]] == 0)
            {
                matCost[filling1Y->t[i]-1][filling1X->t[i]] = cellValue;
                append(&filling2X,filling1X->t[i]);
                append(&filling2Y,filling1Y->t[i]-1);

                matVect[filling1Y->t[i]-1][filling1X->t[i]] = 2;
            }
            //Bottom cell
            if (filling1Y->t[i]+1 < sizeY && matCost[filling1Y->t[i]+1][filling1X->t[i]] == 0)
            {
                matCost[filling1Y->t[i]+1][filling1X->t[i]] = cellValue;
                append(&filling2X,filling1X->t[i]);
                append(&filling2Y,filling1Y->t[i]+1);

                matVect[filling1Y->t[i]+1][filling1X->t[i]] = 0;
            }
                //printf("%d - %d\n",i,getLen(filling2X));
            //Right cell
            if (filling1X->t[i]-1 >= 0 && matCost[filling1Y->t[i]][filling1X->t[i]-1] == 0)
            {
                matCost[filling1Y->t[i]][filling1X->t[i]-1] = cellValue;
                append(&filling2X,filling1X->t[i]-1);
                append(&filling2Y,filling1Y->t[i]);

                matVect[filling1Y->t[i]][filling1X->t[i]-1] = 1;
            }
            //Left cell
            if (filling1X->t[i]+1 < sizeX && matCost[filling1Y->t[i]][filling1X->t[i]+1] == 0)
            {
                matCost[filling1Y->t[i]][filling1X->t[i]+1] = cellValue;
                append(&filling2X,filling1X->t[i]+1);
                append(&filling2Y,filling1Y->t[i]);

                matVect[filling1Y->t[i]][filling1X->t[i]+1] = 3;
            }
        }

        // The next cost will be the current + 1
        cellValue++;

        // Empty the previous list of cells
        delete(&filling1X);
        delete(&filling1Y);

        // Filling the list with the next cells to fill
        for (int i=0; i<getLen(filling2X); i++)
        {
            append(&filling1X,filling2X->t[i]);
            append(&filling1Y,filling2Y->t[i]);
        }

        // Delete the saved list of cells, because it's now in filling1
        // We will refill it with the next list for the next itteration
        delete(&filling2X);
        delete(&filling2Y);
    }

    delete(&filling1X);
    delete(&filling1Y);

    //Filling the way

    Array wayX = NULL;
    Array wayY = NULL;

    if (!(matCost[end[1]][end[0]] <= 0)) {
        append(&wayX, end[0]);
        append(&wayY, end[1]);
    }

    int x = end[0];
    int y = end[1];

    //On trace le chemin sauf si la case de début est aussi la case d'arrivé OU case innateignable
    int isDone = ((end[0] == start[0]) && (end[1] == start[1])) || (matCost[end[1]][end[0]] <= 0);

    //printf("%d",isDone);

    while (!isDone)
    {
        switch(matVect[y][x])
        {
        case 0:
            append(&wayX, x);
            append(&wayY, --y);
            break;
        case 1:
            append(&wayX, ++x);
            append(&wayY, y);
            break;
        case 2:
            append(&wayX, x);
            append(&wayY, ++y);
            break;
        case 3:
            append(&wayX, --x);
            append(&wayY, y);
            break;
        }

        //Si la case actuel est la case d'arrivé, alors on a le chemin
        isDone = ((x == start[0]) && (y == start[1]));
    }

    //display(wayX);
    //display(wayY);

    //printf("%d - %d\n",getLast(wayX), getLast(wayY));

    for (int i=0; i<getLen(wayX); i++) {
        matCost[wayY->t[i]][wayX->t[i]] = 777;
    }

    if (wayX == NULL) {
        printf("No way found\n");
        return 10000;
    } else {
        getLen(wayX);
    }
}