cmake_minimum_required(VERSION 3.6)
project(COMS327)

set(CMAKE_C_STANDARD 99)

#set(ASSGN-0-SOURCE Assignment-0/main.c Assignment-0/main.h)

set(ASSGN-13-SOURCE Assignment-1.03/dungeon.c Assignment-1.03/dungeon.h Assignment-1.03/dijkstra.h Assignment-1.03/dijkstra.c Assignment-1.03/heap_a.c Assignment-1.03/heap_a.h Assignment-1.03/proj_incl.h)
add_executable(Assignment-1.03 ${ASSGN-13-SOURCE})
#add_executable(Assignment0 ${ASSGN-0-SOURCE})