cmake_minimum_required(VERSION 3.6)
project(COMS327)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
add_executable(COMS327 ${SOURCE_FILES})

set(ASSGN-0-SOURCE Assignment-0/main.c Assignment-0/main.h)
add_executable(ASSGN0 ${ASSGN-0-SOURCE})