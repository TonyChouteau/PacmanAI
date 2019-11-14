// add the needed C libraries below
#include <stdbool.h> // bool, true, false
#include <stdlib.h> // rand

// look at the file below for the definition of the direction type
// pacman.h must not be modified!
#include "pacman.h"

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
const char * binome="Random";

// put the prototypes of your additional functions/procedures below


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

  // answer to the game engine 
  return d;
}

// the code of your additional functions/procedures must be put below

