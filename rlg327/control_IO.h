//
// Created by wojoinc on 2/26/17.
//

#ifndef COMS327_CONTROL_IO_H
#define COMS327_CONTROL_IO_H

#include "dungeon.h"
#include "ncurses.h"
#include "proj_incl.h"

#define WIN_TOP 0
#define WIN_LEFT 0
#define WIN_HEIGHT 20
#define WIN_WIDTH 80
#define TERM_HEIGHT 24;
#define TERM WIDTH 80;

typedef struct UI{
    dungeon_t *dungeon;
    int size_x;
    int size_y;
    int top;
    int left;
}UI_t;

void mv_view(UI_t *ui, int y, int x);
void draw_cell(UI_t *ui,int y, int x, int c);
UI_t *init_UI(dungeon_t *dungeon);
void cleanup_win();
void draw_dungeon(UI_t *ui);
void view_focus_player(UI_t *ui, int y, int x);


#endif //COMS327_CONTROL_IO_H
