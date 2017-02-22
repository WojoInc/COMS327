//
// Created by wojoinc on 2/21/17.
//

#include "monster.h"

monster_t *spawn(unsigned char abilities){
    monster_t *monster = (monster_t*)malloc(sizeof(monster_t));
    monster->abilities = abilities;
    return monster;
}