//
// Created by wojoinc on 2/21/17.
//

#ifndef COMS327_MONSTER_H
#define COMS327_MONSTER_H

#include "proj_incl.h"
#include "../deprecated/dungeon.h"
#include "dijkstra.h"
#include "status.h"

#define INTELLIGENCE 0x1
#define TELEPATHY 0x2
#define TUNNELING 0x4
#define ERRATIC 0x8
#define SPD_CONST 1000
#define TUNNEL_CONST 85


typedef struct monster{
    int symbol;
    int type;
    int location_type;
    graph_t *dungeon;
    graph_t *dungeon_no_rock;
    vertex_t * location;
    vertex_t * PC_location;
}monster_t;

typedef struct monster_event{
    int interval;
    int next_exec;
    monster_t *monster;
}m_event;

m_event *spawn(int type, int speed, graph_t *dungeon, graph_t *dungeon_no_rock);

void detect_PC(monster_t *monster);
void detect_PC_LOS(monster_t *monster);
void tunnel(monster_t *monster, vertex_t *moveTo);
void move_intel(monster_t *monster);
void move_unintel(monster_t *monster);
bool move_monster(monster_t *monster);
void m_unflatten(monster_t *monster, vertex_t *fromPosition);
void m_flatten(monster_t *monster);
void m_update(m_event *mEvent, s_event *sEvent);
int m_rand_abilities();

#endif //COMS327_MONSTER_H
