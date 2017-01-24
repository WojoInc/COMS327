//
// Created by 97wes on 1/21/2017.
//

#ifndef COMS327_MAIN_H
#define COMS327_MAIN_H

#endif //COMS327_MAIN_H

#define d_HEIGHT 105
#define d_WIDTH 160

#define r_MIN_W 7
#define r_MIN_H 5

#define NUM_ROOMS 10

typedef enum wunit_state{
    IMPASS = 99,
    ROCK = 10,
    CORRIDOR = 5,
    DIAMOND = 4,
    STONE = 3,
    DIRT = 2,
    rm_FLOOR = 98,
}wunit_state_t;
typedef int bool;
#define true 1
#define false 0

typedef struct wunit{
    int state;
}wunit_t;

typedef struct room{
    wunit_t *cells[r_MIN_H][r_MIN_W];
    wunit_t topLeft;
    int width;
    int height;
}room_t;

typedef struct dungeon{
    room_t rooms[NUM_ROOMS];
    wunit_state_t wunits[d_HEIGHT][d_WIDTH];
}dungeon_t;


void setBoundaries(wunit_state_t dungeon[][d_WIDTH]);
void printDungeon(wunit_state_t dungeon[][d_WIDTH]);