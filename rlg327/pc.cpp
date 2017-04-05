//
// Created by wojoinc on 3/21/17.
//

#include "pc.h"

bool PC::move(int dir) {
    Vertex *last_pos = location;
    if(dir<0||dir>8) return false;
    if(location->getNeighbors()[dir]->getCell()->getType()==CORRIDOR||
       location->getNeighbors()[dir]->getCell()->getType()==rm_FLOOR||
       location->getNeighbors()[dir]->getCell()->getType()==STAIR_DOWN||
       location->getNeighbors()[dir]->getCell()->getType()==STAIR_UP) {
        //update player location
        location = location->getNeighbors()[dir];
        graph->setSource(location);
        graph_nr->setSource(&graph_nr->getVerticies()[location->getCell()->getY() * graph_nr->getDungeon()->getWidth() +
                                                                              location->getCell()->getX()]);
        //update dungeon
        graph->dijkstra();
        graph_nr->dijkstra_no_rock();
        unflatten(last_pos);
        flatten();
        //player has moved
        return true;
    }
    return false;
}

PC::PC(Dungeon *dungeon, int speed) : Character(0,speed, nullptr, nullptr){
    int y=1,x=1;
    //player spawn point is the middle of the first generated room,
    // which may be anywhere in dungeon
    dungeon->getRooms()[0].getCentroid(&y,&x);
    symbol = PLAYER_SYMBOL;
    spawn_point = &dungeon->getCells()[y*dungeon->getWidth()+x];
    next_move = 1000/speed;
}

void PC::spawnPC(Graph *graph, Graph *no_rock) {
    this->graph = graph;
    this->graph_nr = no_rock;
    location = &graph->getVerticies()[(spawn_point->getY()*graph->getDungeon()->getWidth())+spawn_point->getX()];
    flatten();
}

bool PC::move() {
    return false;
}

