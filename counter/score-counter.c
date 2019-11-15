#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>

/*
    Signal handler :

    1 - Coin
    2 - Energy
    3 - Ghost

*/

#define COINV 10
#define BOOSTV 50
#define GHOSTV 200

#define COIN 3
#define BOOST 4
#define GHOST 5

long long coinCount;
long long energyCount;
long long ghostCount;
long long ghostInRow;

long long score;

void p();

void signalHandler(int signal){
    switch(signal) {
    case COIN: //Coin
        coinCount++;
        score+=COINV;
        p();
        break;
    case BOOST: //Energy
        ghostInRow = 0;
        energyCount++;
        score+=BOOSTV;
        p();
        break;
    case GHOST:
        ghostCount++;
        ghostInRow++;
        score+=GHOSTV*pow(2, ghostInRow-1);
        p();
        break;
    }
}

void p(){
     printf("Coin(s) %lld - Pacgum(s) %lld - Ghost(s) - %lld - Ghost in a row : %lld | TOTAL : %lld\n", coinCount, energyCount, ghostCount, ghostInRow, score);
}


int main(){
    
    coinCount = 0;
    energyCount = 0;
    ghostCount = 0;
    ghostInRow = 0;

    for (int i=COIN; i<GHOST+1; i++){
        if (signal(i, signalHandler) == SIG_ERR) {
            printf("Error handling signal %d\n", i);
        }
    }

    while (true) {
        sleep(1);
    }
}