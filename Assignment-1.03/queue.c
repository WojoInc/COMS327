//
// Created by wojoinc on 2/6/17.
//

#include <stdlib.h>
#include "queue.h"

/**
 * Test method for queue structure. For now, queue is strictly FIFO, but LIFO may be added later if needed.
 * @param argc NA
 * @param argv NA
 * @return NA
 */

int main(int argc, char *argv[]) {
    queue_t *test = createQueue();
    w_unit_t temp = {CORRIDOR, 0, 0, 0};
    queue(test, &temp);
    w_unit_t temp2 = {CORRIDOR, 0, 1, 0};
    queue(test, &temp2);
    w_unit_t temp3 = {CORRIDOR, 0, 2, 0};
    queue(test, &temp3);

    int x;
    while (test->size > 0) {
        printf("%d\n", dequeue(test)->y);
    }
}

queue_t *createQueue(){
    return (queue_t *) malloc(sizeof(queue_t));
}

void queue(queue_t *queue, w_unit_t *w_unit){
    /*
     * Allocate room for the node, this avoids needing to copy the data for each node into the
     * queue, instead each q_node_t->next points to the address of the node in memory
     */
    q_node_t *temp = (q_node_t *) malloc(sizeof(q_node_t));
    temp->w_unit = w_unit;
    if(queue->size == 0){
        queue->first = temp;
        queue->last = temp;
        queue->first->next=NULL;
        queue->first->prev=NULL;
        queue->size++;
    }
    else if(queue->size==1) {
        queue->last = temp;
        queue->first->next = queue->last;
        queue->last->next = NULL;
        queue->last->prev = queue->first;
        queue->size++;
    }
    else {
        q_node_t *lastTemp = queue->last;//grab address of last element in queue
        queue->last = temp;//copy temp into last element of queue
        lastTemp->next = queue->last;//set previous last element's next to the address of new element
        queue->last->next = NULL;//set pointer of last element to null, as there are no elements after it
        queue->last->prev = lastTemp; //set pointer of last element prev to lastTemp
        queue->size++;
    }

}

w_unit_t *dequeue(queue_t *queue){
    if(queue->size == 1){
        w_unit_t *wunit = queue->first->w_unit;
        free(queue->first);
        queue->first = NULL;
        queue->last = NULL;
        queue->size = 0;
        return wunit;
    }
    //grab address of first element in queue
    q_node_t *temp = queue->first;
    queue->first = queue->first->next;
    queue->first->prev = NULL;
    w_unit_t *wunit = temp->w_unit;
    free(temp);
    queue->size--;
    return wunit;
}