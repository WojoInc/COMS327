//
// Created by wojoinc on 2/5/17.
//

#ifndef COMS327_DIJKSTRA_H
#define COMS327_DIJKSTRA_H

#include "dungeon.h"
#include "heap_a.h"

#define WEIGHT_1 1
#define WEIGHT_2 1
#define WEIGHT_3 2
#define WEIGHT_4 3

typedef struct vertex{
    int weight;
    bool visited;
    bool queued;
    w_unit_t *w_unit;
    struct vertex *prev;
    struct vertex **neighbors;
}vertex_t;

typedef struct graph{
    int size;
    dungeon_t *dungeon;
    vertex_t *source;
    vertex_t *verticies;
}graph_t;

void dijkstra(graph_t *graph);
graph_t *create_graph_dungeon(dungeon_t *dungeon, w_unit_t *source);
void print_graph(graph_t *graph);
void update_adjacent(heap_t *heap, vertex_t *source);
int w_unit_weight(int weight, w_unit_t *w_unit);
#endif //COMS327_DIJKSTRA_H
