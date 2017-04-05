//
// Created by wojoinc on 3/30/17.
//

#ifndef COMS327_PARSER_H
#define COMS327_PARSER_H


#include <fstream>
#include <string>
#include <cstdlib>
#include "dice.h"
#include "npc.h"
#include "pc.h"

using namespace std;

enum PARSE_ERROR{
    BAD_MON_FILE=-1
};

class Parser {

private:
    const char mon_file_desc[] = "RLG327 MONSTER DESCRIPTION 1";
    const char mon_header[] = "BEGIN MONSTER\n";
    const char* NPC_COLOR[] = {"RED","GREEN","BLUE","CYAN","YELLOW","MAGENTA","WHITE","BLACK"};
    const char* NPC_ABIL[] = {"SMART","TELE","TUNNEL","ERRATIC","PASS","PICKUP","DESTROY"};

    NPC parseNPC(char* buf);
    char* parseColor(const char* buf);
    char *parseDesc(const char *buf);
    int parseNumerical(const char *buf,Dice* dice);
    int parseAbil(const char *buf, string* abil);

public:
    int parseFile(string file);
};


#endif //COMS327_PARSER_H
