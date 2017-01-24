#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "main.h"

int main(int argc, char *argv[]){

    if(argc<=1) printf("Not enough arguments.\n");
    int xMax = atoi(argv[1]);
    int yMax = atoi(argv[2]);
    char **universe;

    printf("X: %i\nY: %i\n",xMax,yMax);

    universe = malloc(yMax * sizeof *universe);
    for(int i =0; i<yMax; i++){
        universe[i] = malloc(xMax * sizeof *universe[i]);
    }

    for(int i=3; i<argc; i++){
        printf("Pair: (%s,%s)\n",argv[i], argv[i+1]);
        universe[atoi(argv[i+1])][atoi(argv[i++])] = 'X';
    }

    bool alive = true; //kills simulation is no active cells left.
    displayuniverse(universe,yMax,xMax);
    while(alive){
        alive = checkRules(universe,yMax,xMax);
        displayuniverse(universe,yMax,xMax);
        usleep(83333);
    }


}

int countNeighbours(char** universe, int yPos, int xPos,int yMax, int xMax){
    int count = 0, Y, X;
    for (int k = -1; k < 2; k++) {
        if ((yPos + k) < 0) Y = yMax - 1;
        else if ((yPos + k) == yMax) Y = 0;
        else Y = yPos + k;
        for (int l = -1; l < 2; l++) {
            if ((xPos + l) < 0) X = xMax - 1;
            else if ((xPos + l) == xMax) X = 0;
            else X = xPos + l;

            if (universe[Y][X] == 'X' && (k != 0 && l != 0))count++;
        }
    }

}
bool checkRules(char **universe, int height, int width) {
    bool keepAlive =false;
    char** temp;
    temp = malloc(height * sizeof *temp);
    for (int i = 0; i < width; ++i) {
        temp[i] = malloc(width * sizeof *temp[i]);
    }
    for(int i =0; i<height; i++) {
        for (int j = 0; j < width; j++) {
            int count = countNeighbours(universe,i,j,height,width);

            //If cell is alive and has 0 or 1 neighbours, kill it
            if(count<=1 && universe[i][j]=='X'){
                temp[i][j] = ' ';
            }
            else if((count==2 || count==3) && universe[i][j]=='X') {
                temp[i][j] = 'X';
                keepAlive = true;
            }
            else if(count==3 && universe[i][j]==' ') {
                temp[i][j]='X';
                keepAlive = true;
            }
            else if(count>3 && universe[i][j]=='X') {
                temp[i][j] = ' ';
            }
            else {
                temp[i][j] = ' ';
            }
        }
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            universe[i][j] = temp[i][j];
        }
    }
    return keepAlive;
}

void displayuniverse(char** universe, int height, int width){
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            printf("%c",universe[i][j]);
        }
        printf("\n");
    }
}