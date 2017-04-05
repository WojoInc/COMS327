//
// Created by wojoinc on 3/21/17.
//

#include "character.h"

void Character::update_event() {
    next_move+=SPD_CONST/speed;
}

Character::Character(int type, int speed, Graph *dungeon, Graph *graph_nr) {
    this->type = type;
    this->speed = speed;
    this->graph = dungeon;
    this->graph_nr = graph_nr;
    this->next_move = SPD_CONST/speed;
}

void Character::flatten() {
    location_type = location->getCell()->getType();
    location->getCell()->setType(symbol);
}

void Character::unflatten(Vertex* fromPosition) {
    fromPosition->getCell()->setType(location_type);
}

Vertex *Character::getLocation() const {
    return location;
}

int Character::getLocation_type() const {
    return location_type;
}

int Character::getSymbol() const {
    return symbol;
}
