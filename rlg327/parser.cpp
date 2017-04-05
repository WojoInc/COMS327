//
// Created by wojoinc on 3/30/17.
//

#include <cstring>
#include <iostream>
#include "parser.h"

NPC Parser::parseNPC(char *buf) {


    return NPC(0, 0, nullptr, nullptr);
}

int Parser::parseFile(string file) {
    ifstream fs;
    string name = "";
    char symbol = 0;
    string color = "";
    string desc = "";
    Dice speed(0,0,0);
    Dice dam(0,0,0);
    Dice HP(0,0,0);
    string abil="";

    bool nameFound = false;
    bool symbolFound = false;
    bool colorFound = false;
    bool descFound = false;
    bool speedFound = false;
    bool damageFound = false;
    bool hpFound = false;
    bool abilFound = false;

    bool monsterparsed=false;
    bool parse_err = false;
    bool foundinline = false;

    string line;
    std::size_t found;
    fs.open(file);
    getline(fs,line);
    //check if the metadata is correct for this version
    if(line.find(mon_file_desc)!=string::npos) return BAD_MON_FILE;

    //begin parsing NPCs
    while(!fs.eof()){

        //search for beginning of definition
        getline(fs,line);

        while(!parse_err) {
            while(line.find("BEGIN MONSTER") == string::npos && !fs.eof()){
                getline(fs,line);
            }
            getline(fs,line);
            //continue to read traits until end of definition, or parse error, or end of file
            while (line.find("END") == string::npos && !fs.eof()) {
                found = line.find("NAME ");
                if (found == 0) {
                    if (!name.empty()) parse_err = true; //if already set, throw error to discard this monster.
                    else {
                        name = line.data() + 5;
                        foundinline = true;
                        nameFound=true;
                    }
                }
                found = line.find("SYMB ");
                if (found == 0 && !foundinline && !parse_err) {
                    if (symbol != 0) parse_err = true; //if already set, throw error to discard this monster.
                    else {
                        symbol = line.at(5);
                        if(symbol!=0) symbolFound = true;
                        else parse_err=true;
                    }
                }
                found = line.find("COLOR ");
                if (found == 0 && !foundinline && !parse_err) {
                    if (color.empty()) parse_err = true; //if already set, throw error to discard this monster.
                    else color = parseColor(line.data()+5);
                    if (color.empty()) parse_err = true; //if color was not set, throw parse error
                    else colorFound=true;
                }
                found = line.find("DESC ");
                if (found == 0 && !foundinline && !parse_err) {
                    if (!desc.empty()) parse_err = true; //if already set, throw error to discard this monster.
                    else {
                        desc = parseDesc(line.data()+5);
                        if(!desc.empty()) descFound = true;
                        else parse_err=true;
                    }
                }
                found = line.find("ABIL ");
                if (found == 0 && !foundinline && !parse_err) {
                    if (!abil.empty()) parse_err = true; //if already set, throw error to discard this monster.
                    else {
                        if(parseAbil(line.data()+5,&abil)>=0) abilFound = true;
                        else parse_err=true;
                    }
                }
                found = line.find("SPEED ");
                if (found == 0 && !foundinline && !parse_err) {
                    if (speedFound) parse_err = true; //if already set, throw error to discard this monster.
                    else {
                        if(parseNumerical(line.data()+6,&speed)>=0) speedFound = true;
                        else parse_err=true;
                    }
                }
                found = line.find("HP ");
                if (found == 0 && !foundinline && !parse_err) {
                    if (hpFound) parse_err = true; //if already set, throw error to discard this monster.
                    else {
                        if(parseNumerical(line.data()+3,&HP)>=0) hpFound = true;
                        else parse_err=true;
                    }
                }
                found = line.find("DAM ");
                if (found == 0 && !foundinline && !parse_err) {
                    if (damageFound) parse_err = true; //if already set, throw error to discard this monster.
                    else {
                        if(parseNumerical(line.data()+4,&dam)>=0) damageFound = true;
                        else parse_err=true;
                    }
                }
                if(parse_err||!foundinline) break;
                if(nameFound&&speedFound&&colorFound&&symbolFound&&descFound&&damageFound&&hpFound&&abilFound){
                    //print or create monster
                    cout<<"Name "<<name<<endl;
                    cout<<"Description "<<desc<<endl;
                    cout<<"Color "<<color<<endl;
                    cout<<"Speed "<<speed.toString()<<endl;
                    cout<<"Abilities "<<abil<<endl;
                    cout<<"HP "<<HP.toString()<<endl;
                    cout<<"Damage "<<dam.toString()<<endl<<endl;

                    name="";
                    desc="";
                    color="";
                    speed = Dice(0,0,0);
                    HP = Dice(0,0,0);
                    abil= "";
                    dam= Dice(0,0,0);
                    nameFound = false;
                    symbolFound = false;
                    colorFound = false;
                    descFound = false;
                    speedFound = false;
                    damageFound = false;
                    hpFound = false;
                    abilFound = false;

                    monsterparsed=false;
                    parse_err = false;
                    foundinline = false;
                }
                getline(fs, line);
            }
            if(!monsterparsed){
                name="";
                desc="";
                color="";
                speed = Dice(0,0,0);
                HP = Dice(0,0,0);
                abil= "";
                dam= Dice(0,0,0);
                nameFound = false;
                symbolFound = false;
                colorFound = false;
                descFound = false;
                speedFound = false;
                damageFound = false;
                hpFound = false;
                abilFound = false;

                monsterparsed=false;
                parse_err = false;
                foundinline = false;
            }
        }
    }

    return 0;
}

char* Parser::parseColor(const char *buf) {
    if(strcmp(NPC_COLOR[0],buf)==0) return (char*)NPC_COLOR[0];
    if(strcmp(NPC_COLOR[1],buf)==0) return (char*)NPC_COLOR[1];
    if(strcmp(NPC_COLOR[2],buf)==0) return (char*)NPC_COLOR[2];
    if(strcmp(NPC_COLOR[3],buf)==0) return (char*)NPC_COLOR[3];
    if(strcmp(NPC_COLOR[4],buf)==0) return (char*)NPC_COLOR[4];
    if(strcmp(NPC_COLOR[5],buf)==0) return (char*)NPC_COLOR[5];
    if(strcmp(NPC_COLOR[6],buf)==0) return (char*)NPC_COLOR[6];
    else return nullptr;
}

char *Parser::parseDesc(const char *buf) {
    return nullptr;
}

int Parser::parseNumerical(const char *buf,Dice* dice) {
    int base = atoi(buf);
    int numdice = atoi(strstr(buf,"+"));
    int numsides = atoi(strstr(buf,"d"));
    dice = new Dice(base, numdice, numsides);
    return 0;
}

int Parser::parseAbil(const char *buf, string* abil) {
    *abil="";
    if(strstr(buf,NPC_ABIL[0]) != nullptr) *abil+=NPC_ABIL[0];
    if(strstr(buf,NPC_ABIL[1]) != nullptr) *abil+=NPC_ABIL[1];
    if(strstr(buf,NPC_ABIL[2]) != nullptr) *abil+=NPC_ABIL[2];
    if(strstr(buf,NPC_ABIL[3]) != nullptr) *abil+=NPC_ABIL[3];
    if(strstr(buf,NPC_ABIL[4]) != nullptr) *abil+=NPC_ABIL[4];
    if(strstr(buf,NPC_ABIL[5]) != nullptr) *abil+=NPC_ABIL[5];
    if(strstr(buf,NPC_ABIL[6]) != nullptr) *abil+=NPC_ABIL[6];
    if(strlen(*abil)>0)return 0;
    else return -1;
}

