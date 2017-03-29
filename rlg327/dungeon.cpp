//
// Created by root on 3/29/17.
//

#include <cstdlib>
#include "dungeon.h"

Dungeon::Dungeon(int height, int width, int num_rooms) {

    cells = new Cell[height*width];
    rooms = new Room[num_rooms];
    applyProperties();
    setBoundaries();
    //printDungeon(&dungeon);
    placeRooms();
    placeStairs();
    drawCorridors();

}

Dungeon::~Dungeon() {
    delete(rooms);
    delete(cells);
}

void Dungeon::placeStairs() {
    int y, x;
    for (int i = 0; i < NUM_STAIRS; ++i) {
        rooms[i].getCentroid(&y, &x);
        cells[y*width+x].setType(STAIR_DOWN);
        cells[y*width+x+1].setType(STAIR_DOWN);
        cells[y*width+x+2].setType(STAIR_DOWN);
    }
}

void Dungeon::applyProperties() {
    srand(time(NULL));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            cells[i*width+j].setHardness((rand()%253)+1);
            cells[i*width+j].setY(i);
            cells[i*width+j].setX(j);
            cells[i*width+j].setType(ROCK);
        }
    }
}

bool Dungeon::checkRoom(Room *room) {
    if((room->getX()+room->getWidth())>=width) return false;
    if((room->getY()+room->getHeight())>=height) return false;
    //check to make sure there is a 1 world unit gap around the area the room would cover
    for (int i = -1; i <= room->getHeight(); ++i) {
        //check each row verify world units are either ROCK
        for (int j = -1; j < room->getWidth(); ++j) {
            if(cells[(room->getY()+i)*width+(room->getX()+j)].getType()!=ROCK) return false;
        }
    }
    return true;
}

void Dungeon::placeRooms() {
    srand((unsigned int) time(NULL));
    Room temp;
    int tries;

    for (int count = 0; count < NUM_ROOMS; ++count) {
        //printDungeon(dungeon);
        temp.setY((rand()%height)+1);
        temp.setX((rand()%width)+1);
        temp.setHeight((rand()%r_MIN_H-1)+10);
        temp.setWidth((rand()%r_MIN_W-1)+10);

        tries = 0;

        while (!checkRoom(&temp) && tries < MAX_TRIES) {
            temp.setY((rand() % height) + 1);
            temp.setX((rand() % width) + 1);
            temp.setHeight((rand() % r_MIN_H) + 7);
            temp.setWidth((rand() % r_MIN_W) + 7);
            tries++;
        }
        if (tries < MAX_TRIES) { //meaning we did not time out, and room is valid
            rooms[count] = temp;
            applyRoom(&temp);
        }
    }
}

void Dungeon::setBoundaries() {
    //sides
    for (int i = 0; i < height; ++i) {
        cells[(i*width)].setType(IMPASS);
        cells[(i*width)+(width-1)].setType(IMPASS);
        cells[(i*width)].setHardness(255);
        cells[(i*width)+(width-1)].setHardness(255);
    }
    //top and bottom
    for (int j = 0; j < width; ++j) {
        cells[j].setType(IMPASS);
        cells[(height-1)*width+j].setType(IMPASS);
        cells[j].setHardness(255);
        cells[(height-1)*width+j].setHardness(255);
    }
}

int Dungeon::compareDistance(Room *ref, Room *room1, Room *room2) {
    return 0;
}

int Dungeon::compareDistanceCtrd(Room *ref, Room room1, Room room2) {
    return 0;
}

int Dungeon::writeDungeon(FILE *file) {
    int bytesWritten=0;
    char *NULL_BYTE = 0x00;
    int toWrite=0;
/*    for (int i = 0; i < d_HEIGHT; ++i) {
        for (int j = 0; j < d_WIDTH; ++j) {
            if(dungeon->wunits[i][j].type==(rm_FLOOR||CORRIDOR)){
                fwrite(&NULL_BYTE,sizeof(char),1,f);
                bytesWritten++;
            }
            else{
                toWrite = (uint8_t)htobe32((unsigned int) dungeon->wunits[i][j].hardness);
                fwrite(&toWrite,sizeof(char),1,f);
                bytesWritten++;
            }
        }
    }*/
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if(cells[i*width+j].getType()==(rm_FLOOR||CORRIDOR)){
                fwrite(&NULL_BYTE,sizeof(char),1,file);
                bytesWritten++;
            }
            else{
                toWrite = cells[i*width+j].getHardness();
                fwrite(&toWrite,sizeof(char),1,file);
                bytesWritten++;
            }
        }
    }
    return bytesWritten;
}

int Dungeon::writeRooms(FILE *file) {
    int bytesWritten=0;
    for (int i = 0; i < NUM_ROOMS; ++i) {
        std::fwrite(rooms[i].getX(),sizeof(char),1,file);
        std::fwrite(rooms[i].getY(),sizeof(char),1,file);
        std::fwrite(rooms[i].getWidth(),sizeof(char),1,file);
        std::fwrite(rooms[i].getHeight(),sizeof(char),1,file);
        bytesWritten += 4;
    }
    return bytesWritten;
}

void Dungeon::drawCorridors() {
    int x1, x2, y1, y2;
    int leftRight;
    int upDown;
    Room *temp;
    for (int i = 0; i < NUM_ROOMS; ++i) {
        rooms[i].getCentroid(&y1,&x1);
        temp = rooms[i].getClosestRoom(rooms);
        temp->getCentroid(&y2,&x2);

        if(x1<x2) leftRight = -1;
        else if(x1>x2) leftRight = 1;
        else leftRight = 0;

        if(y1<y2){
            upDown = -1;
        }
        else if(y1>y2) upDown = 1;
        else upDown = 0;

        //traverse the y direction, using x2 as the x coordinate
        for (int j = y2; j!=y1; j+=upDown) {
            /*
             * Check if the world unit is a ROCK, if so, make it a CORRIDOR.
             * Otherwise, it is a rm_FLOOR or a CORRIDOR already.
             */
            if(cells[j*width+x2].getType()==ROCK){
                cells[j*width+x2].setType(CORRIDOR);
                cells[j*width+x2].setHardness(0);
            }
        }

        for (int k = x2; k != x1; k+=leftRight) {
            if(cells[y1*width+k].getType()==ROCK){
                cells[y1*width+k].setType(CORRIDOR);
                cells[y1*width+k].setHardness(0);
            }
        }
    }
}

void Dungeon::applyRoom(Room *room) {
    for (int i = 0; i < room->getHeight(); ++i) {
        for (int j = 0; j < room->getWidth(); ++j) {
            cells[(room->getY()+i)*width+(room->getX()+j)].setType(rm_FLOOR);
            cells[(room->getY()+i)*width+(room->getX()+j)].setHardness(0);
        }
    }
}

Room* Room::getClosestRoom(Room *rooms) {

}
