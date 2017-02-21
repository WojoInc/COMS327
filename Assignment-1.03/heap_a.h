//
// Created by wojoinc on 2/19/17.
//

#ifndef COMS327_HEAP_A_H
#define COMS327_HEAP_A_H

#include <wchar.h>
#include "proj_incl.h"

typedef struct heap_node{
    int priority;
    void *data;
}heap_n;

typedef struct heap{
    size_t num_alloc;
    int next;
    heap_n *nodes;
}heap_t;

heap_t *heap_init(size_t size);

void swap_nodes(heap_t *heap, int node_a, int node_b);

int add_with_priority(heap_t *heap, void *element, int priority);

int get_size(heap_t *heap);

int peek_min(heap_t *heap);

void * peek_min_data(heap_t *heap);

void * remove_min(heap_t *heap);
#endif //COMS327_HEAP_A_H
