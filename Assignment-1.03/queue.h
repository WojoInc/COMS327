//
// Created by wojoinc on 2/6/17.
//

#ifndef COMS327_QUEUE_H
#define COMS327_QUEUE_H

#include "dungeon.h"

typedef struct q_node{
    w_unit_t *w_unit;
    struct q_node *next;
    struct q_node *prev;
}q_node_t;

typedef struct queue{
    q_node_t *first;
    q_node_t *last;
    int size;
}queue_t;

queue_t *createQueue();
w_unit_t *dequeue(queue_t *queue);
void queue(queue_t *queue, w_unit_t *w_unit);

#endif //COMS327_QUEUE_H
