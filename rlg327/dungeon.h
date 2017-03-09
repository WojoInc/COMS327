//
// Created by 97wes on 1/27/2017.
//

#ifndef COMS327_DUNGEON_H
#define COMS327_DUNGEON_H

#include <stdint.h>
#include <stdio.h>
#include "proj_incl.h"

#define d_HEIGHT 105
#define d_WIDTH 160

#define r_MIN_W 7
#define r_MIN_H 5

#define NUM_ROOMS 15
#define NUM_STAIRS 2
#define MAX_TRIES 2000

#define IMPASS 256
#define ROCK ' '
#define CORRIDOR '#'
#define rm_FLOOR '.'
#define STAIR_UP '<'
#define STAIR_DOWN '>'

typedef struct w_unit{
    int type;
    int x;
    int y;
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
    w_unit_t wunits[d_HEIGHT][d_WIDTH];
    room_t *rooms;
}dungeon_t;

void place_stairs(dungeon_t *dungeon);
/**
 * Generates a new dungeon.
 *
 * @return value of new dungeon
 */
dungeon_t *generateDungeon();

/**
 * Generates a new dungeon with number of rooms given by @numRooms.
 *
 * Can also be used to allocate space when reading dungeon from file.
 *
 * ***To be deprecated later****
 * @param numRooms number of rooms to allocate space for
 * @return value of new dungeon
 */
dungeon_t generateDungeon_d(int numRooms);

/**
 * Randomizes the hardness of each world unit in @dungeon
 *
 * @param dungeon dungeon to randomize
 */
void apply_properties(dungeon_t *dungeon);

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
 * *************DEPRECATED***************
 * Prints the dungeon via stdOut to the console
 * @param dungeon the dungeon to be displayed
 */
void printDungeon(dungeon_t *dungeon);

/**
 * Writes contents of @dungeon to file @f.
 * Writes only the values of the hardness of the world units.
 *
 * @param dungeon the dungeon to be written
 * @param f the save file to write to
 * @return number of bytes written
 */
int writeDungeon(dungeon_t *dungeon, FILE *f);

/**
 * Writes the location data for each room in @dungeon to @f.
 *
 * @param dungeon dungeon to load rooms from
 * @param f dungeon save file
 * @return number of bytes written
 */
int writeRooms(dungeon_t *dungeon, FILE *f);

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
#endif //COMS327_DUNGEON_H
