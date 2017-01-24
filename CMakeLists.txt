cmake_minimum_required(VERSION 3.6)
project(COMS327)

set(CMAKE_C_STANDARD 99)

#set(ASSGN-0-SOURCE Assignment-0/main.c Assignment-0/main.h)

set(ASSGN-11-SOURCE Assignment-1-1/main.c Assignment-1-1/main.h)
add_executable(Assignment-1-1 ${ASSGN-11-SOURCE})
#add_executable(Assignment0 ${ASSGN-0-SOURCE})