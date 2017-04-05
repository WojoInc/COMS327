//
// Created by wojoinc on 3/29/17.
//

//
// Created by 97wes on 1/21/2017.
//

#include "main.h"
#include "parser.h"

//setup dungeon
Dungeon *dungeon;
NPC **monsters;
PC* player;
NPC *characterTmp;
//p_event *pEvent;
s_event *sEvent;
Heap *m_event_queue;
Graph *graph, *graph_no_rock;
UI *ui;
int num_mon = 15;

int main(int argc, char *argv[]){

    //************************
    //Just print monster definitions for now
    std::string path = "";
    path += getenv("HOME");//get path to home directory
    path+= DUNGEON_DIR;//concat the new directory on to the stored path
    path+= "monster_desc.txt";
    Parser *parser = new Parser;

    parser->parseFile(path);
    return 0;

    int opt = 0;
    int long_index =0;
    bool save=false,load=false,help=false,display=false,nummon=false,PC_hit=false;
    while ((opt = getopt_long_only(argc, argv,"", long_options, &long_index )) != -1) {
        switch (opt) {
            case 'o' : save = true;
                break;
            case 'i' : load = true;
                break;
            case 'h' : help = true;
                break;
            case 'm' : nummon = true;
                num_mon = atoi(optarg);
                break;
            default:
                help = true;
                load = false;
                save = false;
                display = false;
                num_mon = false;
        }
    }

    if(load){
        loadDungeon(dungeon);
    }
    else {
        //if not loading a dungeon, generate a new one
        dungeon = new Dungeon(d_MIN_HEIGHT,d_MIN_WIDTH,NUM_ROOMS);
        player = new PC(dungeon,10);

        graph = new Graph(dungeon, player->getSpawn_point());
        graph_no_rock = new Graph(dungeon, player->getSpawn_point());
        player->spawnPC(graph, graph_no_rock);

        m_event_queue = new Heap(15);
        monsters = generate_monsters(num_mon, m_event_queue, graph, graph_no_rock);

        //printDungeon(dungeon);
        graph->dijkstra();
        graph_no_rock->dijkstra_no_rock();

    }
    //sEvent = malloc(sizeof(sEvent));
    //sEvent->type = 0;
    ui = new UI(dungeon);
    ui->view_focus_player(player->getLocation()->getCell()->getY(), player->getLocation()->getCell()->getX());
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
    free(dungeon->getRooms());
    free(player);

    free(sEvent);
    /*for (int j = 0; j < num_mon; ++j) {
        free(monsters[j]);
    }
    free(monsters);*/
    free(dungeon->getCells());

    delete(m_event_queue);
   /* cleanup_graph(graph);
    cleanup_graph(graph_no_rock);*/
    delete(ui);
    exit(0);
}

void change_floors(){
    dungeon = new Dungeon(d_MIN_HEIGHT,d_MIN_WIDTH,NUM_ROOMS);
    player = new PC(dungeon, 10);

    graph = new Graph(dungeon, player->getSpawn_point());
    graph_no_rock = new Graph(dungeon, player->getSpawn_point());
    player->spawnPC(graph, graph_no_rock);

    //characterTmp = malloc(sizeof(struct character));
    m_event_queue = new Heap(15);
    monsters = generate_monsters(num_mon, m_event_queue, graph, graph_no_rock);

    //printDungeon(dungeon);
    graph->dijkstra();
    graph_no_rock->dijkstra_no_rock();

    ui = new UI(dungeon);
    ui->view_focus_player(player->getLocation()->getCell()->getY(), player->getLocation()->getCell()->getX());
    //reset player event counter
    //TODO fix this once fully ported to C++
}

void ctl_mv_p(int dir){
    if(dir==-1){//if player is waiting a turn
        player->update_event();
        return;
    }
    if(dir==8){
        if(player->getLocation_type()==STAIR_DOWN){
            change_floors();
            return;
        }
        else return;
    }
    if(dir==9){
        if(player->getLocation_type()==STAIR_DOWN){
            change_floors();
            return;
        }
        else return;
    }
    int prev_x = player->getLocation()->getCell()->getX();
    int prev_y = player->getLocation()->getCell()->getY();
    int prev_type = player->getLocation_type();
    //do nothing unless the player actually moves, ie, location is not a wall or border
    if(player->move(dir)) {
        //Use this code for all movements
        //Redraw the floor under the player
        ui->draw_cell(prev_y,prev_x, prev_type);
        if(!ui->in_view(player->getLocation()->getCell()->getY(),player->getLocation()->getCell()->getX())){
            ui->view_focus_player(player->getLocation()->getCell()->getY(),player->getLocation()->getCell()->getX());
        }
        player->update_event();
        //draw new location of player
        ui->draw_cell(player->getLocation()->getCell()->getY(), player->getLocation()->getCell()->getX(), player->getSymbol());
        //TODO handle moving view to move with player if player moves outside bounds of view
    }
}

void ctl_mv_m(struct NPC *monster){
    int prev_x = monster->getLocation()->getCell()->getX();
    int prev_y = monster->getLocation()->getCell()->getY();
    int prev_type = monster->getLocation_type();

    //Redraw the floor under the monster
    ui->draw_cell(prev_y,prev_x, prev_type);
    monster->move();
    monster->update_event();
    //draw new location of monster
    ui->draw_cell(monster->getLocation()->getCell()->getY(), monster->getLocation()->getCell()->getX(),
              monster->getSymbol());

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
                    ui->mv_view(-1,0);
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
                    ui->mv_view(0,1);
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
                    ui->mv_view(1,0);
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
                    ui->mv_view(0,-1);
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
                ui->view_focus_player(player->getLocation()->getCell()->getY(),player->getLocation()->getCell()->getX());
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
        while (first_move && m_event_queue->peekMin() <= player->getNext_move()) {
            characterTmp = (NPC*)m_event_queue->removeMin();
            ctl_mv_m(characterTmp);
            m_event_queue->addWithPriority(characterTmp, characterTmp->getNext_move());
        }
        //get_status(sEvent);
    }

}

NPC **generate_monsters(int num_mon, Heap *heap, Graph *dungeon, Graph *dungeon_no_rock){
    //monster_t **monsters = malloc(num_mon*sizeof(monster_t*));
    NPC *npc;
    for (int i = 0; i < num_mon; ++i) {
        npc = new NPC(rand()%16,(rand()%15)+5,dungeon,dungeon_no_rock);
        //  monsters[i] = event->monster;
        heap->addWithPriority(npc,npc->getNext_move());
    }
    return monsters;
}

void loadDungeon(Dungeon *dungeon){
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

    dungeon = new Dungeon(d_MIN_HEIGHT,d_MIN_WIDTH,numRooms);
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
    for (int i = 0; i < d_MIN_HEIGHT; ++i) {
        for (int j = 0; j < d_MIN_WIDTH; ++j) {
            //TODO fix this part
            int hardness;
            fread(&hardness,sizeof(char),1,f);
            dungeon->getCells()[i*dungeon->getWidth()+j].setHardness(hardness);
            if(dungeon->getCells()[i*dungeon->getWidth()+j].getHardness()==255){
                dungeon->getCells()[i*dungeon->getWidth()+j].setType(IMPASS);
            }
            else if(dungeon->getCells()[i*dungeon->getWidth()+j].getHardness()==0){
                dungeon->getCells()[i*dungeon->getWidth()+j].setType(CORRIDOR);
            }
            else {
                dungeon->getCells()[i*dungeon->getWidth()+j].setType(ROCK);
            }
        }
    }
    int tmp;
    for (int k = 0; k < numRooms; ++k) {
        fread(&tmp,sizeof(char),1,f);
        dungeon->getRooms()[k].setX(tmp);
        fread(&tmp,sizeof(char),1,f);
        dungeon->getRooms()[k].setY(tmp);
        fread(&tmp,sizeof(char),1,f);
        dungeon->getRooms()[k].setWidth(tmp);
        fread(&tmp,sizeof(char),1,f);
        dungeon->getRooms()[k].setHeight(tmp);
        dungeon->applyRoom(&dungeon->getRooms()[k]);
    }

    //close file
    fclose(f);
}

void saveDungeon(Dungeon *dungeon){
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
    bytesWritten += dungeon->writeDungeon(f);
    bytesWritten += dungeon->writeRooms(f);
    if(htobe32(bytesWritten)!=fileSize) printf("Failed to write the correct number of bytes.\n");

    //close file
    fclose(f);
}

unsigned int calcSaveSize(){
    //number of bytes to write for dungeon world units
    unsigned int size = d_MIN_HEIGHT*d_MIN_WIDTH;
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
