//
// Created by 97wes on 1/21/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <memory.h>
#include <errno.h>
#include <zconf.h>
#include "main.h"
#include "player.h"
#include "control_IO.h"

//setup dungeon
dungeon_t *dungeon;
monster_t **monsters;
m_event *eventTemp;
p_event *pEvent;
heap_t *m_event_queue;
graph_t *graph, *graph_no_rock;
UI_t *ui;

int main(int argc, char *argv[]){

    int opt = 0;
    int long_index =0;
    int num_mon = 15;
    unsigned int tick;
    bool save=FALSE,load=FALSE,help=FALSE,display=FALSE,nummon=FALSE,PC_hit=FALSE;
    while ((opt = getopt_long_only(argc, argv,"", long_options, &long_index )) != -1) {
        switch (opt) {
            case 'o' : save = TRUE;
                break;
            case 'i' : load = TRUE;
                break;
            case 'h' : help = TRUE;
                break;
            case 'm' : nummon = TRUE;
                num_mon = atoi(optarg);
                break;
            default:
                help = TRUE;
                load = FALSE;
                save = FALSE;
                display = FALSE;
                num_mon = FALSE;
        }
    }

    if(load){
        loadDungeon(dungeon);
    }
    else {
        //if not loading a dungeon, generate a new one
        dungeon = generateDungeon();
        pEvent = player_init(dungeon, 10);

        graph = create_graph_dungeon(dungeon, pEvent->player->spawn_point);
        graph_no_rock = create_graph_dungeon(dungeon, pEvent->player->spawn_point);
        spawn_player(pEvent->player, graph, graph_no_rock);

        eventTemp = (m_event *) malloc(sizeof(m_event));
        m_event_queue = heap_init((size_t) 15);
        monsters = generate_monsters(num_mon, m_event_queue, graph, graph_no_rock);

        //printDungeon(dungeon);
        dijkstra(graph);
        dijkstra_no_rock(graph_no_rock);

    }
    ui = init_UI(dungeon);
    view_focus_player(ui,pEvent->player->location->w_unit->y, pEvent->player->location->w_unit->x);
    main_game();

    if(save){
        saveDungeon(dungeon);
    }
    if(help){
        printf("%s","Usage is: dungeon [options]"
                "\n--save save dungeon to file at ./rlg327"
                "\n--load load dungeon from file at ./rlg327"
                "\n--help display this help message");
    }
   //TODO add cleanup function
    free(dungeon->rooms);
    free(monsters);
    free(m_event_queue);
    free(graph);
    free(graph_no_rock);
    cleanup_win();

}

int main_game() {

    bool ctl_mode = false; //false = look, true = control
    //draw_dungeon(ui);
    int ch = 0;
    while(ch!='Q'){
        ch = getch();

        switch(ch){
            case '7':
            case 'y':
                if(ctl_mode){}
                break;
            case '8':
            case 'k':
                if(ctl_mode){}
                else{
                    mv_view(ui,-1,0);
                }
                break;
            case '9':
            case 'u':
                if(ctl_mode){}
                break;
            case '6':
            case 'l':
                if(ctl_mode){}
                else{
                    mv_view(ui,0,1);
                }
                break;
            case '3':
            case 'n':
                if(ctl_mode){}
                break;
            case '2':
            case 'j':
                if(ctl_mode){}
                else{
                    mv_view(ui,1,0);
                }
                break;
            case '1':
            case 'b':
                if(ctl_mode){}
                break;
            case '4':
            case 'h':
                if(ctl_mode){}
                else{
                    mv_view(ui,0,-1);
                }
                break;
            case '5':
            case ' ':
                if(!ctl_mode){}
                break;
            case '>':
                if(ctl_mode){}
                break;
            case '<':
                if(ctl_mode){}
                break;
            case 'L':
                //if in control ctl_mode, switch to look ctl_mode
                if(ctl_mode) ctl_mode = false;
                break;
            case 27:
                //If in look ctl_mode, switch to control ctl_mode and then refocus view to player
                if(!ctl_mode) ctl_mode = true;
                view_focus_player(ui,pEvent->player->location->w_unit->y,pEvent->player->location->w_unit->x);
                break;
            default:
                break;
        }

    }


    /*while(!PC_hit) {
        *//*
         * For now player just moves every 100 ticks. If the player has moved,
         * move the monsters whose next_exec is less than the current tick.
         *//*
        if (pEvent->next_exec <=tick) {
            p_update(pEvent);
            pEvent->next_exec+=pEvent->interval;
            while (peek_min(m_event_queue) <= tick) {
                eventTemp = (m_event *) remove_min(m_event_queue);
                m_update(eventTemp);
                eventTemp->next_exec += eventTemp->interval;
                add_with_priority(m_event_queue, eventTemp, eventTemp->next_exec);
            }
            printDungeon(dungeon);
        }
        usleep(3);
        tick++;
    }*/

}

monster_t **generate_monsters(int num_mon, heap_t *heap, graph_t *dungeon, graph_t *dungeon_no_rock){
    monster_t **monsters = (monster_t **)malloc(num_mon*sizeof(monster_t*));
    m_event *event;
    for (int i = 0; i < num_mon; ++i) {
        event = spawn(m_rand_abilities(),(rand()%15)+5,dungeon,dungeon_no_rock);
        monsters[i] = event->monster;
        add_with_priority(heap,event,event->interval);
    }
    return monsters;
}

void loadDungeon(dungeon_t *dungeon){
    char path[80];
    char fileDesc[FILE_TYPE];
    char fileVer[FILE_VER];
    unsigned int fileSize;
    int numRooms =0;

    //Parse predefined file path, open file
    parseFilePath(path);
    FILE *f = openDungeon(path,"r");

    /*
     * Begin reading file, and converting from big endian values where necessary, then, using a byte
     * offset, calculate the number of rooms in the file.
     */
    fread(fileDesc,sizeof(char),FILE_TYPE,f);
    fread(fileVer,sizeof(char),FILE_VER,f);
    fread(&fileSize,sizeof(char),FILE_SIZE,f);

    fileSize = be32toh(fileSize);
    numRooms = (fileSize - ROOM_OFFSET)/ROOM_SIZE;// hardcoded value for now. will change when we get to the point where the dimensions of the dungeon can change

    *dungeon = generateDungeon_d(numRooms);
    /*
     * Read the dungeon hardnesses in row major order,
     * Then parse the room data, and place the rooms in the dungeon.
     *
     * NOTE: In order to mark the corridors properly, all world units with a hardness of 0
     * are given a type of CORRIDOR.
     * This is overwritten later, if the world unit becomes part of a room, leaving the remaining
     * world units as CORRIDORS
     *
     */
    for (int i = 0; i < d_HEIGHT; ++i) {
        for (int j = 0; j < d_WIDTH; ++j) {
             fread(&dungeon->wunits[i][j].hardness,sizeof(char),1,f);
            if(dungeon->wunits[i][j].hardness==255){
                dungeon->wunits[i][j].type=IMPASS;
            }
            else if(dungeon->wunits[i][j].hardness==0){
                dungeon->wunits[i][j].type = CORRIDOR;
            }
            else {
                dungeon->wunits[i][j].type=ROCK;
            }
        }
    }
    for (int k = 0; k < numRooms; ++k) {
        fread(&dungeon->rooms[k].x,sizeof(char),1,f);
        fread(&dungeon->rooms[k].y,sizeof(char),1,f);
        fread(&dungeon->rooms[k].width,sizeof(char),1,f);
        fread(&dungeon->rooms[k].height,sizeof(char),1,f);
        applyRoom(&dungeon->rooms[k],dungeon);
    }

    //close file
    fclose(f);
}

void saveDungeon(dungeon_t *dungeon){
    char path[80];
    unsigned int bytesWritten = 0;
    unsigned int fileVersion = htobe32(VERSION);
    unsigned int fileSize = htobe32(calcSaveSize()+FILE_TYPE+FILE_VER+FILE_SIZE);

    /*
     * Parse the file path for the dungeon save file, and open the file or display
     * an error with a proper error message indicating why saving failed.
     */
    parseFilePath(path);
    FILE *f = openDungeon(path,"w+");//w+ for write or create new
    printf("Directory created, and file saved to: %s\n",path);

    /*
     * Begin writing to file, byte by byte, using constants outlined in project description as lengths
     */
	bytesWritten += fwrite("RLG327-S2017",sizeof(char),FILE_TYPE,f);
    bytesWritten += fwrite(&fileVersion,sizeof(char),FILE_VER,f);
    bytesWritten += fwrite(&fileSize,sizeof(char),FILE_SIZE,f);
    bytesWritten += writeDungeon(dungeon,f);
    bytesWritten += writeRooms(dungeon,f);
    if(htobe32(bytesWritten)!=fileSize) printf("Failed to write the correct number of bytes.\n");

    //close file
    fclose(f);
}

unsigned int calcSaveSize(){
    //number of bytes to write for dungeon world units
    unsigned int size = d_HEIGHT*d_WIDTH;
    size+=NUM_ROOMS*4;
    return size;
}

void parseFilePath(char *path) {
#ifdef UNIX_FS
    strcpy(path, getenv("HOME"));//get path to home directory
    strncat(path, DUNGEON_DIR, 9);//concat the new directory on to the stored path
    mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);//create new directory if it doesnt exist already.
    strncat(path, DUNGEON_FILE_NAME, 7);//concat file name onto the path
#endif
#ifdef WINDOWS_FS //add this part later if time permits
#endif
}

FILE *openDungeon(char *path, char *mode){
    FILE *f = fopen(path,mode);
    errno=0;
    if(f==NULL&&mode=="r"){
        printf("%s%s%c","\nCould not open file for reading.",strerror(errno),'\n');
        exit(errno);
    }
    else if(f==NULL&&mode=="w+"){
        printf("%s%s%c","\nCould not open file for writing.",strerror(errno),'\n');
        exit(errno);
    }
    else return f;
}
