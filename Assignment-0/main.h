//
// Created by 97wes on 1/11/2017.
//

#ifndef COMS327_MAIN_H
#define COMS327_MAIN_H

#endif //COMS327_MAIN_H
typedef int bool;
#define true = 1;
#define false = 0;
struct Animal{
    char state;
    int x;
    int y;
};

int main(int argc, char *argv[]);

int parseInt(char c);
bool checkRules(char animal, int index);

void updateSquare(struct Animal animal);

void displayHabitat(char habitat);
