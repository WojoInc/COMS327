//
// Created by wojoinc on 3/21/17.
//

#ifndef COMS327_PC_H
#define COMS327_PC_H

#define PLAYER_SYMBOL '@'

#include "character.h"
#include "dijkstra.h"

class PC : public Character{
private:
    Cell *spawn_point;
public:

    PC(Dungeon *dungeon, int speed);
    ~PC();
    void spawnPC(Graph *graph, Graph *no_rock);
    bool move();
    bool move(int dir);

    Cell *getSpawn_point() const {
        return spawn_point;
    }

    void setSpawn_point(Cell *spawn_point) {
        PC::spawn_point = spawn_point;
    }
};

#endif //COMS327_PC_H
