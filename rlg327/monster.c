//
// Created by wojoinc on 2/21/17.
//

#include "monster.h"

const char mon_symbol[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

m_event *spawn(int type, int speed, graph_t *dungeon, graph_t *dungeon_no_rock){
    monster_t *monster = (monster_t*)malloc(sizeof(monster_t));
    monster->type = type;
    monster->dungeon = dungeon;
    monster->dungeon_no_rock = dungeon_no_rock;

    monster->symbol=mon_symbol[monster->type];

    if((monster->type & TUNNELING)==TUNNELING){
        int posx = (rand()%158)+1;
        int posy = (rand()%103)+1;

        monster->location = &dungeon->verticies[posy*d_WIDTH +posx];
    }
    else{
        int posx = (rand()%158)+1;
        int posy = (rand()%103)+1;

        while(dungeon_no_rock->verticies[posy*d_WIDTH +posx].w_unit->type!=rm_FLOOR){
            posx = (rand()%158)+1;
            posy = (rand()%103)+1;
        }
        monster->location = &dungeon_no_rock->verticies[posy*d_WIDTH +posx];
    }
    m_flatten(monster);

    m_event *mEvent = malloc(sizeof(m_event));
    mEvent->monster = monster;
    mEvent->interval = SPD_CONST/speed;
    mEvent->next_exec = SPD_CONST/speed;
    return mEvent;
}
void detect_PC_LOS(monster_t *monster){
    //ask about better ways to do this later.
    for (int i = 0; i < 9; ++i) {
        if(monster->location->neighbors[i]!=NULL) {
            if (monster->location->neighbors[i]->w_unit == monster->dungeon->source->w_unit) {
                monster->PC_location = monster->location->neighbors[i];
            }
        }
    }
}
void detect_PC(monster_t *monster){
    //if monster is telepathic, get pc location from graph, else, by LOS
    if((monster->type & TELEPATHY)==TELEPATHY){
        monster->PC_location = monster->dungeon->source;
    } else{
        detect_PC_LOS(monster);
    }

}

void tunnel(monster_t *monster, vertex_t *moveTo){
    moveTo->w_unit->hardness -= TUNNEL_CONST;
    if(moveTo->w_unit->hardness<=0){
        moveTo->w_unit->hardness=0;
        moveTo->w_unit->type = CORRIDOR;
        monster->location = moveTo;
        //update non-tunneling and tunneling graph
        dijkstra_no_rock(monster->dungeon_no_rock);
        dijkstra(monster->dungeon);
    }
}

void move_intel(monster_t *monster){
    /*
     * Move monster intelligently, using location->prev to move_monster monster to
     * previous vertex.
     * No need to check whether monster is able to tunnel or not, as location.prev points to
     * a different vertex depending on the graph, dungeon or dungeon_no_rock, that
     * the monster is using.
     * Instead, just checks if moveTo is rock, then tunnels
     */
    vertex_t *moveTo = monster->location->prev;
    if(moveTo->w_unit->type == ROCK){
        tunnel(monster,moveTo);
    }
    monster->location = monster->location->prev;
}

void move_unintel(monster_t *monster){
    //determine if monster knows where PC is
    if(monster->PC_location!=NULL){
        //determine direction to move_monster
        int pcy, pcx, mony, monx;
        pcy = monster->PC_location->w_unit->y;
        pcx = monster->PC_location->w_unit->x;

        mony = monster->location->w_unit->y;
        monx = monster->location->w_unit->x;

        int y = 0, x=0;
        //determine if PC is above or below monster
        if(mony>pcy) y=-1;
        else if (mony<pcy) y=1;
        //determine if PC is to the left or right of the monster
        if(monx>pcx) x=-1;
        else if(monx<pcx) x=1;

        vertex_t * moveTo = &monster->dungeon->verticies[(mony+y)*d_WIDTH + (monx+x)];
        if(moveTo->w_unit->type==ROCK){
            //if monster can tunnel, tunnel, else monster is stuck
            if((monster->type & TUNNELING)==TUNNELING) {
                tunnel(monster, moveTo);
            }
        }
        else{
            monster->location = moveTo;
        }
    }
    else return;
}

void move_monster(monster_t *monster){
    if((monster->type & INTELLIGENCE)==INTELLIGENCE){
        move_intel(monster);
    }
    else{ //unintelligent, move in straight line to PC. if PC not yet seen or remembered, don't move_monster
        move_unintel(monster);
    }
}

void move_erratic(monster_t *monster) {
    int num = rand()%8;
    bool moved = FALSE;
    while(!moved) {
        //make sure neighbor is not a dungeon border
        while (monster->location->neighbors[num]->w_unit->type == IMPASS) {
            num = rand() % 8;
        }
        if (monster->location->neighbors[num]->w_unit->type == ROCK) {
            //if neighbor is a rock, check if tunneling, else choose a different neighbor
            if ((monster->type & TUNNELING)==TUNNELING){
                tunnel(monster,monster->location->neighbors[num]);
                moved = TRUE;
            }
            else{
                num = rand() % 8;
            }
        }
        else{//if neighbor is an open space, move_monster to it
            monster->location = monster->location->neighbors[num];
            moved = TRUE;
        }
    }
}

void m_unflatten(monster_t *monster, vertex_t *fromPosition){
    fromPosition->w_unit->type = monster->location_type;
}

void m_flatten(monster_t *monster){
    monster->location_type = monster->location->w_unit->type;
    monster->location->w_unit->type = monster->symbol;
}

int m_rand_abilities(){
    return rand()%16;
}
void m_update(m_event *mEvent){
    vertex_t *last_pos = mEvent->monster->location;

    if(last_pos->w_unit == mEvent->monster->dungeon->source->w_unit ||
            last_pos->w_unit == mEvent->monster->dungeon_no_rock->source->w_unit){
        printf("\nGAME OVER\n");
        exit(0);
    }
    /*
     * If monster is erratic, generate random number. Determine if it is even or odd.
     * Theoretically, this should give the monster a 50/50 chance of moving as per
     * it's abilities, however, rand() is not necessarily uniform.
     *
     * even = move according to abilities
     * odd = move_monster to random neighbor cell
     */
    if((mEvent->monster->type & ERRATIC)==ERRATIC){
        if(rand()%2==0){
            detect_PC(mEvent->monster);
            move_monster(mEvent->monster);
        }
        else{
            move_erratic(mEvent->monster);
        }
        return;
    }
    detect_PC(mEvent->monster);
    move_monster(mEvent->monster);
    m_unflatten(mEvent->monster,last_pos);
    m_flatten(mEvent->monster);
}

