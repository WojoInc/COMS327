//
// Created by 97wes on 1/27/2017.
//

#include "dungeon.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//TODO convert all dungeon generation to make use of dynamic sizes, eliminate static definitions in dungeon.h
dungeon_t *generateDungeon(){
//TODO set world unit x and y to its respective position in the dungeon
    //dungeon variable, an struct storing the weights of each square in the dungeon
    dungeon_t *dungeon = malloc(sizeof(dungeon_t));
    dungeon->rooms = calloc(sizeof(room_t),NUM_ROOMS);

    randHardness(dungeon);
    setBoundaries(dungeon);
    //printDungeon(&dungeon);
    placeRooms(dungeon);
    drawCorridors(dungeon);

    return dungeon;
}

dungeon_t generateDungeon_d(int numRooms){

    //dungeon variable, an struct storing the weights of each square in the dungeon
    dungeon_t *dungeon;
    dungeon = malloc(sizeof(dungeon_t));
    dungeon->rooms = calloc(sizeof(room_t),(size_t)numRooms);

    randHardness(dungeon);
    setBoundaries(dungeon);
    //printDungeon(&dungeon);
    placeRooms(dungeon);
    drawCorridors(dungeon);

    return *dungeon;
}

void randHardness(dungeon_t *dungeon){
    srand(time(NULL));
    for (int i = 0; i < d_HEIGHT; ++i) {
        for (int j = 0; j < d_WIDTH; ++j) {
            dungeon->wunits[i][j].hardness=(rand() % 253)+1;
            dungeon->wunits[i][j].y=i;
            dungeon->wunits[i][j].x=j;
        }
    }

}

void applyRoom(room_t *room, dungeon_t *dungeon){
    for (int i = 0; i < room->height; ++i) {
        for (int j = 0; j < room->width; ++j) {
            dungeon->wunits[room->y+i][room->x+j].type = rm_FLOOR;
            dungeon->wunits[room->y+i][room->x+j].hardness = 0;
        }
    }
}

bool checkRoom(room_t *room, dungeon_t *dungeon){

    if((room->x+room->width)>=d_WIDTH) return FALSE;
    if((room->y+room->height)>=d_HEIGHT) return FALSE;
    //check to make sure there is a 1 world unit gap around the area the room would cover
    for (int i = -1; i <= room->height; ++i) {
        //check each row verify world units are either ROCK
        for (int j = -1; j < room->width; ++j) {
            if(dungeon->wunits[room->y+i][room->x+j].type!=ROCK) return FALSE;
        }
    }
    return TRUE;
}

void placeRooms(dungeon_t *dungeon){

    srand((unsigned int) time(NULL));
    room_t temp;
    int tries;

    for (int count = 0; count < NUM_ROOMS; ++count) {
        //printDungeon(dungeon);
        temp.y = (rand()%d_HEIGHT)+1;
        temp.x = (rand()%d_WIDTH)+1;
        temp.height = (rand()%r_MIN_H-1)+10;
        temp.width = (rand()%r_MIN_W-1)+10;

        tries = 0;

        while (checkRoom(&temp, dungeon) == FALSE && tries < MAX_TRIES) {
            temp.y = (rand() % d_HEIGHT) + 1;
            temp.x = (rand() % d_WIDTH) + 1;
            temp.height = (rand() % r_MIN_H) + 7;
            temp.width = (rand() % r_MIN_W) + 7;
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
        dungeon->wunits[i][0].hardness = 255;
        dungeon->wunits[i][d_WIDTH-1].hardness = 255;
    }
    //top and bottom
    for (int j = 0; j < d_WIDTH; ++j) {
        dungeon->wunits[0][j].type = IMPASS;
        dungeon->wunits[d_HEIGHT-1][j].type = IMPASS;
        dungeon->wunits[0][j].hardness = 255;
        dungeon->wunits[d_HEIGHT-1][j].hardness = 255;
    }
}

void printDungeon(dungeon_t *dungeon){

    for (int i = 0; i < d_HEIGHT; ++i) {
        for (int j = 0; j < d_WIDTH; ++j) {

            switch(dungeon->wunits[i][j].type){

                case IMPASS:
                    printf("%c",' ');
                    break;
                case CORRIDOR:
                    printf("%c",'#');
                    break;
                case rm_FLOOR:
                    printf("%c",'.');
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

int writeDungeon(dungeon_t *dungeon, FILE *f){
    int bytesWritten=0;
    char *NULL_BYTE = 0x00;
    int toWrite=0;
/*    for (int i = 0; i < d_HEIGHT; ++i) {
        for (int j = 0; j < d_WIDTH; ++j) {
            if(dungeon->wunits[i][j].type==(rm_FLOOR||CORRIDOR)){
                fwrite(&NULL_BYTE,sizeof(char),1,f);
                bytesWritten++;
            }
            else{
                toWrite = (uint8_t)htobe32((unsigned int) dungeon->wunits[i][j].hardness);
                fwrite(&toWrite,sizeof(char),1,f);
                bytesWritten++;
            }
        }
    }*/
    for (int i = 0; i < d_HEIGHT; ++i) {
        for (int j = 0; j < d_WIDTH; ++j) {
            if(dungeon->wunits[i][j].type==(rm_FLOOR||CORRIDOR)){
                fwrite(&NULL_BYTE,sizeof(char),1,f);
                bytesWritten++;
            }
            else{
                toWrite = dungeon->wunits[i][j].hardness;
                fwrite(&toWrite,sizeof(char),1,f);
                bytesWritten++;
            }
        }
    }
    return bytesWritten;
}

int writeRooms(dungeon_t *dungeon, FILE *f){
    int bytesWritten=0;
    int x,y,height,width;
    for (int i = 0; i < NUM_ROOMS; ++i) {
        /*x = htobe32((unsigned int)dungeon->rooms[i].x);
        y = htobe32((unsigned int)dungeon->rooms[i].y);
        width = htobe32((unsigned int)dungeon->rooms[i].width);
        height = htobe32((unsigned int)dungeon->rooms[i].height);*/
        x = dungeon->rooms[i].x;
        y = dungeon->rooms[i].y;
        width = dungeon->rooms[i].width;
        height = dungeon->rooms[i].height;

        fwrite(&dungeon->rooms[i].x,sizeof(char),1,f);
        fwrite(&dungeon->rooms[i].y,sizeof(char),1,f);
        fwrite(&dungeon->rooms[i].width,sizeof(char),1,f);
        fwrite(&dungeon->rooms[i].height,sizeof(char),1,f);
        bytesWritten += 4;
    }
    return bytesWritten;
}

void drawCorridors(dungeon_t *dungeon){

    int x1, x2, y1, y2;
    int leftRight;
    int upDown;
    room_t *temp;
    for (int i = 0; i < NUM_ROOMS; ++i) {
        getCentroid(&dungeon->rooms[i],&y1,&x1);
        temp = getClosestRoom(&dungeon->rooms[i],dungeon->rooms);
        getCentroid(temp,&y2,&x2);

        if(x1<x2) leftRight = -1;
        else if(x1>x2) leftRight = 1;
        else leftRight = 0;

        if(y1<y2){
            upDown = -1;
        }
        else if(y1>y2) upDown = 1;
        else upDown = 0;

        //traverse the y direction, using x2 as the x coordinate
        for (int j = y2; j!=y1; j+=upDown) {
            /*
             * Check if the world unit is a ROCK, if so, make it a CORRIDOR.
             * Otherwise, it is a rm_FLOOR or a CORRIDOR already.
             */
            if(dungeon->wunits[j][x2].type==ROCK){
                dungeon->wunits[j][x2].type = CORRIDOR;
                dungeon->wunits[j][x2].hardness = 0;
            }
        }

        for (int k = x2; k != x1; k+=leftRight) {
            if(dungeon->wunits[y1][k].type==ROCK){
                dungeon->wunits[y1][k].type = CORRIDOR;
                dungeon->wunits[y1][k].hardness = 0;
            }
        }
    }
}

room_t *getClosestRoom(room_t *room, room_t rooms[]){
    //begin by assuming first room is closest
    room_t *temp = &rooms[0];
    //perform a sequential search on the rooms, updating the temp pointer if any rooms are found to be closer.
    for (int i = 1; i < NUM_ROOMS; ++i) {
        if(rooms[i].x==0){ //meaning that the room is not yet initialized
            break;
        }
        else{
            if(compareDistanceCtrd(room,temp,&rooms[i])>0){
                temp = &rooms[i];
            }
        }
    }
    return temp;
}

void getCentroid(room_t *room, int *y, int *x){
    //may be off by 1 due to integer division, but good enough for this purpose
    *y = room->y+ room->height/2;
    *x = room->x+ room->width/2;
}

int compareDistanceCtrd(room_t *ref, room_t *room1, room_t * room2){

    int p1Res = dotProduct(ref->y,ref->x,room1->y,room1->x,room2->y,room2->x);
    int p2Res = dotProduct(ref->y,ref->x,room2->y,room2->x,room1->y,room1->x);

    if (p1Res == p2Res)return 0;
    else if (p1Res < p2Res)return -1;
    else return 1;
}

int compareDistance(room_t *ref, room_t *room1, room_t *room2)
{
    int p1Res = dotProduct(ref->y,ref->x,room1->y,room1->x,room2->y,room2->x);
    int p2Res = dotProduct(ref->y,ref->x,room2->y,room2->x,room1->y,room1->x);

    if (p1Res == p2Res)return 0;
    else if (p1Res < p2Res)return -1;
    else return 1;
}

int dotProduct(int refY, int refX, int y1, int x1, int y2, int x2)
{
    int xProduct = (x1-refX) * (x2-refX);
    int yProduct = (y1-refY) * (y2-refY);

    return xProduct+yProduct;
}