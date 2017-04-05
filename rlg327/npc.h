//
// Created by wojoinc on 3/21/17.
//

#ifndef COMS327_NPC_H
#define COMS327_NPC_H

#define INTELLIGENCE 0x1
#define TELEPATHY 0x2
#define TUNNELING 0x4
#define ERRATIC 0x8
#define SPD_CONST 1000
#define TUNNEL_CONST 85

#include "proj_incl.h"
#include "character.h"
#include "dijkstra.h"

class NPC : public Character{
private:
    const char mon_symbol[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    Vertex * PC_location;

    void detect_PC();
    void detect_PC_LOS();
    void tunnel(Vertex *moveTo);
    void move_intel();
    void move_unintel();
    void move_erratic();
public:
    NPC(int type, int speed, Graph *graph, Graph *graph_nr);
    ~NPC();
    bool move();
    int rand_abilities();
};

#endif //COMS327_NPC_H
