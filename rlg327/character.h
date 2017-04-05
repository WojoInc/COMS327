//
// Created by wojoinc on 3/21/17.
//

#ifndef COMS327_CHARACTER_H
#define COMS327_CHARACTER_H

#define SPD_CONST 1000

#include "dijkstra.h"

class Character {
protected:
    int symbol;
    int type;
    int speed;
    int next_move;
    int location_type;
    Graph *graph;
    Graph *graph_nr;
    Vertex * location;

public:
    //Character();
    Character(int type, int speed, Graph *graph, Graph *graph_nr);
    Vertex *getLocation() const;

    virtual int getLocation_type() const;
    int getSymbol() const;
    inline int getNext_move() const {
        return next_move;
    }
    inline void setNext_move(int next){
        next_move = next;
    }
    void flatten();
    void unflatten(Vertex* fromPosition);
    virtual bool move()= 0;
    void update_event();
};

#endif //COMS327_CHARACTER_H
