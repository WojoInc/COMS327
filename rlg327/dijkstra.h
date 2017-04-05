//
// Created by wojoinc on 3/29/17.
//

#ifndef COMS327_DIJKSTRA_H
#define COMS327_DIJKSTRA_H

#include "dungeon.h"
#include "heap.h"

#define WEIGHT_1 1
#define WEIGHT_2 1
#define WEIGHT_3 2
#define WEIGHT_4 3

class Vertex {

private:
    int weight;
    bool visited;
    bool queued;
    int num_neighbrs;
    Cell *cell;
    Vertex *prev;
    Vertex **neighbors;
public:

    Vertex();
    ~Vertex();

    void updateAdjacent(Heap *heap);

    void updateAdjacentNoRock(Heap *heap);

    int calcWeight();

    int getWeight() const {
        return weight;
    }

    void setWeight(int weight) {
        Vertex::weight = weight;
    }

    bool isVisited() const {
        return visited;
    }

    void setVisited(bool visited) {
        Vertex::visited = visited;
    }

    bool isQueued() const {
        return queued;
    }

    void setQueued(bool queued) {
        Vertex::queued = queued;
    }

    int getNum_neighbrs() const {
        return num_neighbrs;
    }

    void setNum_neighbrs(int num_neighbrs) {
        Vertex::num_neighbrs = num_neighbrs;
    }

    Cell *getCell() const {
        return cell;
    }

    void setCell(Cell *cell) {
        Vertex::cell = cell;
    }

    Vertex *getPrev() const {
        return prev;
    }

    void setPrev(Vertex *prev) {
        Vertex::prev = prev;
    }

    Vertex **getNeighbors() const {
        return neighbors;
    }

    void setNeighbors(Vertex **neighbors) {
        Vertex::neighbors = neighbors;
    }
};

class Graph{
private:
    int size;
    Dungeon *dungeon;
    Vertex *source;
    Vertex *verticies;
public:

    void dijkstra();

    void dijkstra_no_rock();

    void resetVerticies();

    Graph(Dungeon *dungeon, Cell *source);

    ~Graph();

    int getSize() const {
        return size;
    }

    void setSize(int size) {
        Graph::size = size;
    }

    Dungeon *getDungeon() const {
        return dungeon;
    }

    void setDungeon(Dungeon *dungeon) {
        Graph::dungeon = dungeon;
    }

    Vertex *getSource() const {
        return source;
    }

    void setSource(Vertex *source) {
        Graph::source = source;
    }

    Vertex *getVerticies() const {
        return verticies;
    }

    void setVerticies(Vertex *verticies) {
        Graph::verticies = verticies;
    }
};

#endif //COMS327_DIJKSTRA_H
