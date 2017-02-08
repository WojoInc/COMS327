//
// Created by wojoinc on 2/5/17.
//

#include <stdlib.h>
#include "dijkstra.h"
#include "queue.h"

void calcAdjacent(w_unit_t *wunit, int weight){
    for (int i = -1; i < 3; i+=2) {
        calcDist()
    }
    wunit->x;
}

int calcDist(int weight, w_unit_t* w_unit){
//TODO handle what happens to the weight when a neighbor is the dungeon edge
    if(w_unit->hardness==0) return weight + WEIGHT_1;
    else if(w_unit->hardness>0 && w_unit->hardness<85) return weight + WEIGHT_2;
    else if(w_unit->hardness>=85 && w_unit->hardness<171) return weight + WEIGHT_3;
    else if(w_unit->hardness>=171 && w_unit->hardness<255) return weight + WEIGHT_4;
    else return -1;
    //TODO make function consider the current weight of the world unit
}

queue_t *dijkstra(dungeon_t *dungeon, w_unit_t *source, w_unit_t *dest){
//TODO have this function return a map, and create another function which takes a map, and creates a queue of world units for the monster path
    // array with same size as dungeon, to keep track of which
    // cells have been considered visited
    //begin with source

    bool visited[d_HEIGHT][d_WIDTH] = {false};
    int weight[d_HEIGHT][d_WIDTH] = {INT32_MAX};
    //set source weight to 0
    weight[source->y][source->x] = 0;
    visited[source->y][source->x] =true;
    w_unit_t *nextUnit =NULL;

    for (int i = 0; i < d_HEIGHT; ++i) {
        for (int j = 0; j < d_WIDTH; ++j) {
            nextUnit = getLightestNeighbor(dungeon,source,weight);
            visited[nextUnit->y][nextUnit->x] = true;

        }
    }

}

void updateAdjacent(dungeon_t *dungeon, w_unit_t *source, int weight[d_HEIGHT][d_WIDTH]){
    //TODO update adjacent world unit weights
    /*
     * Update weights of world units adjacent to the source unit,
     * ignore
     */
}
w_unit_t *getLightestNeighbor(dungeon_t *dungeon, w_unit_t *source, int weight[d_HEIGHT][d_WIDTH]){
    /*
     * Only check the four world units surrounding the source,
     * as I don't believe that the character or monsters are able to move
     * diagonally.
     */

    int y = source->y;
    int x = source->x;
    int unitWeight =INT32_MAX,temp;
    w_unit_t *neighbor=NULL;
    for (int i = -1; i < 2; i+=2) {
        temp = calcDist(weight[y][x],&dungeon->wunits[y+i][x]);
        if(temp < unitWeight) {
            unitWeight = temp;
            weight[y+i][x] = unitWeight;
            neighbor = &dungeon->wunits[y+i][x];
        }
    }
    for (int i = -1; i < 2; i+=2) {
        temp = calcDist(weight[y][x],&dungeon->wunits[y][x+i]);
        if(temp < unitWeight) {
            unitWeight = temp;
            weight[y+i][x] = unitWeight;
            neighbor = &dungeon->wunits[y][x+i];
        }
    }
    return neighbor;
}