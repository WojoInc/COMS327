//
// Created by 97wes on 1/21/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <memory.h>
#include <endian.h>
#include "main.h"


int main(int argc, char *argv[]){

    if(argc<2)printf("%s\n","Incorrect syntax");
    if(argc==2||argc==3){
    }
    dungeon_t dungeon = generateDungeon();
    saveDungeon(&dungeon);
    printDungeon(&dungeon);
    loadDungeon(&dungeon);
    printDungeon(&dungeon);
    free(&dungeon.rooms);

}
void loadDungeon(dungeon_t *dungeon){
    char path[80];
    char fileDesc[FILE_TYPE];
    char fileVer[FILE_VER];
    char filesize[FILE_SIZE];
    int numRooms =0;
#ifdef UNIX_FS
    strcpy(path,getenv("HOME"));//get path to home directory
    strncat(path,DUNGEON_DIR,9);//concat the new directory on to the stored path
    mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);//create new directory if it doesnt exist already.
    strncat(path,DUNGEON_FILE_NAME,7);//concat file name onto the path
#endif
    FILE *f = fopen(path,"r");
    fread(fileDesc,sizeof(char),FILE_TYPE,f);
    fread(fileVer,sizeof(char),FILE_VER,f);
    fgets(filesize,FILE_SIZE,f);


    numRooms = (atoi(filesize) - 16820)/4;// hardcoded value for now. will change when we get to the point where the dimensions of the dungeon can change
    dungeon = malloc(sizeof(dungeon_t) + numRooms*sizeof(room_t));


    //read the dungeon hardnesses
    for (int i = 0; i < d_WIDTH; ++i) {
        for (int j = 0; j < d_HEIGHT; ++j) {
             fread(&dungeon->wunits[j][i].hardness,sizeof(char),1,f);
            if(dungeon->wunits[j][i].hardness==255)dungeon->wunits[j][i].type=IMPASS;
            else dungeon->wunits[j][i].type=ROCK;
        }
    }


    for (int k = 0; k < numRooms; ++k) {
        fread(&dungeon->rooms[k].x,sizeof(char),1,f);
        fread(&dungeon->rooms[k].y,sizeof(char),1,f);
        fread(&dungeon->rooms[k].width,sizeof(char),1,f);
        fread(&dungeon->rooms[k].height,sizeof(char),1,f);
        applyRoom(&dungeon->rooms[k],dungeon);
    }
    fclose(f);
}
void saveDungeon(dungeon_t *dungeon){
    char path[80];
    int bytesWritten = 0;
    int fileSize = htobe32((unsigned int)(calcSaveSize()+FILE_TYPE+FILE_VER+FILE_SIZE));
    int *NULL_BYTE = 0x00;

#ifdef UNIX_FS
    strcpy(path,getenv("HOME"));//get path to home directory
    strncat(path,DUNGEON_DIR,9);//concat the new directory on to the stored path
    mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);//create new directory if it doesnt exist already.
    strncat(path,DUNGEON_FILE_NAME,7);//concat file name onto the path
#endif
#ifdef WINDOWS_FS //add this part later if time permits
#endif
    printf("Directory created, and file saved to: %s\n",path);
    FILE *f = fopen(path,"w+");//w+ for write or create new
    if(f ==NULL)printf("%s\n","The file could not be created.");
	bytesWritten += fwrite("RLG327-S2017",sizeof(char),FILE_TYPE,f);
    bytesWritten += fwrite(&NULL_BYTE,sizeof(char),FILE_VER,f);
    bytesWritten += fwrite(&fileSize,sizeof(char),FILE_SIZE,f);
    bytesWritten += writeDungeon(dungeon,f);
    bytesWritten += writeRooms(dungeon,f);
    if(htobe32((unsigned int)bytesWritten)!=fileSize) printf("Failed to write the correct number of bytes.\n");
    fclose(f);
}


int calcSaveSize(){
    //number of bytes to write for dungeon world units
    int size = d_HEIGHT*d_WIDTH;
    size+=NUM_ROOMS*4;
    return size;
}
