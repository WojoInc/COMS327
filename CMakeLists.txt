cmake_minimum_required(VERSION 3.6)
project(COMS327)

set(CMAKE_C_STANDARD 99)

set(ASSGN-13-SOURCE rlg327/main.c rlg327/main.h rlg327/dijkstra.c rlg327/dijkstra.h rlg327/dungeon.c rlg327/dungeon.h rlg327/heap_a.c rlg327/heap_a.h rlg327/monster.c rlg327/monster.h rlg327/proj_incl.h rlg327/player.c rlg327/player.h)
add_executable(rlg327 ${ASSGN-13-SOURCE})
#add_executable(Assignment0 ${ASSGN-0-SOURCE})