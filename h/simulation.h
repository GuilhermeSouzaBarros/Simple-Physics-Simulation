#ifndef SIMULATION
#define SIMULATION

#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"

#include "structs.h"
#include "player.h"
#include "objects.h"

Simulation* setupSimulation();
void freeSimulation(Simulation* simulation);
void updateSimulation(Simulation* simulation);
void drawSimulation(Simulation* simulation);

#endif