#include <stdio.h>
#include "main.h"

int main(int argc, char *argv[]){

    if(argc<=1) printf("Not enough arguments.\n");
    int xMax = parseInt(argv[1]);
    int yMax = parseInt(argv[2]);

    printf("X: %i\n Y: %i\n",xMax,yMax);

    char habitat[xMax][yMax];
    for(int i=3; i<argc; i++){
        printf("Pair: (%i,%i)\n",parseInt(argv[i]),parseInt(argv[i+1]));
        habitat[parseInt(argv[i])][parseInt(argv[i])] = '0';

    }


}

int parseInt(char c){
    return (c + 0x30);
}
