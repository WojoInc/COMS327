//
// Created by 97wes on 1/21/2017.
//

#include "main.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]){

    //dungeon variable, an struct storing the weights of each square in the dungeon
    dungeon_t dungeon= {{0,0,0,0},{ROCK,0}};

    setBoundaries(&dungeon);
    //printDungeon(&dungeon);
    placeRooms(&dungeon);
    printDungeon(&dungeon);


}

void applyRoom(room_t *room, dungeon_t *dungeon){
    for (int i = 0; i < room->height; ++i) {
        for (int j = 0; j < room->width; ++j) {
            dungeon->wunits[room->y+i][room->x+j].type = rm_FLOOR;
        }
    }
}

bool checkRoom(room_t *room, dungeon_t *dungeon){

    //check to make sure there is a 1 world unit gap around the area the room would cover
    for (int i = -1; i <= room->height; ++i) {
        //check each row verify world units are either ROCK or a CORRIDOR
        for (int j = -1; j < room->width; ++j) {
            if(dungeon->wunits[room->y+i][room->x+j].type!=ROCK) return false;
        }
    }
    return true;
}

void placeRooms(dungeon_t *dungeon){

    room_t temp;
    int tries;
    //TODO find better method for randomly choosing a number from a range
    for (int count = 0; count < NUM_ROOMS; ++count) {
        //printDungeon(dungeon);
        temp.y = (rand()%d_HEIGHT-1)+1;
        temp.x = (rand()%d_WIDTH-1)+1;
        temp.height = (rand()%r_MIN_H-1)+7;
        temp.width = (rand()%r_MIN_W-1)+7;

        tries = 0;

        while (checkRoom(&temp, dungeon) == false && tries < MAX_TRIES) {
            temp.y = (rand() % d_HEIGHT - 1) + 1;
            temp.x = (rand() % d_WIDTH - 1) + 1;
            temp.height = (rand() % r_MIN_H - 1) + 7;
            temp.width = (rand() % r_MIN_W - 1) + 7;
            tries++;
        }
        if (tries < MAX_TRIES) { //meaning we did not time out, and room is valid
            dungeon->rooms[count] = temp;
            applyRoom(&temp,dungeon);
        }
    }
}

void setBoundaries(dungeon_t *dungeon){
    //sides
    for (int i = 0; i < d_HEIGHT; ++i) {
        dungeon->wunits[i][0].type = IMPASS;
        dungeon->wunits[i][d_WIDTH-1].type = IMPASS;
    }
    //top and bottom
    for (int j = 0; j < d_WIDTH; ++j) {
        dungeon->wunits[0][j].type = IMPASS;
        dungeon->wunits[d_HEIGHT-1][j].type = IMPASS;
    }
}

void printDungeon(dungeon_t *dungeon){

    for (int i = 0; i < d_HEIGHT; ++i) {
        for (int j = 0; j < d_WIDTH; ++j) {

            switch(dungeon->wunits[i][j].type){

                case IMPASS:
                    printf("%c",'$');
                    break;
                case CORRIDOR:
                    printf("%c",'#');
                    break;
                case rm_FLOOR:
                    printf("%c",'-');
                    break;
                case ROCK:
                    printf("%c",' ');
                    break;
                default:
                    printf("%c",'x');//just to make it easier to spot errors
                    break;
            }
        }
        printf("%c",'\n');
    }
}