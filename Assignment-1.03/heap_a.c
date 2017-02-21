//
// Created by wojoinc on 2/19/17.
//

#include <stdlib.h>
#include "heap_a.h"

/*int main(int argc, char *argv[]){
    heap_t *heap = heap_init(6);
    add_with_priority(heap,1,1);
    add_with_priority(heap,2,15);
    add_with_priority(heap,3,13);
    add_with_priority(heap,4,7);

    printf("%d\n",(int)remove_min(heap));
    printf("%d\n",(int)remove_min(heap));
    add_with_priority(heap,5,4);
    printf("%d\n",(int)remove_min(heap));
    printf("%d\n",(int)remove_min(heap));
    printf("%d\n",(int)remove_min(heap));
    printf("%d\n",(int)remove_min(heap));
}
*/

heap_t *heap_init(size_t size){
    heap_t *heap = malloc(sizeof(heap_t));
    heap->num_alloc = size;
    heap->nodes = (heap_n *)calloc(sizeof(heap_n), size);
    return heap;
}

int add_with_priority(heap_t *heap, void *element, int priority){
    if(get_size(heap)==heap->num_alloc-1){
        printf("%s\n","Attempted to add element to full heap");
        return -1;
    }
    heap->nodes[heap->next].data = element;
    heap->nodes[heap->next].priority = priority+1;

    int cursor = heap->next;
    while(cursor>0
          && heap->nodes[cursor/2].priority>priority) {
        swap_nodes(heap, cursor, cursor / 2);
        cursor /= 2;
    }
    heap->next++;
    return 0;
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

int get_size(heap_t *heap){
    return heap->next;
}

int peek_min(heap_t *heap){
    return heap->nodes[0].priority-1;
}

void * peek_min_data(heap_t *heap){
    return &heap->nodes[0].data;
}

void * remove_min(heap_t *heap){
    if(get_size(heap)==0){
        printf("%s\n","Attempted to remove element from empty heap");
        return NULL;
    }
    void * min = heap->nodes[0].data;
    heap->next--;
    heap->nodes[0].data = heap->nodes[heap->next].data;
    heap->nodes->priority = heap->nodes[heap->next].priority;

    heap->nodes[heap->next].data = NULL;
    heap->nodes[heap->next].priority = 0;

    int cursor = 0, left = 0,right = 0, priority = heap->nodes[0].priority;
    bool terminate = FALSE;
    while(cursor*2+2<heap->num_alloc && terminate==FALSE){

        left = heap->nodes[cursor*2+1].priority;
        right = heap->nodes[cursor*2+2].priority;

        /*check if child nodes are empty, make their calculated priority higher than
         * priority of cursor to avoid swapping with null element.
         */
        if(left==0) left = priority+1;
        if(right==0) right = priority+1;

        //continue checking which child has lower priority
        if(left < priority ||
                right < priority){
            if((left&&right)<priority){//both children have lower priority

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