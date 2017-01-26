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

#define NUM_ROOMS 15
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

/**
 * Draws corridors between each of the rooms
 *
 * @param dungeon dungeon to draw corridors on
 */
void drawCorridors(dungeon_t *dungeon);

/**
 * Compares the distances between two rooms and a single reference room.
 * Compares using the top left corner of each room
 *
 * @param ref the room to use as a reference point
 * @param room1 first room to compare
 * @param room2 second room to compare
 * @return 0 if rooms are equidistant, 1 if room2 is closer, -1 if room1 is closer
 */
int compareDistance(room_t *ref, room_t *room1, room_t *room2);

/**
 * Effectively the same as compareDistance, only using the centroid of each room instead.
 *
 * @param ref the room to use as a reference point
 * @param room1 first room to compare
 * @param room2 second room to compare
 * @return 0 if rooms are equidistant, 1 if room2 is closer, -1 if room1 is closer
 */
int compareDistanceCtrd(room_t *ref, room_t *room1, room_t * room2);

/**
 * Calculates the dot product of two points with respect to a single reference point.
 * Uses integers and no trigonometery to avoid precision loss and needless casting.
 *
 * @param refY reference point Y
 * @param refX reference point X
 * @param y1 point 1 Y
 * @param x1 point 1 X
 * @param y2 point 2 Y
 * @param x2 point 2 X
 * @return the resulting dot product
 */
int dotProduct(int refY, int refX, int y1, int x1, int y2, int x2);

/**
 * Returns the centroid of a room. Uses integer division, so may be +/- 1 world unit
 * from the true center, but it should be accurate enough for this purpose
 *
 * @param room the room to find the centroid of
 * @param y the pointer to store y value to
 * @param x the pointer to store x value to
 */
void getCentroid(room_t *room, int *y, int *x);

/**
 * Returns the closest room from an array of rooms, with respect to room
 *
 * @param room the reference room
 * @param rooms array of rooms to search
 * @return pointer to closest room
 */
room_t *getClosestRoom(room_t *room, room_t rooms[]);