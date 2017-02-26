//
// Created by wojoinc on 2/22/17.
//

#ifndef COMS327_PLAYER_H
#define COMS327_PLAYER_H

#include "proj_incl.h"
#include "dijkstra.h"
#include "dungeon.h"

typedef struct player_char{
    unsigned char abilities;
    graph_t *dungeon;
    graph_t *dungeon_no_rock;
    w_unit_t *spawn_point;
    vertex_t *location;
}player_t;

typedef struct player_event{
    unsigned int interval;
    unsigned int next_exec;
    player_t *player;
}p_event;

void p_update(p_event *pEvent);
void p_unflatten(player_t *player, vertex_t *fromPosition);
void p_flatten(player_t *player);
void move_player(player_t *player);
void spawn_player(player_t *player,graph_t *dungeon, graph_t *dungeon_no_rock);
p_event *player_init(dungeon_t *dungeon, unsigned int speed);

#endif //COMS327_PLAYER_H
