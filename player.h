#ifndef PLAYER
#define PLAYER

#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#include "structs.h"
#include "physics.h"

Player* setupPlayer();

void freePlayer(Player* player);

void updatePlayer(Simulation* simulation);

#endif