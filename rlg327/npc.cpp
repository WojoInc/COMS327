//
// Created by wojoinc on 3/21/17.
//

#include "npc.h"

NPC::NPC(int type, int speed, Graph *graph, Graph *graph_nr) :
    Character(type,speed,graph,graph_nr){
    symbol=mon_symbol[type];
    PC_location = nullptr;

    if((type & TUNNELING)==TUNNELING){
        int posx = (rand()%158)+1;
        int posy = (rand()%103)+1;

        location = &graph->getVerticies()[posy*graph->getDungeon()->getWidth() +posx];
    }
    else{
        int posx = (rand()%158)+1;
        int posy = (rand()%103)+1;

        while(graph_nr->getVerticies()[posy*graph_nr->getDungeon()->getWidth() +posx].getCell()->getType()!=rm_FLOOR){
            posx = (rand()%158)+1;
            posy = (rand()%103)+1;
        }
        location = &graph_nr->getVerticies()[posy*graph->getDungeon()->getWidth() +posx];
    }
    flatten();
}
void NPC::detect_PC_LOS(){
    //ask about better ways to do this later.
    for (int i = 0; i < 9; ++i) {
        if(location->getNeighbors()[i]!=NULL) {
            if (location->getNeighbors()[i]->getCell() == graph->getSource()->getCell()) {
                PC_location = location->getNeighbors()[i];
            }
        }
    }
}
void NPC::detect_PC(){
    //if monster is telepathic, get pc location from graph, else, by LOS
    if((type & TELEPATHY)==TELEPATHY){
        PC_location = graph->getSource();
    } else{
        detect_PC_LOS();
    }

}

void NPC::tunnel(Vertex *moveTo){
    moveTo->getCell()->setHardness(moveTo->getCell()->getHardness() - TUNNEL_CONST);
    if(moveTo->getCell()->getHardness()<=0 && moveTo->getCell()->getType()==ROCK){
        moveTo->getCell()->setHardness(0);
        moveTo->getCell()->setType(CORRIDOR);
        location = moveTo;
        //update non-tunneling and tunneling graph
        graph_nr->dijkstra_no_rock();
        graph->dijkstra();

    }
}

void NPC::move_intel(){
    /*
     * Move monster intelligently, using location->prev to move_monster monster to
     * previous vertex.
     * No need to check whether monster is able to tunnel or not, as location.prev points to
     * a different vertex depending on the graph, dungeon or dungeon_no_rock, that
     * the monster is using.
     * Instead, just checks if moveTo is rock, then tunnels
     */
    Vertex *moveTo = location->getPrev();
    if(moveTo==NULL) return;
    if(moveTo->getCell()->getType() == ROCK){
        tunnel(moveTo);
    }
    else if(location->getPrev()->getCell()->getType()!=IMPASS) {
        if(moveTo->getCell()->getType() ==rm_FLOOR||
           moveTo->getCell()->getType() ==CORRIDOR||
           moveTo->getCell()->getType() =='@'){//make sure its not moving on top of another monster
            location = moveTo;
        }
    }

}

void NPC::move_unintel(){
    //determine if monster knows where PC is
    if(PC_location!=NULL){
        //determine direction to move_monster
        int pcy, pcx, mony, monx;
        pcy = PC_location->getCell()->getY();
        pcx = PC_location->getCell()->getX();

        mony = location->getCell()->getY();
        monx = location->getCell()->getX();

        int y = 0, x=0;
        //determine if PC is above or below monster
        if(mony>pcy) y=-1;
        else if (mony<pcy) y=1;
        //determine if PC is to the left or right of the monster
        if(monx>pcx) x=-1;
        else if(monx<pcx) x=1;

        Vertex * moveTo = &graph->getVerticies()[(mony+y)*graph->getDungeon()->getWidth() + (monx+x)];
        if(moveTo->getCell()->getType() ==ROCK){
            //if monster can tunnel, tunnel, else monster is stuck
            if((type & TUNNELING)==TUNNELING) {
                tunnel(moveTo);
            }
        }
        else if(moveTo->getCell()->getType() ==IMPASS){
            return;
        }
        else if(moveTo->getCell()->getType() ==rm_FLOOR||
                moveTo->getCell()->getType() ==CORRIDOR
                ||moveTo->getCell()->getType() =='@'){//make sure its not moving on top of another monster
            location = moveTo;
        }
    }
    else return;
}

void NPC::move_erratic() {
    int num = rand()%8;
    bool moved = FALSE;
    while(!moved) {
        //make sure neighbor is not a dungeon border
        while (location->getNeighbors()[num]->getCell()->getType() == IMPASS) {
            num = rand() % 8;
        }
        if (location->getNeighbors()[num]->getCell()->getType() == ROCK) {
            //if neighbor is a rock, check if tunneling, else choose a different neighbor
            if ((type & TUNNELING)==TUNNELING){
                tunnel(location->getNeighbors()[num]);
                moved = TRUE;
            }
            else{
                num = rand() % 8;
            }
        }
        else if(location->getNeighbors()[num]->getCell()->getType() ==rm_FLOOR||
                location->getNeighbors()[num]->getCell()->getType() ==CORRIDOR){//if neighbor is an open space, move_monster to it
            location = location->getNeighbors()[num];
            moved = TRUE;
        }
    }
}

int NPC::rand_abilities(){
    return rand()%16;
}

bool NPC::move() {
    Vertex *last_pos = location;
    detect_PC();

    /*
     * If monster is erratic, generate random number. Determine if it is even or odd.
     * Theoretically, this should give the monster a 50/50 chance of moving as per
     * it's abilities, however, rand() is not necessarily uniform.
     *
     * even = move according to abilities
     * odd = move_monster to random neighbor cell
     */
    if((type & ERRATIC)==ERRATIC){
        if(rand()%2!=0){
            move_erratic();

        }
        else{
            if((type & INTELLIGENCE)==INTELLIGENCE){
                move_intel();
            }
            else{ //unintelligent, move in straight line to PC. if PC not yet seen or remembered, don't move_monster
                move_unintel();
            }
        }
        unflatten(last_pos);
        flatten();
        return true;
    }


    if((type & INTELLIGENCE)==INTELLIGENCE){
        move_intel();
    }
    else{ //unintelligent, move in straight line to PC. if PC not yet seen or remembered, don't move_monster
        move_unintel();
    }

    unflatten(last_pos);
    flatten();
    return true;
}

NPC::~NPC() {
    delete(graph);
    delete (graph_nr);
}
