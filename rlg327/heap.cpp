//
// Created by wojoinc on 3/29/17.
//

#include <clocale>
#include "heap.h"

Heap::Heap(int size) {
    num_alloc = size;
    next=0;
    nodes = new HeapNode[num_alloc];
}

Heap::~Heap() {
    delete(nodes);
}

void Heap::swapNodes(int node_a, int node_b) {
    //store values in temp variables
    void *data = nodes[node_a].getData();
    int priority = nodes[node_a].getPriority();
    //move_monster node b contents to a
    nodes[node_a].setData(nodes[node_b].getData());
    nodes[node_a].setPriority(nodes[node_b].getPriority());
    //move_monster temp variables to b
    nodes[node_b].setData(data);
    nodes[node_b].setPriority(priority);
}

int Heap::addWithPriority(void *element, int priority) {
    if(getSize()==num_alloc){
        //printf("%s\n","Attempted to add element to full heap");
        return -1;
    }
    nodes[next].setData(element);
    nodes[next].setPriority(priority+1);

    int cursor = next;
    while(cursor>0
          && nodes[cursor/2].getPriority()>priority) {
        swapNodes(cursor, cursor / 2);
        cursor /= 2;
    }
    next++;
    return 0;
}

int Heap::getSize() {
    return next;
}

int Heap::peekMin() {
    return nodes[0].getPriority()-1;
}

void *Heap::peekMinData() {
    return nodes[0].getData();
}

void *Heap::removeMin() {
    if(getSize()==0){
        //std::printf("%s\n","Attempted to remove element from empty heap");
        return NULL;
    }
    void * min = nodes[0].getData();
    next--;
    nodes[0].setData(nodes[next].getData());
    nodes[0].setPriority(nodes[next].getPriority());

    nodes[next].setData(nullptr);
    nodes[next].setPriority(0);

    int cursor = 0, left = 0,right = 0, priority = nodes[0].getPriority();
    bool terminate = false;
    while(cursor*2+2<num_alloc && !terminate){

        left = nodes[cursor*2+1].getPriority();
        right = nodes[cursor*2+2].getPriority();

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
                    swapNodes(cursor*2+1,cursor);
                    cursor = cursor*2+1;
                }
                else{//swap with right
                    swapNodes(cursor*2+2,cursor);
                    cursor = cursor*2+2;
                }
            }
            else if(left<right){
                swapNodes(cursor*2+1,cursor);
                cursor = cursor*2+1;
            }
            else{
                swapNodes(cursor*2+2,cursor);
                cursor = cursor*2+2;
            }
        }
        else{//neither child has lower priority. STOP
            terminate = true;
        }
    }
    return min;
}
