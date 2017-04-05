//
// Created by wojoinc on 3/29/17.
//

#ifndef COMS327_HEAP_H
#define COMS327_HEAP_H

class HeapNode{
    int priority;
    void *data;

public:
    int getPriority() const {
        return priority;
    }

    void setPriority(int priority) {
        HeapNode::priority = priority;
    }

    void *getData() const {
        return data;
    }

    void setData(void *data) {
        HeapNode::data = data;
    }
};

class Heap {

    int num_alloc;
    int next;
    HeapNode *nodes;

public:
    Heap(int size);
    ~Heap();

    int getNum_alloc() const {
        return num_alloc;
    }

    int getNext() const {
        return next;
    }

    void swapNodes(int node_a, int node_b);

    int addWithPriority(void *element, int priority);

    int getSize();

    int peekMin();

    void * peekMinData();

    void * removeMin();
};


#endif //COMS327_HEAP_H
