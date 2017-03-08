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

void move_player(player_t *player){
    int moveTo = 0;
    moveTo = rand()%8;
    if(player->location->neighbors[moveTo]->w_unit->type!=IMPASS||player->location->neighbors[moveTo]==NULL) {
        moveTo = rand()%8;
    }
    player->location = player->location->neighbors[moveTo];
    player->dungeon->source = player->location;
    player->dungeon_no_rock->source = &player->dungeon_no_rock->verticies[player->location->w_unit->y*d_WIDTH +
    player->location->w_unit->x];
    dijkstra(player->dungeon);
    dijkstra_no_rock(player->dungeon_no_rock);
}
void p_update(p_event *pEvent){
    vertex_t *last_pos = pEvent->player->location;
    move_player(pEvent->player);
    p_unflatten(pEvent->player,last_pos);
    p_flatten(pEvent->player);

}