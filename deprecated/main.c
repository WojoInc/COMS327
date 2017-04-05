//
// Created by 97wes on 1/21/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <memory.h>
#include <errno.h>
#include "main.h"
#include "PC.h"
#include "control_IO.h"
#include "status.h"

//setup dungeon
dungeon_t *dungeon;
NPC **monsters;
PC* player;
struct Character *characterTmp;
//p_event *pEvent;
s_event *sEvent;
heap_t *m_event_queue;
graph_t *graph, *graph_no_rock;
UI_t *ui;
int num_mon = 15;

int main(int argc, char *argv[]){

    int opt = 0;
    int long_index =0;
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
        player = newPC(dungeon,10);

        graph = create_graph_dungeon(dungeon, get_spawn_point(player));
        graph_no_rock = create_graph_dungeon(dungeon, get_spawn_point(player));
        call_PC_spawn(player, graph, graph_no_rock);

        characterTmp = malloc(sizeof(struct Character));
        m_event_queue = heap_init((size_t) 15);
        monsters = generate_monsters(num_mon, m_event_queue, graph, graph_no_rock);

        //printDungeon(dungeon);
        dijkstra(graph);
        dijkstra_no_rock(graph_no_rock);

    }
    sEvent = malloc(sizeof(sEvent));
    sEvent->type = 0;
    ui = init_UI(dungeon);
    view_focus_player(ui,get_location((Character *)player)->w_unit->y, get_location((Character *)player)->w_unit->x);
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

    close_game();
}

void close_game(){
    for(int i=0; i<NUM_ROOMS; ++i){

    }
    free(dungeon->rooms);
    free(player);

    free(sEvent);
    /*for (int j = 0; j < num_mon; ++j) {
        free(monsters[j]);
    }
    free(monsters);*/
    free(dungeon->wunits);

    cleanup_heap(m_event_queue);
    cleanup_graph(graph);
    cleanup_graph(graph_no_rock);
    cleanup_win();
    exit(0);
}

void change_floors(){
    dungeon = generateDungeon();
    player = newPC(dungeon, 10);

    graph = create_graph_dungeon(dungeon, get_spawn_point(player));
    graph_no_rock = create_graph_dungeon(dungeon, get_spawn_point(player));
    call_PC_spawn(player, graph, graph_no_rock);

    characterTmp = malloc(sizeof(struct Character));
    m_event_queue = heap_init((size_t) 15);
    monsters = generate_monsters(num_mon, m_event_queue, graph, graph_no_rock);

    //printDungeon(dungeon);
    dijkstra(graph);
    dijkstra_no_rock(graph_no_rock);

    ui = init_UI(dungeon);
    view_focus_player(ui,get_location((Character*)player)->w_unit->y, get_location((Character*)player)->w_unit->x);
    //reset player event counter
    //TODO fix this once fully ported to C++
}

void ctl_mv_p(int dir){
    if(dir==-1){//if player is waiting a turn
        call_Character_update_event((Character*)player);
        return;
    }
    if(dir==8){
        if(get_location_type((Character*)player)==STAIR_DOWN){
            change_floors();
            return;
        }
        else return;
    }
    if(dir==9){
        if(get_location_type((Character*)player)==STAIR_DOWN){
            change_floors();
            return;
        }
        else return;
    }
    int prev_x = get_location((Character*)player)->w_unit->x;
    int prev_y = get_location((Character*)player)->w_unit->y;
    int prev_type = get_location_type((Character*)player);
    //do nothing unless the player actually moves, ie, location is not a wall or border
    if(call_PC_move(player,dir)) {
        //Use this code for all movements
        //Redraw the floor under the player
        draw_cell(ui,prev_y,prev_x, prev_type);
        if(!in_view(ui,get_location((Character*)player)->w_unit->y,get_location((Character*)player)->w_unit->x)){
            view_focus_player(ui,get_location((Character*)player)->w_unit->y,get_location((Character*)player)->w_unit->x);
        }
        call_Character_update_event((Character*)player);
        //draw new location of player
        draw_cell(ui, get_location((Character*)player)->w_unit->y, get_location((Character*)player)->w_unit->x, player);
        //TODO handle moving view to move with player if player moves outside bounds of view
    }
}

void ctl_mv_m(struct NPC *monster){
    int prev_x = get_location((Character*)monster)->w_unit->x;
    int prev_y = get_location((Character*)monster)->w_unit->y;
    int prev_type = get_location_type((Character*)monster);

    //Redraw the floor under the monster
    draw_cell(ui,prev_y,prev_x, prev_type);
    call_Character_move((Character*) monster);
    call_Character_update_event((Character*)monster);
    //draw new location of monster
    draw_cell(ui, get_location((Character*)monster)->w_unit->y, get_location((Character*)monster)->w_unit->x,
              get_symbol((Character*)monster));

}

void get_status(s_event *sEvent){

    if(sEvent->type==ENDGAME) {
        //printf("\nGAME OVER\n");
        close_game();
    }

}

int main_game() {

    bool ctl_mode = false; //false = look, true = control
    bool first_move = false;//used to make sure monsters dont move until PC has made its first turn
    //draw_dungeon(ui);
    int ch = 0;
    while(ch!='Q'){
        ch = getch();

        switch(ch){
            case '7':
            case 'y':
                if(ctl_mode){
                    first_move = true;
                    ctl_mv_p(0);
                }
                break;
            case '8':
            case 'k':
                if(ctl_mode){
                    first_move = true;
                    ctl_mv_p(1);
                }
                else{
                    mv_view(ui,-1,0);
                }
                break;
            case '9':
            case 'u':
                if(ctl_mode){
                    first_move = true;
                    ctl_mv_p(2);
                }
                break;
            case '6':
            case 'l':
                if(ctl_mode){
                    first_move = true;
                    ctl_mv_p(4);
                }
                else{
                    mv_view(ui,0,1);
                }
                break;
            case '3':
            case 'n':
                if(ctl_mode){
                    first_move = true;
                    ctl_mv_p(7);
                }
                break;
            case '2':
            case 'j':
                if(ctl_mode){
                    first_move = true;
                    ctl_mv_p(6);
                }
                else{
                    mv_view(ui,1,0);
                }
                break;
            case '1':
            case 'b':
                if(ctl_mode){
                    first_move = true;
                    ctl_mv_p(5);
                }
                break;
            case '4':
            case 'h':
                if(ctl_mode){
                    first_move = true;
                    ctl_mv_p(3);
                }
                else{
                    mv_view(ui,0,-1);
                }
                break;
            case '5':
            case ' ':
                if(ctl_mode){
                    first_move = true;
                    ctl_mv_p(-1);
                }
                break;
            case '>':
                if(ctl_mode){
                    first_move = true;
                    ctl_mv_p(8);
                }
                break;
            case '<':
                if(ctl_mode){
                    first_move = true;
                    ctl_mv_p(9);
                }
                break;
            case 'L':
                //if in control ctl_mode, switch to look ctl_mode
                if(ctl_mode) ctl_mode = false;
                break;
            case 27:
                //If in look ctl_mode, switch to control ctl_mode and then refocus view to player
                if(!ctl_mode) ctl_mode = true;
                view_focus_player(ui,get_location((Character*)player)->w_unit->y,get_location((Character*)player)->w_unit->x);
                break;
            default:
                break;
        }

        /*  process monster movements, move only monsters whose next execution time
         * is less than the players next exec. This means that monsters that move
         * faster than the PC will be able to move multiple times, each time adding their
         * interval to the next_exec counter until it is greater than the PC's
         * This loop will not execute unless the PC's next exec is updated, so look commands will not
         * trigger it, but commands like moving or waiting a turn will update the PC's next_exec
         * and cause the loop to execute.
         */
        while (first_move && peek_min(m_event_queue) <= get_next_move((Character*)player)) {
            characterTmp = remove_min(m_event_queue);
            ctl_mv_m(characterTmp);
            add_with_priority(m_event_queue, characterTmp, get_next_move(characterTmp));
        }
        get_status(sEvent);
    }

}

NPC **generate_monsters(int num_mon, heap_t *heap, graph_t *dungeon, graph_t *dungeon_no_rock){
    //monster_t **monsters = malloc(num_mon*sizeof(monster_t*));
    struct NPC *npc;
    for (int i = 0; i < num_mon; ++i) {
        npc = newNPC(rand()%16,(rand()%15)+5,dungeon,dungeon_no_rock);
      //  monsters[i] = event->monster;
        add_with_priority(heap,npc,get_next_move(npc));
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
