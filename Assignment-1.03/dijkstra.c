//
// Created by wojoinc on 2/5/17.
//

#include <stdlib.h>
#include "dijkstra.h"
#include "queue.h"

int main(int argc, char *argv[]){
    dungeon_t *dungeon = generateDungeon();
    dijkstra(dungeon,&dungeon->wunits[50][50]);
}
int calcDist(int weight, w_unit_t* w_unit){
//TODO handle what happens to the weight when a neighbor is the dungeon edge
    if(w_unit->hardness==0) return weight + WEIGHT_1;
    else if(w_unit->hardness>0 && w_unit->hardness<85) return weight + WEIGHT_2;
    else if(w_unit->hardness>=85 && w_unit->hardness<171) return weight + WEIGHT_3;
    else if(w_unit->hardness>=171 && w_unit->hardness<255) return weight + WEIGHT_4;
    else return -1;
}

void dijkstra(dungeon_t *dungeon, w_unit_t *source){
//TODO have this function return a map, and create another function which takes a map, and creates a queue of world units for the monster path
    // array with same size as dungeon, to keep track of which
    // cells have been considered visited
    //begin with source

    bool visited[d_HEIGHT][d_WIDTH];
    int weight[d_HEIGHT][d_WIDTH];
    for (int i = 0; i < d_HEIGHT; ++i) {
        for (int j = 0; j < d_WIDTH; ++j) {
            weight[i][j] = INT32_MAX;
        }
    }
    //set source weight to 0
    weight[source->y][source->x] = 0;
    visited[source->y][source->x] =true;
    w_unit_t *nextUnit =NULL;

    //updateAdjacent(dungeon,source,weight,visited);
    nextUnit = getLightestNeighbor(dungeon, source, weight, visited);
    for (int i = 0; i < 50; ++i) {
        nextUnit = getLightestNeighbor(dungeon, nextUnit, weight, visited);
        visited[nextUnit->y][nextUnit->x] = true;
        updateAdjacent(dungeon, nextUnit, weight, visited);
    }
    for (int k = 0; k < d_HEIGHT; ++k) {
        for (int i = 0; i < d_WIDTH; ++i) {
            printf("%d",weight[k][i]);
        }
        printf("\n");
    }
}

void updateAdjacent(dungeon_t *dungeon, w_unit_t *source, int weight[d_HEIGHT][d_WIDTH], bool visited[d_HEIGHT][d_WIDTH]){
    /*
     * Update weights of world units adjacent to the source unit,
     * ignore
     */
    int y = source->y;
    int x = source->x;
    int temp;
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            if(!(i==0 && j==0)) {
                temp = calcDist(weight[y][x], &dungeon->wunits[y + i][x + j]);
                /*
                 * Check to make sure that the wunit being updated is not a dungeon border
                 * Also check to make sure it is not in the visited array
                 * Then if calculated distance between this node
                 * and the neighbor is less, update the weight.
                 */
                if (dungeon->wunits[y + i][x + j].type != IMPASS
                    && temp < weight[y + i][x + j] && visited[y + i][x + j] == false) {
                    weight[y + i][x + j] = temp;
                }
            }
        }

    }
}
w_unit_t *getLightestNeighbor(dungeon_t *dungeon, w_unit_t *source, int weight[d_HEIGHT][d_WIDTH], bool visited[d_HEIGHT][d_WIDTH]){
    /*
     * Only check the four world units surrounding the source,
     * as I don't believe that the character or monsters are able to move
     * diagonally.
     */

    int y = source->y;
    int x = source->x;
    int unitWeight =INT32_MAX,temp;
    w_unit_t *neighbor=NULL;

    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            if (!(i == 0 && j == 0)) {
                temp = calcDist(weight[y][x], &dungeon->wunits[y + i][x + j]);
                if (temp < unitWeight && visited[y+i][x+j]==false) {
                    unitWeight = temp;
                    weight[y + i][x + j] = unitWeight;
                    neighbor = &dungeon->wunits[y + i][x + j];
                }
            }
        }
    }
    return neighbor;
}