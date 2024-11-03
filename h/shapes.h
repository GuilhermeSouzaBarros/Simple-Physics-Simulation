#ifndef SHAPES
#define SHAPES

#include "raylib.h"
#include "math.h"
#include "stdlib.h"

#include "structs.h"

typedef struct Prism4 {
    float x_size;
    float y_size;
    float z_size;
    Vector3 position;
    Vector3 up;
} Prism4;

typedef struct Physics_obj {
    void* hitbox;
    int hitbox_shape; // 0: Prisma 4 sided base / 1: Cylinder
    Vector3 speed;
    float drag;
    float mass;
    float fric[2]; // 0: Stationary / 1: Moving
} Physics_obj;

typedef struct chainColl {
    Physics_obj* obj;
    float s_gap;
    struct chainColl* next;
} chainColl;

void setupPrism4(Physics_obj* obj, double x_size, double y_size, double z_size, Vector3 pos,
                 float fric_0, float fric_1, float drag, float mass);
void drawPrism4(Prism4 prisma);

Vector3 getCoordinates(void* object, int shape);
void addCoordinates(void* object, int shape, Vector3 coordinates);
void setCoordinates(void* object, int shape, Vector3 coordinates);

double getDistFarthestPoint(void* object, int shape);
double getLowestPoint(void* object, int shape);
double getArea(void* object, int shape, int coord); // 0: X / 1: Y / 2: Z
double getHeight(void* object, int shape);

int hasCollidedPrism4ToPrism4(Prism4* prism1, Prism4* prism2, Vector3 prism1_n, Vector3 prism2_n);

#endif