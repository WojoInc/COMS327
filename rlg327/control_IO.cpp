//
// Created by wojoinc on 2/26/17.
//

#include "control_IO.h"


UI::UI(Dungeon *dungeon){
    initscr();
    clear();
    cbreak();
    noecho();
    keypad(stdscr,FALSE); // re-enable for special keys like arrows, but, need set export ESC_DELAY to 0 to remove lag
    curs_set(0);
        size_y = WIN_HEIGHT;
        size_x = WIN_WIDTH;
        left = WIN_LEFT;
        top = WIN_TOP;
        this->dungeon = dungeon;
}

void UI::view_focus_player(int y, int x){
    left = x-WIN_WIDTH/2>0 ? x-WIN_WIDTH/2 : 0;
    top = y-WIN_HEIGHT/2>0 ? y-WIN_HEIGHT/2 : 0;

    draw_dungeon();
}

bool UI::in_view(int y, int x){
    if(y<top||y>(top+size_y)) return false;
    else if(x<left||x>(left+size_x)) return false;
    else return true;
}

void UI::draw_cell(int y, int x, int c){
    //skip drawing cell if outside window boundaries
    if(y<top||y>(top+size_y)) return;
    else if(x<left||x>(left+size_x)) return;
    //calculate cell pos wrt the window
    int cell_locx = x - left;
    int cell_locy = y - top +1; //prevent printing on the top line and bottom 2 lines

    mvaddch(cell_locy,cell_locx, c);
    refresh();
}
void UI::draw_dungeon(){
    for(int i=top+1; i<top + size_y-1; ++i){
        for(int j=left+1; j<left + size_x-1; j++) {
            draw_cell(i,j,dungeon->getCells()[i*dungeon->getWidth()+j].getType());
        }
    }
    refresh();
}
//TODO prevent from printing dungeon borders
void UI::mv_view(int y, int x){
    int newtop = top + y*WIN_HEIGHT, newleft = left + x*WIN_WIDTH;
    if(newtop<=0) top = 0;
    else if(newtop+WIN_HEIGHT>dungeon->getHeight()) top = dungeon->getHeight()-WIN_HEIGHT;
    else top = newtop;

    if(newleft<=0) left = 0;
    else if(newleft+WIN_WIDTH>dungeon->getWidth()) left = dungeon->getWidth()-WIN_WIDTH;
    else left = newleft;

    draw_dungeon();
}
 UI::~UI(){
    endwin();
}