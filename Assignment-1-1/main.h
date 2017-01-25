//
// Created by 97wes on 1/21/2017.
//

#include <stdint.h>

#ifndef COMS327_MAIN_H
#define COMS327_MAIN_H

#endif //COMS327_MAIN_H

#define d_HEIGHT 160
#define d_WIDTH 105

#define r_MIN_W 7
#define r_MIN_H 5

#define NUM_ROOMS 10
#define MAX_TRIES 2000


typedef int bool;
#define true 1
#define false 0

typedef enum wunit_type{
    IMPASS = 255,
    ROCK = 0,
    CORRIDOR = 2,
    rm_FLOOR = 1,
}wunit_type_t;

typedef struct w_unit{
    wunit_type_t type;
    int hardness;
}w_unit_t;

typedef struct room{
    //x and y coordinates representing the top left corner of the room
    int y;
    int x;
    int width;
    int height;
}room_t;

typedef struct dungeon{
    room_t rooms[NUM_ROOMS];
    w_unit_t wunits[d_HEIGHT][d_WIDTH];
}dungeon_t;

//TODO implement functions for connecting rooms

/**
 * Treats the @room as a layer, then flattens this layer onto the dungeon layer. This is done by setting the value
 * of any world unit in the dungeon that is within the area the room encompasses to wunit_state.ROOM.
 * @param room pointer to the room to flatten
 * @param dungeon the dungeon to apply room to
 */
void applyRoom(room_t *room, dungeon_t *dungeon);

/**
 * Verify if room is able to be placed at the desired location in the dungeon
 * @param room the room to check
 * @param dungeon the dungeon to check against
 * @return is room placement valid
 */
bool checkRoom(room_t *room, dungeon_t *dungeon);

/**
 * Place a number of rooms in the dungeon specified by NUM_ROOMS
 * @param dungeon the dungeon for rooms to be placed in to
 */
void placeRooms(dungeon_t *dungeon);

/**
 * Sets the weight of the cells at the edges of the dungeon to IMPASS, effectively
 * creating boundaries which rooms, corridors, the PC, and NPCs may not pass or rest on.
 * @param dungeon the dungeon to draw boundaries on.
 */
void setBoundaries(dungeon_t *dungeon);

/**
 * Prints the dungeon via stdOut to the console
 * @param dungeon the dungeon to be displayed
 */
void printDungeon(dungeon_t *dungeon);