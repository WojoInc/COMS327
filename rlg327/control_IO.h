//
// Created by wojoinc on 2/26/17.
//

#ifndef COMS327_CONTROL_IO_H
#define COMS327_CONTROL_IO_H

#include "proj_incl.h"
#include "dungeon.h"

#define WIN_TOP 0
#define WIN_LEFT 0
#define WIN_HEIGHT 20
#define WIN_WIDTH 80
#define TERM_HEIGHT 24;
#define TERM WIDTH 80;

class UI{
private:
    Dungeon *dungeon;
    int size_x;
    int size_y;
    int top;
    int left;

public:
    void mv_view(int y, int x);
    void draw_cell(int y, int x, int c);
    UI(Dungeon *dungeon);
    ~UI();
    void draw_dungeon();
    void view_focus_player(int y, int x);
    bool in_view(int y, int x);

    Dungeon *getDungeon() const {
        return dungeon;
    }

    void setDungeon(Dungeon *dungeon) {
        UI::dungeon = dungeon;
    }

    int getSize_x() const {
        return size_x;
    }

    void setSize_x(int size_x) {
        UI::size_x = size_x;
    }

    int getSize_y() const {
        return size_y;
    }

    void setSize_y(int size_y) {
        UI::size_y = size_y;
    }

    int getTop() const {
        return top;
    }

    void setTop(int top) {
        UI::top = top;
    }

    int getLeft() const {
        return left;
    }

    void setLeft(int left) {
        UI::left = left;
    }
};



#endif //COMS327_CONTROL_IO_H
