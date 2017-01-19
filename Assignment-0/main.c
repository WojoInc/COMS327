#include <stdio.h>
#include <unistd.h>
#include "main.h"

int main(int argc, char *argv[]){

    if(argc<=1) printf("Not enough arguments.\n");
    int xMax = (int) *argv[1]-0x30;
    int yMax = (int) *argv[2]-0x30;

    //xMax = 80;
    //yMax = 24;

    printf("X: %i\nY: %i\n",xMax,yMax);

    char habitat[xMax][yMax];
    for(int i =0; i<yMax; i++){
        for(int j =0; j<xMax; j++){
            habitat[j][i] = ' ';
        }
    }
    for(int i=3; i<argc; i++){
        printf("Pair: (%s,%s)\n",argv[i], argv[i+1]);
        habitat[((int) *argv[i])-0x30][((int) *argv[i])-0x30] = 'X';
        ++i;
    }

    bool alive = true;
    while(alive){
        checkRules(yMax,habitat,xMax);
        displayHabitat(yMax,habitat,xMax);
        usleep(83333);
    }


}

    void checkRules(int yMax, char habitat[][yMax], int xMax){
        for(int i =0; i<yMax; i++){
            for(int j =0; j<xMax; j++){
                int count = 0,Y,X;
                for(int k = -1; k < 2; k++) {
                    for (int l = -1; l < 2; l++) {
                        if((j+k)<0) Y=yMax-1;
                        else if((j+k)==yMax) Y=0;
                        else Y = j+k;
                        if((i+l)<0) X=xMax-1;
                        else if((i+l)==xMax) X=0;
                        else X = i+l;

                        if (habitat[X][Y] == 'X')count++;
                    }
                }
                //If cell is alive and has 0 or 1 neigbors, kill it
                if(count<=1 && habitat[j][i]=='X') habitat[j][i] = ' ';
                else if((count==2 || count==3) && habitat[j][i]=='X') habitat[j][i] = 'X';
                else if(count>=4 && habitat[j][i]=='X') habitat[j][i] = ' ';
                else if(count==3 && habitat[j][i]==' ')habitat[j][i]='X';
            }
        }
    }

void displayHabitat(int yMax, char habitat[][yMax], int xMax){
    printf("%s\n",(char *) habitat);
}