//
// Created by wojoinc on 4/2/17.
//

#ifndef COMS327_DICE_H
#define COMS327_DICE_H

#include <cstdlib>
#include <string>

class Dice {
private:
    int base;
    int dice;
    int sides;
public:
    Dice(int base, int dice, int sides);
    virtual ~Dice();
    int roll();
    char* toString();
};


#endif //COMS327_DICE_H
