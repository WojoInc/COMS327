//
// Created by wojoinc on 2/12/17.
//

#ifndef COMS327_HEAP_H
#define COMS327_HEAP_H

#include "dungeon.h"

typedef struct heap_node{
    void * data;
    int priority;
    struct heap_node *parent;
    struct heap_node **nextSibling;
    struct heap_node *left;
    struct heap_node *right;
}heap_n;

typedef struct heap{
    int size;
    struct heap_node *min;
    struct heap_node **next;
    struct heap_node **last;
}heap_t;
#endif //COMS327_HEAP_H
