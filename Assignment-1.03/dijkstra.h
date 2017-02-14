//
// Created by wojoinc on 2/5/17.
//

#ifndef COMS327_DIJKSTRA_H
#define COMS327_DIJKSTRA_H

#include "dungeon.h"

#define WEIGHT_1 1
#define WEIGHT_2 1
#define WEIGHT_3 2
#define WEIGHT_4 3

typedef struct vertex{
    w_unit_t *w_unit;
    int weight;
    struct vertex *prev;
}vertex_t;

typedef struct graph{
    int size;
    vertex_t *verticies[];
}graph_t;

void dijkstra(dungeon_t *dungeon, w_unit_t *source);
void updateAdjacent(dungeon_t *dungeon, w_unit_t *source, int weight[d_HEIGHT][d_WIDTH], bool visited[d_HEIGHT][d_WIDTH]);
w_unit_t *getLightestNeighbor(dungeon_t *dungeon, w_unit_t *source, int weight[d_HEIGHT][d_WIDTH], bool visited[d_HEIGHT][d_WIDTH]);
#endif //COMS327_DIJKSTRA_H
