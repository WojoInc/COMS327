//
// Created by wojoinc on 3/29/17.
//

#include "dijkstra.h"

void Graph::dijkstra() {
    // array with same num_alloc as dungeon, to keep track of which
    // cells have been considered visited
    //begin with source

    //clean visited, queued, and prev attributes of verticies.
    resetVerticies();
    //setup source vertex.
    source->setVisited(true);
    //create heap
    Heap *heap = new Heap(size);

    //add source to heap
    heap->addWithPriority(source, source->getWeight());
    source->setQueued(true);

    Vertex *cursor;
    while(heap->getSize()!=0){
        cursor = (Vertex*)heap->removeMin();
        cursor->setVisited(true);
        cursor->updateAdjacent(heap);
    }
    delete(heap);
    //print_graph(graph);
}

void Graph::dijkstra_no_rock() {
    // array with same num_alloc as dungeon, to keep track of which
    // cells have been considered visited
    //begin with source

    //clean visited, queued, and prev attributes of verticies.
    resetVerticies();
    //setup source vertex.
    source->setVisited(true);
    //create heap
    Heap *heap = new Heap(size);

    //add source to heap
    heap->addWithPriority(source, source->getWeight());
    source->setQueued(true);

    Vertex *cursor;
    while(heap->getSize()!=0){
        cursor = (Vertex*)heap->removeMin();
        cursor->setVisited(true);
        cursor->updateAdjacentNoRock(heap);
    }
    delete(heap);
    //print_graph(graph);
}

void Graph::resetVerticies(){
    for (int i = 0; i < dungeon->getHeight(); ++i) {
        for (int j = 0; j < dungeon->getWidth(); ++j) {
            verticies[i*dungeon->getWidth() +j].setVisited(false);
            verticies[i*dungeon->getWidth() +j].setQueued(false);
            verticies[i*dungeon->getWidth() +j].setPrev(nullptr);
            verticies[i*dungeon->getWidth() +j].setWeight(0);
        }
    }
}

Graph::Graph(Dungeon *dungeon, Cell *source) {
    this->dungeon=dungeon;
    size = dungeon->getHeight() * dungeon->getWidth();
    verticies = new Vertex[size];

    for (int i = 0; i < dungeon->getHeight(); ++i) {
        for (int j = 0; j < dungeon->getWidth(); ++j) {

            verticies[(i*dungeon->getWidth()) +j].setCell(&dungeon->getCells()[i*dungeon->getWidth()+j]);

            /*assign neighbors to be the 8 units surrounding the vertex.
             * each one is a pointer the neighbors location in
             * graph.verticies, and subsequently, its memory address.
             *
             * NOTE: skip this if the unit is a dungeon border, as neighbors
             * would be out of the range of the array. We also do not care about the borders
             * for the purposes of dijkstra's
             */
            if(dungeon->getCells()[i*dungeon->getWidth()+j].getType()!=IMPASS){//check that unit is not a dungeon border

                //verticies[(i*dungeon->getWidth()) +j].getNeighbors()[] = Vertex[8];

                verticies[(i*dungeon->getWidth()) +j].getNeighbors()[0] = &verticies[(i-1)*dungeon->getWidth() +j-1];
                verticies[(i*dungeon->getWidth()) +j].getNeighbors()[1] = &verticies[(i-1)*dungeon->getWidth() +j];
                verticies[(i*dungeon->getWidth()) +j].getNeighbors()[2] = &verticies[(i-1)*dungeon->getWidth() +j+1];
                verticies[(i*dungeon->getWidth()) +j].getNeighbors()[3] = &verticies[(i)*dungeon->getWidth() +j-1];
                verticies[(i*dungeon->getWidth()) +j].getNeighbors()[4] = &verticies[(i)*dungeon->getWidth() +j+1];
                verticies[(i*dungeon->getWidth()) +j].getNeighbors()[5] = &verticies[(i+1)*dungeon->getWidth() +j-1];
                verticies[(i*dungeon->getWidth()) +j].getNeighbors()[6] = &verticies[(i+1)*dungeon->getWidth() +j];
                verticies[(i*dungeon->getWidth()) +j].getNeighbors()[7] = &verticies[(i+1)*dungeon->getWidth() +j+1];
            }
        }

    }
    this->source = &verticies[source->getY() * dungeon->getWidth() + source->getX()];
}

Graph::~Graph() {
    delete(verticies);
}

void Vertex::updateAdjacent(Heap *heap) {
/*
 * Update each neighbor of the source vertex, if the new weight is less than the current
 * weight, add it to the heap. Assumes heap initially contains only source.
 */
    int temp =0;
    for (int i = 0; i < 8; ++i) {
        if(weight == -1) return;//if unit is a dungeon border
        temp = neighbors[i]->calcWeight();
        if((neighbors[i]->weight == 0 || temp<neighbors[i]->weight)
           && !neighbors[i]->visited) {
            if(!neighbors[i]->queued) {
                neighbors[i]->weight = temp;
                neighbors[i]->prev = this;
                neighbors[i]->queued = true;
                heap->addWithPriority(neighbors[i], neighbors[i]->weight);
            }
        }
    }
}

void Vertex::updateAdjacentNoRock(Heap *heap) {
/*
 * Update each neighbor of the source vertex, if the new weight is less than the current
 * weight, add it to the heap. Assumes heap initially contains only source.
 */
    int temp =0;
    for (int i = 0; i < 8; ++i) {
        if(weight == -1) return;//if unit is a dungeon border or rock in this case

        if(neighbors[i]->cell->getType()==rm_FLOOR||neighbors[i]->cell->getType()==CORRIDOR){
            temp = neighbors[i]->calcWeight();
            if ((neighbors[i]->weight == 0 || temp < neighbors[i]->weight)
                && !neighbors[i]->visited) {
                if (!neighbors[i]->queued) {
                    neighbors[i]->weight = temp;
                    neighbors[i]->prev = this;
                    neighbors[i]->queued = true;
                    heap->addWithPriority(neighbors[i], neighbors[i]->weight);
                }
            }

        }
        else {
            neighbors[i]->weight = -1;
            neighbors[i]->prev = this;
        }
    }
}

int Vertex::calcWeight() {
    if(cell->getHardness()==0) return weight + WEIGHT_1;
    else if(cell->getHardness()>0 && cell->getHardness()<85) return weight + WEIGHT_2;
    else if(cell->getHardness()>=85 && cell->getHardness()<171) return weight + WEIGHT_3;
    else if(cell->getHardness()>=171 && cell->getHardness()<255) return weight + WEIGHT_4;
    else return -1;
}

Vertex::Vertex() {
    neighbors = new Vertex*[8];
}

Vertex::~Vertex() {

}
