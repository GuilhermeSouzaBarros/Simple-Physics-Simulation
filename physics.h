#ifndef PHYSICS
#define PHYSICS

#include "structs.h"
#include "shapes.h"

void updatePhysics(Physics_obj* object, Simulation* simulation);
void updateCoord(Simulation* Simulation);
int isGrounded(Physics_obj object);

#endif