#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>

/*
    Signal handler :

    1 - Coin
    2 - Energy
    3 - Ghost

*/

#define COIN 3
#define BOOST 4
#define GHOST 5

int coinCount;
int energyCount;
int ghostCount;

void signalHandler(int signal){
    switch(signal) {
    case COIN: //Coin
        coinCount++;
        break;
    case BOOST: //Energy
        energyCount++;
        break;
    case GHOST:
        ghostCount++;
        break;
    }
    printf("Coin(s) %d - Pacgum(s) %d - Ghost(s) - %d\n", coinCount, energyCount, ghostCount);
}


int main(){
    
    coinCount = 0;
    energyCount = 0;
    ghostCount = 0;

    for (int i=COIN; i<GHOST+1; i++){
        if (signal(i, signalHandler) == SIG_ERR) {
            printf("Error handling signal %d\n", i);
        }
    }

    while (true) {
        sleep(1);
    }
}