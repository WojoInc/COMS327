//
// Created by wojoinc on 2/26/17.
//

#include "control_IO.h"


UI_t *init_UI(dungeon_t *dungeon){
    initscr();
    clear();
    cbreak();
    noecho();
    keypad(stdscr,FALSE); // re-enable for special keys like arrows, but, need set export ESC_DELAY to 0 to remove lag
    curs_set(0);
    UI_t *ui;
    if((ui = malloc(sizeof(UI_t)))) {
        ui->size_y = WIN_HEIGHT;
        ui->size_x = WIN_WIDTH;
        ui->left = WIN_LEFT;
        ui->top = WIN_TOP;
        ui->dungeon = dungeon;
        return ui;
    }
    return NULL;
}

void view_focus_player(UI_t *ui, int y, int x){
    ui->left = x-WIN_WIDTH/2>0 ? x-WIN_WIDTH/2 : 0;
    ui->top = y-WIN_HEIGHT/2>0 ? y-WIN_HEIGHT/2 : 0;

    draw_dungeon(ui);
}
void draw_cell(UI_t *ui,int y, int x, int c){
    //skip drawing cell if outside window boundaries
    if(y<ui->top||y>(ui->top+ui->size_y)) return;
    else if(x<ui->left||x>(ui->left+ui->size_x)) return;
    //calculate cell pos wrt the window
    int cell_locx = x - ui->left;
    int cell_locy = y - ui->top +1; //prevent printing on the top line and bottom 2 lines

    mvaddch(cell_locy,cell_locx, c);
    refresh();
}
void draw_dungeon(UI_t *ui){
    for(int i=ui->top; i<ui->top + ui->size_y; ++i){
        for(int j=ui->left; j<ui->left + ui->size_x; j++) {
            draw_cell(ui,i,j,ui->dungeon->wunits[i][j].type);
        }
    }
    refresh();
}
//TODO prevent from printing dungeon borders
void mv_view(UI_t *ui, int y, int x){
    int newtop = ui->top + y*WIN_HEIGHT, newleft = ui->left + x*WIN_WIDTH;
    if(newtop<=0) ui->top = 0;
    else if(newtop+WIN_HEIGHT>d_HEIGHT) ui->top = d_HEIGHT-WIN_HEIGHT;
    else ui->top = newtop;

    if(newleft<=0) ui->left = 0;
    else if(newleft+WIN_WIDTH>d_WIDTH) ui->left = d_WIDTH-WIN_WIDTH;
    else ui->left = newleft;

    draw_dungeon(ui);
}
void cleanup_win(){
    endwin();
}