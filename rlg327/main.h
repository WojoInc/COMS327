//
// Created by 97wes on 1/21/2017.
//
#ifndef COMS327_MAIN_H
#define COMS327_MAIN_H

#include <stdint.h>
#include "dungeon.h"
#include "proj_incl.h"
#include "dijkstra.h"
#include "monster.h"
#include <getopt.h>

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
#define VERSION 0 //define actual file version, defined here for later use
#define FILE_SIZE 4 //bytes 16-19 are for the file size and represent 1 32bit integer
#define ROOM_OFFSET 16820 //bytes after 16819 are reserved for room placement data
#define ROOM_SIZE 4 //number of bytes needed to store information for each room, defined here for later use

static struct option long_options[] = {
        {"save", no_argument, 0, 'o'},
        {"load", no_argument, 0, 'i'},
        {"help", no_argument, 0, 'h'},
        {"nummon", required_argument,0,'m'},
        {0,0,0,0}
};


monster_t **generate_monsters(int nummon, heap_t *heap, graph_t *dungeon, graph_t *dungeon_no_rock);
/**
 * Saves @dungeon to a file path created at %HOME%/.rlg327/dungeon
 * @param dungeon the dungeon to be saved
 */
void saveDungeon(dungeon_t *dungeon);

/**
 * Loads @dungeon from a file path created at %HOME%/.rlg327/dungeon
 * @param dungeon dungeon to be loaded
 */
void loadDungeon(dungeon_t *dungeon);

/**
 * Calculates the size of the dungeon to be saved, based on the number of
 * rooms, and constants predefined in main.h
 * @return size of the savefile in bytes
 */
unsigned int calcSaveSize();

/**
 * Parses the file path for the save file.
 * @param path address to store path
 */
void parseFilePath(char *path);

/**
 * Opens the dungeon save file for either reading or writing. Value passed to @mode is
 * passed directly to the mode parameter of fopen(). Also handles displaying appropriate
 * errors when file cannot be read/written.
 *
 * @param path the file path
 * @param mode the mode for file I/O
 * @return file handle for the dungeon save file
 */
FILE *openDungeon(char *path, char *mode);

#endif //COMS327_MAIN_H