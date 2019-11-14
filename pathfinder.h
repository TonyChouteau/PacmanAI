
#include <stdio.h>
#include <stdbool.h>
#include "util.h"

dir pathfinder(char * * map, int targetX, int targetY, int sizeX, int sizeY, bool en);
int pathfinderLen(char * * map, int targetX, int targetY, int sizeX, int sizeY, bool en);