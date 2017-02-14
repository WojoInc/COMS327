//
// Created by wojoinc on 2/12/17.
//

#include <stdlib.h>
#include "heap.h"

void swap(heap_n *parent, heap_n *child) {

    void **tempPd = &parent->data;
    int tempPp = parent->priority;
    void **tempCd = &child->data;
    int tempCp = child->priority;

    parent->priority = tempCp;
    parent->data = *tempCd;

    child->priority = tempPp;
    child->data = *tempPd;

}
bool getNextSibling(heap_n *node) {
    if (node->parent->left == node) {//if left child
        node->nextSibling = &node->parent->right;
        return true;
    } else if (node->parent->right == node) {
        if (node->parent->nextSibling != NULL) {
            node->nextSibling = &(*node->parent->nextSibling)->left;
            return true;
        }
    }
    return false;
}
void addWithPriority(heap_t *heap, void * element, int priority){
    //heap is empty
    if(heap->size ==0){
        heap_n *temp = malloc(sizeof(struct heap_node));
        temp->data = element;
        temp->priority = priority;
        temp->left = malloc(sizeof(struct heap_node));
        temp->right = malloc(sizeof(struct heap_node));
        temp->left->parent=temp;
        temp->right->parent=temp;
        temp->parent = NULL;
        temp->prevSibling = NULL;
        heap->min = temp;
        heap->next  = &heap->min->left;
        heap->last = &heap->min;
        heap->size++;

        //setup the first two child nodes to recognize each other
        heap->min->left->nextSibling = &heap->min->right;
    }
    else {
        heap_n *temp = malloc(sizeof(heap_n));
        temp->data = element;
        temp->priority = priority;
        temp->left = malloc(sizeof(heap_n));
        temp->right = malloc(sizeof(heap_n));
        temp->left->parent = temp;
        temp->right->parent = temp;
        temp->prevSibling = heap->last;
        temp->parent = (*heap->next)->parent;
        *heap->next = temp;

        /*
         * figure out which node should be marked as next for insertion
         * also determine temp.nextSibling, and set previous heap.next to heap.last,
         * this keeps track of the last non-NULL node in the tree, and is used when removing a node
         */
        if(getNextSibling(temp)){
            heap->last = heap->next;
            heap->next = temp->nextSibling;
        }
        else {//if no next sibling, next node should be leftmost child.
            heap_n *walker = heap->min;
            while (walker->left != NULL) {
                walker = walker->left;
            }
            heap->last = heap->next;
            heap->next = &walker->parent->left;
        }

        //compare element to parent
        while (temp->parent != NULL && temp->parent->priority > temp->priority) {
            swap(temp->parent, temp);
        }
        heap->size++;
    }
}
//TODO handle detection of empty children to eliminate incorrect swaps
void *removeMin(heap_t *heap){
    void *temp;
    temp = heap->min->data;

    /*
     * Grab last node in tree, and bring it to the root. Then, while there is
     * at least one child with lower priority than this node, swap node with smallest
     * child. This allows the heap to decide which child is next smallest and should be
     * moved to the root after removal.
     */
    heap->min->data = (*heap->last)->data;
    heap->min->priority = (*heap->last)->priority;

    heap_n *walker = heap->min;
    while(!(walker->left==NULL && walker->right==NULL) && !(walker->left->priority==0 && walker->right->priority==0) &&
            (walker->left->priority<walker->priority||walker->right->priority<walker->priority)){

        //if both left and right child have lower priorities than walker node, pick the smaller of the two
        if(walker->left->priority<walker->priority&&walker->right->priority<walker->priority){

            if(walker->left->priority>=walker->right->priority){//if right child has lower priority
                swap(walker,walker->right);
                walker = walker->right;
            }
            else{
                swap(walker,walker->left);
                walker = walker->left;
            }
        }
            //only one of the children has lower priority
        else if(walker->left->priority<walker->priority){
            swap(walker,walker->left);
            walker = walker->left;
        }
        else{
            swap(walker,walker->right);
            walker = walker->right;
        }
    }

    /*
     * set heap.next to removed node's position
     * set heap.last to the new most recently added node.
     */
    heap->next = heap->last;

    //effectively remove the node from the tree, keeping its allocated memory for now.
    (*heap->last)->priority=0;
    (*heap->last)->data = NULL;
    heap->last = (*heap->last)->prevSibling;
    heap->size--;
    return temp;
}

int main(int argc, char *argv[]){
    heap_t * heap = malloc(sizeof(heap_t));
    heap->size = 0;
    w_unit_t test = {rm_FLOOR,0,1,1};
    w_unit_t test2 = {rm_FLOOR,0,1,2};
    w_unit_t test3 = {rm_FLOOR,0,1,4};
    w_unit_t test4 = {rm_FLOOR,0,1,4};
    w_unit_t test5 = {rm_FLOOR,0,1,4};
    w_unit_t test6 = {rm_FLOOR,0,1,4};
    addWithPriority(heap,&test, 1);
    addWithPriority(heap,&test2, 3);
    addWithPriority(heap,&test3, 2);
    addWithPriority(heap,&test4, 4);
    addWithPriority(heap,&test5, 8);
    addWithPriority(heap,&test6, 9);
    printf("%d\n",((w_unit_t*)removeMin(heap))->hardness);
    printf("%d\n",((w_unit_t*)removeMin(heap))->hardness);
    printf("%d\n",((w_unit_t*)removeMin(heap))->hardness);
    int x;
}