#ifndef SHAPES
#define SHAPES

#include "raylib.h"
#include "math.h"
#include "stdlib.h"

typedef struct Triangle {
    Vector3 sides[3];
} Triangle;

typedef struct Prism4 {
    float x_size;
    float y_size;
    float z_size;
    Vector3 position;
    Vector3 up;
} Prism4;

typedef struct Cylinder {
    float radius;
    float height;
    Vector3 position;
    Vector3 up;
} Cylinder;

int sign(double number);
double module(double number);
double distance2Points(Vector3 p1, Vector3 p2);
double vector2Module(Vector2 vector);
double vector3Module(Vector3 vector);
double vectorToAngle(Vector2 vector);
double toRad(double degree);
double toAngles(double radians);
double circleArea(double radius);

Vector3 getCoordinates(void* object, int shape);
void addCoordinates(void* object, int shape, Vector3 coordinates);
void setCoordinates(void* object, int shape, Vector3 coordinates);

double getDistFarthestPoint(void* object, int shape);
double getLowestPoint(void* object, int shape);
double getArea(void* object, int shape, int coord); // 0: X / 1: Y / 2: Z
double getHeight(void* object, int shape);

#endif