//
// Created by 97wes on 1/21/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <memory.h>
#include "main.h"


int main(int argc, char *argv[]){

    if(argc<2)printf("%s\n","Incorrect syntax");
    if(argc==2||argc==3){
    }
    saveDungeon(NULL);

}
void loadDungeon(dungeon_t *dungeon){


}
void saveDungeon(dungeon_t *dungeon){
    char path[80];
#ifdef WINDOWS_FS
#endif
#ifdef UNIX_FS
    strcpy(path,getenv("HOME"));
    strncat(path,DUNGEON_DIR,8);
    //mkdir(DUNGEON_DIR,)
    strncat(path,DUNGEON_PATH,16);//TODO fix issue where parent directory must exist
    printf("%s",path);
#endif
    FILE *f = fopen(path,"w+");
    if(f ==NULL)printf("%s\n","The file could not be created.");
	fwrite("RLG327-S2017",sizeof(char),FILE_TYPE,f);
    fclose(f);
}

void parseDungeon(){}
int calcSaveSize(){

}
