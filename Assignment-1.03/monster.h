//
// Created by wojoinc on 2/21/17.
//

#ifndef COMS327_MONSTER_H
#define COMS327_MONSTER_H

#include "proj_incl.h"
#define INTELLIGENCE = 0x1
#define TELEPATHY 0x2
#define TUNNELING 0x4
#define ERRATIC 0x8

typedef struct monster{
    unsigned char abilities;
}monster_t;

monster_t *spawn(unsigned char abilities);
#endif //COMS327_MONSTER_H
