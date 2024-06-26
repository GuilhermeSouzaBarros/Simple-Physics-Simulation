#ifndef SHAPES
#define SHAPES

#include "raylib.h"
#include "math.h"
#include "stdlib.h"

#include "structs.h"
#include "map.h"

void setupPrism4(Physics_obj* obj, double x_size, double y_size, double z_size, Vector3 pos);
void drawPrism4(Prism4 prisma);

Vector3 getCoordinates(void* object, int shape);
void addCoordinates(void* object, int shape, Vector3 coordinates);
void setCoordinates(void* object, int shape, Vector3 coordinates);

double getDistFarthestPoint(void* object, int shape);
double getLowestPoint(void* object, int shape);
double getArea(void* object, int shape, int coord); // 0: X / 1: Y / 2: Z
double getHeight(void* object, int shape);

int hasCollidedPrism4AndPrism4(Prism4* prism1, Prism4* prism2, Vector3 prism1_n, Vector3 prism2_n);
int isGroundedProp(Physics_obj* object, Simulation* simulation);
void collisionPrism4ToPrism4(Physics_obj* from_obj, Physics_obj* to_obj, CollInf* col, Simulation* simulation);
void updateCollisionProps(Physics_obj* from_obj, Physics_obj* to_obj,
                         CollInf* col, Simulation* simulation);
void updateCollisionsProps(Physics_obj* obj, Simulation* simulation, CollInf* col);
void updateCoord(Simulation* Simulation);
CollInf updateCollisions(Physics_obj* obj, Simulation* simulation);

#endif