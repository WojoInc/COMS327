//
// Created by wojoinc on 2/22/17.
//

#include "player.h"

p_event *player_init(dungeon_t *dungeon, unsigned int speed){
    int y=1,x=1;
    //player spawn point is the middle of the first generated room,
    // which may be anywhere in dungeon
    getCentroid(&dungeon->rooms[0],&y,&x);

    player_t *player = malloc(sizeof(player_t));
    player->spawn_point = &dungeon->wunits[y][x];
    player->abilities = 0x80; //default of 10000000 in binary for now, will change when player is improved upon

    p_event *pEvent = malloc(sizeof(p_event));
    pEvent->player = player;
    pEvent->interval = 1000/speed;
    pEvent->next_exec =pEvent->interval;
    return pEvent;
}
void spawn_player(player_t *player,graph_t *dungeon, graph_t *dungeon_no_rock){
    player->dungeon = dungeon;
    player->dungeon_no_rock = dungeon_no_rock;
    player->location = &dungeon->verticies[(player->spawn_point->y*d_WIDTH)+player->spawn_point->x];
    p_flatten(player);
}
void p_unflatten(player_t *player, vertex_t *fromPosition){
    fromPosition->w_unit->type = player->location_type;
}

void p_flatten(player_t *player){
    player->location_type = player->location->w_unit->type;
    player->location->w_unit->type = PLAYER;
}

bool move_player(player_t *player, int dir){
    vertex_t *last_pos = player->location;
    if(dir<0||dir>8) return false;
    if(player->location->neighbors[dir]->w_unit->type==CORRIDOR||
       player->location->neighbors[dir]->w_unit->type==rm_FLOOR) {
        //update player location
        player->location = player->location->neighbors[dir];
        player->dungeon->source = player->location;
        player->dungeon_no_rock->source = &player->dungeon_no_rock->verticies[player->location->w_unit->y * d_WIDTH +
                                                                              player->location->w_unit->x];
        //update dungeon
        dijkstra(player->dungeon);
        dijkstra_no_rock(player->dungeon_no_rock);
        p_unflatten(player, last_pos);
        p_flatten(player);
        //player has moved
        return true;
    }
    return false;
}
void p_update(p_event *pEvent){
    pEvent->next_exec+=pEvent->interval;
}