//
// Created by 97wes on 1/21/2017.
//

#include <stdint.h>
#include "dungeon.h"

#ifndef COMS327_MAIN_H
#define COMS327_MAIN_H

#endif //COMS327_MAIN_H

/*
 * Handle issues with filesystem and OS differences by creating flags that
 * can be un/commented based on the system being compiled for. This is good for now,
 * until I can find a way to do so in the Makefile
 */
#define UNIX_FS
//#define WINDOWS_FS
#define DUNGEON_DIR "/.rlg327/"
#define DUNGEON_FILE_NAME "dungeon"
#define FILE_TYPE 12 //bytes 0-11 are for the file descriptor
#define FILE_VER 4 //bytes 12-15 are for the file version and represent 1 32bit integer
#define FILE_SIZE 4 //bytes 16-19 are for the file size and represent 1 32bit integer
#define DUNGEON_MATRIX 16800 //bytes 20-16819 are an array contatining the hardness of each cell in the dungeon

void saveDungeon(dungeon_t *dungeon);
void loadDungeon(dungeon_t *dungeon);
int calcSaveSize();