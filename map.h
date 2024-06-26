#ifndef MAP
#define MAP

#include "structs.h"

int isTouchingMap(Physics_obj* obj, Map_obj* map);
int hasCollidedMap(Physics_obj* obj, Map_obj* map);
void updateObjCollisionsMap(Physics_obj* obj, Simulation* simulation, CollInf* col);
int isGroundedMap(Physics_obj* obj, Simulation* simulation);

#endif