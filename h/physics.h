#ifndef PHYSICS
#define PHYSICS

#include "structs.h"
#include "shapes.h"
#include "map.h"

void updatePhysics(Physics_obj* object, Simulation* simulation);
int isGrounded(Physics_obj* object, Simulation* simulation);

#endif