//
// Created by 97wes on 1/21/2017.
//

#include "main.h"
#include <stdio.h>
#include <unistd.h>


int main(int argc, char *argv[]){

    //dungeon variable, an struct storing the weights of each square in the dungeon
    dungeon_t dungeon;
    //array representing rooms present in the dungeon, for now i1 = height, i2 = width
    int rooms[10][10];
    setBoundaries(dungeon.wunits);
    for (int i = 0; i < 10; ++i) {

    }
    printDungeon(dungeon.wunits);


}

/**
 * Sets the weight of the cells at the edges of the dungeon to IMPASS, effectively
 * creating boundaries which rooms, corridors, the PC, and NPCs may not pass or rest on.
 * @param dungeon the dungeon to draw boundaries on.
 */
void setBoundaries(wunit_state_t dungeon[d_HEIGHT][d_WIDTH]){
    //sides
    for (int i = 0; i < d_HEIGHT; ++i) {
        dungeon[i][0] = IMPASS;
        dungeon[i][d_WIDTH-1] = IMPASS;
    }
    //top and bottom
    for (int j = 0; j < d_WIDTH; ++j) {
        dungeon[0][j] = IMPASS;
        dungeon[d_HEIGHT-1][j] = IMPASS;
    }
}

/**
 * Prints the dungeon via stdOut to the console
 * @param dungeon the dungeon to be displayed
 */
void printDungeon(wunit_state_t dungeon[d_HEIGHT][d_WIDTH]){
    for (int i = 0; i < d_HEIGHT; ++i) {
        for (int j = 0; j < d_WIDTH; ++j) {

            switch(dungeon[i][j]){

                case IMPASS:
                    printf("%c",'$');
                    break;
                case CORRIDOR:
                    printf("%c",'#');
                    break;
                case rm_FLOOR:
                    printf("%c",'-');
                case ROCK:
                    printf("%c",' ');
                default:
                    printf("%c",' ');
            }
        }
        printf("%c",'\n');
    }
}