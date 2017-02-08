//
// Created by wojoinc on 2/5/17.
//

#ifndef COMS327_DIJKSTRA_H
#define COMS327_DIJKSTRA_H

#include "dungeon.h"

#define RANGE1 0

typedef struct vertex{
    struct vertex *neighbors[];
    int distance;
    struct vertex *prev;
}vertex_t;

typedef struct graph{
    int size;
    vertex_t *verticies[];
}graph_t;
#endif //COMS327_DIJKSTRA_H
