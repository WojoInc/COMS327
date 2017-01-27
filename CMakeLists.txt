cmake_minimum_required(VERSION 3.6)
project(COMS327)

set(CMAKE_C_STANDARD 99)

#set(ASSGN-0-SOURCE Assignment-0/main.c Assignment-0/main.h)

set(ASSGN-12-SOURCE Assignment-1.02/main.c Assignment-1.02/main.h Assignment-1.02/dungeon.c Assignment-1.02/dungeon.h)
add_executable(Assignment-1.02 ${ASSGN-12-SOURCE})
#add_executable(Assignment0 ${ASSGN-0-SOURCE})