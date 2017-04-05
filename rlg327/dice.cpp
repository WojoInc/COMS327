//
// Created by wojoinc on 4/2/17.
//


#include "dice.h"

Dice::Dice(int base, int dice, int sides) : base(base), dice(dice), sides(sides) {
    srand(time(nullptr));
}

Dice::~Dice() {

}

int Dice::roll() {
    int total=base;
    for (int i = 0; i < dice; ++i) {
        total+=rand()%sides;
    }
    return base;
}

char *Dice::toString() {
    return (char*)((std::string)std::to_string(base)+=std::to_string('+')+=std::to_string(dice)+=std::to_string('d')+=std::to_string(sides)).c_str();
}
