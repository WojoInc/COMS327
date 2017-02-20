//
// Created by wojoinc on 2/19/17.
//

#include <stdlib.h>
#include "heap_a.h"

//TODO add error trapping
heap_t *heap_init(size_t size){
    heap_t *heap = malloc(sizeof(heap_t));
    heap->size = size;
    heap->nodes = (heap_n *)calloc(sizeof(heap_n), size);
    return heap;
}


void add_with_priority(heap_t *heap, void *element, int priority){
    heap->nodes[heap->next].data = element;
    heap->nodes[heap->next].priority = priority;

    int cursor = heap->next;
    while(cursor>0 && !heap->nodes[cursor/2].isEmpty
          && heap->nodes[cursor/2].priority>priority) {
        swap_nodes(heap, cursor, cursor / 2);
        cursor /= 2;
    }
    heap->next++;
}

void swap_nodes(heap_t *heap, int node_a, int node_b) {
    //store values in temp variables
    void *data = heap->nodes[node_a].data;
    int priority = heap->nodes[node_a].priority;
    //move node b contents to a
    heap->nodes[node_a].data = heap->nodes[node_b].data;
    heap->nodes[node_a].priority = heap->nodes[node_b].priority;
    //move temp variables to b
    heap->nodes[node_b].data = data;
    heap->nodes[node_b].priority = priority;
}


int peek_min(heap_t *heap){
    return heap->nodes[0].priority-1;
}

void * peek_min_data(heap_t *heap){
    return &heap->nodes[0].data;
}

void * remove_min(heap_t *heap){
    void * min = heap->nodes[0].data;
    heap->nodes[0].data = heap->nodes[heap->next-1].data;
    heap->nodes->priority = heap->nodes[heap->next-1].priority;

    heap->nodes[heap->next-1].data = NULL;
    heap->nodes[heap->next-1].priority = 0;
    heap->nodes[heap->next-1].isEmpty = TRUE;

    int cursor = 0, left = 0,right = 0, priority = heap->nodes[0].priority;
    bool terminate = FALSE;
    while(cursor*2+2<heap->size && terminate==FALSE){

        left = heap->nodes[cursor*2+1].priority;
        right = heap->nodes[cursor*2+2].priority;
        if(left > priority ||
                right > priority){
            if((left&&right)>priority){//both children have lower priority

                if(left<=right){//if left and right are the same or left is lower, swap with left
                    swap_nodes(heap,cursor*2+1,cursor);
                    cursor = cursor*2+1;
                }
                else{//swap with right
                    swap_nodes(heap,cursor*2+2,cursor);
                    cursor = cursor*2+2;
                }
            }
            else if(left<right){
                swap_nodes(heap,cursor*2+1,cursor);
                cursor = cursor*2+1;
            }
            else{
                swap_nodes(heap,cursor*2+2,cursor);
                cursor = cursor*2+2;
            }
        }
        else{//neither child has lower priority. STOP
            terminate = TRUE;
        }
    }

    return min;
}