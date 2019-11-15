// add the needed C libraries below
#include <stdbool.h> // bool, true, false
#include <stdlib.h> // rand
#include <math.h>
#include <unistd.h>

// look at the file below for the definition of the direction type
// pacman.h must not be modified!
#include "pacman.h"
#include "pathfinder.h"

#define COIN 3
#define BOOST 4
#define GHOST 5

// ascii characters used for drawing levels
extern const char PACMAN; // ascii used for pacman
extern const char WALL; // ascii used for the walls
extern const char PATH; // ascii used for the explored paths
extern const char DOOR; // ascii used for the ghosts' door
extern const char VIRGIN_PATH; // ascii used for the unexplored paths
extern const char ENERGY; // ascii used for the energizers
extern const char GHOST1; // ascii used for the ghost 1
extern const char GHOST2; // ascii used for the ghost 2
extern const char GHOST3; // ascii used for the ghost 3
extern const char GHOST4; // ascii used for the ghost 4

// reward (in points) when eating dots/energizers 
extern const int VIRGIN_PATH_SCORE; // reward for eating a dot
extern const int ENERGY_SCORE; // reward for eating an energizer

// put the student names below (mandatory)
const char * binome="Sébastien HERT & Tony CHOUTEAU";

// put the prototypes of your additional functions/procedures below
dir getEnergy(char ** map,int xsize, int ysize, int x, int y);
dir getGhosts(char ** map,int xsize, int ysize, int x, int y);
direction goRandom(char ** map,int xsize, int ysize, int x, int y);
int howMany(char** map, int xsize, int ysize, char);
int howManyGhosts(char** map, int xsize, int ysize);
direction dirToDirection(dir d2, char ** map,int xsize, int ysize, int x, int y, direction lastdirection);
dir getCoin(char ** map,int xsize, int ysize, int x, int y);
dir checkghost(char ** map, int xsize, int ysize, int x, int y, dir d2);
bool isGhost(char ** map, int xsize, int ysize, int x, int y);
int countEnergy(char ** map, int xsize, int ysize, int x, int y, int nbEnergy);
void countScore(char** map, int xsize, int ysize, int x, int y, direction d);

void sendSignal(char c);
void saveInFile(char ** map, int xsize, int ysize);
void readFile();

// change the pacman function below to build your own player
// your new pacman function can use as many additional functions/procedures as needed; put the code of these functions/procedures *AFTER* the pacman function
direction pacman(
			char * * map, // the map as a dynamic array of strings, ie of arrays of chars
			int xsize, // number of columns of the map
			int ysize, // number of lines of the map
			int x, // x-position of pacman in the map 
			int y, // y-position of pacman in the map
			direction lastdirection, // last move made by pacman (see pacman.h for the direction type; lastdirection value is -1 at the beginning of the game
			bool energy, // is pacman in energy mode? 
			int remainingenergymoderounds // number of remaining rounds in energy mode, if energy mode is true
			) {
	direction d; // the direction to return
	dir d2 = NONE; //intermediate direction

	//First, we should get super powers
	int nbEnergy = howMany(map, xsize, ysize, ENERGY);
	int energyLeft = countEnergy(map, xsize, ysize, x, y, nbEnergy);
	if ( !energy || (energy && remainingenergymoderounds<energyLeft) ){
		if (nbEnergy > 0){
			d2 = getEnergy(map, xsize, ysize, x, y);
			d2 = checkghost(map, xsize, ysize, x, y, d2);
		}else{
			d2 = getCoin(map, xsize, ysize, x, y);
			d2 = checkghost(map, xsize, ysize, x, y, d2);
		}
	}

	//Now we have super Powers, we should find the best way to find Ghosts
	else {
		d2 = getGhosts(map, xsize, ysize, x, y);
	}

	d = dirToDirection(d2, map, xsize, ysize, x, y, lastdirection);
	// countScore(map, xsize, ysize, x, y, d);

	if (d2 == N) {
		d = NORTH;
		sendSignal(map[y-1][x]);
	}else if (d2 == S) {
		d = SOUTH;
		sendSignal(map[y+1][x]);
	}else if (d2 == E) {
		d = EAST;
		sendSignal(map[y][x+1]);
	}else if (d2 == W) {
		d = WEST;
		sendSignal(map[y][x-1]);
	}
	usleep(500000);

  	// answer to the game engine 
	return d;
}

// the code of your additional functions/procedures must be put below

dir getEnergy(char ** map,int xsize, int ysize, int x, int y){
	// int nb = howMany(map, xsize, ysize,ENERGY);

	int xEnergy = 0;
	int yEnergy = 0;
	float lenMin = -1;

	for (size_t i = 0; i < xsize; i++){
		for (size_t j = 0; j < ysize; j++){
			if ( map[j][i] == ENERGY ){
				float newLen = sqrt( (i - x) * (i - x) + (j - y) * (j - y) );
				if (lenMin == -1 || lenMin > newLen){
					lenMin = newLen;
					xEnergy = i;
					yEnergy = j;
				}
			}
		}
	}

	dir d2 = pathfinder(map, xEnergy, yEnergy, xsize, ysize, false);
	
	return d2;
}

dir getCoin(char ** map,int xsize, int ysize, int x, int y){
	// int nb = howMany(map, xsize, ysize, VIRGIN_PATH);
	int xCoin = 0;
	int yCoin = 0;
	float lenMin = -1;

	for (size_t i = 0; i < xsize; i++){
		for (size_t j = 0; j < ysize; j++){
			if ( map[j][i] == VIRGIN_PATH ){
				float newLen = sqrt( (i - x) * (i - x) + (j - y) * (j - y) );
				if (lenMin == -1 || lenMin > newLen){
					lenMin = newLen;
					xCoin = i;
					yCoin = j;
				}
			}
		}
	}

	dir d2 = pathfinder(map, xCoin, yCoin, xsize, ysize, false);
	
	return d2;
}

dir getGhosts(char ** map,int xsize, int ysize, int x, int y){
	// int nb = howManyGhosts(map, xsize, ysize);

	int xGhost = 0;
	int yGhost = 0;
	int lenMin = -1;

	for (size_t i = 0; i < xsize; i++){
		for (size_t j = 0; j < ysize; j++){
			if ( map[j][i] == GHOST1 || map[j][i] == GHOST2 || map[j][i] == GHOST3 || map[j][i] == GHOST4 ){
				int newLen = pathfinderLen(map, i, j, xsize, ysize, true);					
				if (lenMin == -1 || lenMin > newLen){
					lenMin = newLen;
					xGhost = i;
					yGhost = j;
				}
			}
		}
	}

	dir d2 = pathfinder(map, xGhost, yGhost, xsize, ysize, true);
	
	return d2;
}

direction goRandom(char ** map,int xsize, int ysize, int x, int y){
	direction d;
	bool north=false; // indicate whether pacman can go north; no by default
  	bool east=false; // indicate whether pacman can go east; no by default
  	bool south=false; // indicate whether pacman can go south; no by default
	bool west=false; // indicate whether pacman can go west; no by default
	bool ok=false; // turn true when a valid direction is randomly chosen

	// can pacman go north?
	if(y==0 || (y>0 && map[y-1][x]!=WALL && map[y-1][x]!=DOOR)) north=true;
	// can pacman go east?
	if(x==xsize-1 || (x<xsize-1 && map[y][x+1]!=WALL && map[y][x+1]!=DOOR)) east=true;
	// can pacman go south?
	if(y==ysize-1 || (y<ysize-1 && map[y+1][x]!=WALL && map[y+1][x]!=DOOR)) south=true;
	// can pacman go west?
	if(x==0 || (x>0 && map[y][x-1]!=WALL && map[y][x-1]!=DOOR)) west=true;

	// guess a direction among the allowed four, until a valid choice is made
	do {
		d=rand()%4;
		if((d==NORTH && north)
		|| (d==EAST && east)
		|| (d==SOUTH && south)
		|| (d==WEST && west)) {
		ok=true;
			}  
  	} while(!ok);
	return d;
}

int howMany(char** map, int xsize, int ysize, char c){
	int cpt = 0;
	for (size_t i = 0; i < xsize; i++){
		for (size_t j = 0; j < ysize; j++){
			if (map[j][i] == c){
				cpt++;
			}
		}
	}
	// printf("%d\n", cpt);
	return cpt;
}

int howManyGhosts(char** map, int xsize, int ysize){
	int cpt = 0;
	for (size_t i = 0; i < xsize; i++){
		for (size_t j = 0; j < ysize; j++){
			if (map[j][i] == GHOST1 || map[j][i] == GHOST2 || map[j][i] == GHOST3 || map[j][i] == GHOST4){
				cpt++;
			}
		}
	}
	// printf("%d\n", cpt);
	return cpt;
}

dir checkghost(char ** map, int xsize, int ysize, int x, int y, dir d2){
	switch (d2){
	case N:
		y--;
		break;
	case S:
		y++;
		break;
	case E:
		x++;
		break;
	case W:
		x--;
		break;	
	default:
		return d2;
	}

	if ( isGhost(map, xsize, ysize, x+1, y) || isGhost(map, xsize, ysize, x-1, y) || isGhost(map, xsize, ysize, x, y+1) || isGhost(map, xsize, ysize, x, y-1) ){
		d2 = NONE;
	}

	return d2;
}

bool isGhost(char ** map, int xsize, int ysize, int x, int y){
	return ( map[y%ysize][x%xsize] == GHOST1 || map[y%ysize][x%xsize] == GHOST2 || map[y%ysize][x%xsize] == GHOST3 || map[y%ysize][x%xsize] == GHOST4);
}

direction dirToDirection(dir d2, char ** map,int xsize, int ysize, int x, int y, direction lastdirection){	
	direction d;
	if (d2 == N) {
		d = NORTH;
	}else if (d2 == S) {
		d = SOUTH;
	}else if (d2 == E) {
		d = EAST;
	}else if (d2 == W) {
		d = WEST;
	}else {
		d = (lastdirection+2)%4;
	}
	return d;	
}

void sendSignal(char c) {
	if (c == '.') {
		printf("Coin");
		system("pkill -3 score");
	}
	if (c == 'O') {
		printf("Boost");
		system("pkill -4 score");
	}
	if (c == '$' || c == '#' || c == '%' || c == '&') {
		printf("Ghost");
		system("pkill -5 score");
	}
}


int countEnergy(char ** map, int xsize, int ysize, int x, int y, int nbEnergy){
	if (nbEnergy == 0){
		return 10;
	}
	int xEnergy = 0;
	int yEnergy = 0;
	float lenMin = -1;

	for (size_t i = 0; i < xsize; i++){
		for (size_t j = 0; j < ysize; j++){
			if ( map[j][i] == ENERGY ){
				float newLen = sqrt( (i - x) * (i - x) + (j - y) * (j - y) );
				if (lenMin == -1 || lenMin > newLen){
					lenMin = newLen;
					xEnergy = i;
					yEnergy = j;
				}
			}
		}
	}

	int count = pathfinderLen(map, xEnergy, yEnergy, xsize, ysize, false);
	
	return count;
}

void countScore(char ** map, int xsize, int ysize, int x, int y, direction d){
	switch (d){
	case NORTH:
		y--;
		break;
	case SOUTH:
		y++;
		break;
	case EAST:
		x++;
		break;
	case WEST:
		x--;
		break;
	}

	FILE *fp;
	int i;

	fp = fopen("./compteur", "w+");

	fscanf(fp, "%d", &i);

	printf ("                                                         %d\n", i);
	fclose(fp);
	remove("./compteur");

	

	switch (map[y%ysize][x%xsize]){
	case '.':
		i = i+10;
		break;
	case 'O':
		i = i+50;
		break;
	case '&':
	case '%':
	case '#':
	case '$':
		i = 2*i;
		break;
	default:
		break;
	}
	// sleep(1);

	fp = fopen("./compteur", "w+");
	fprintf(fp, "%d\n", i);
	printf("                                                                                        %d\n", i);
	fclose(fp);
	// sleep (1);
}