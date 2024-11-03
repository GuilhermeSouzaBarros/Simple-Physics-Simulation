#ifndef STRUCTS
#define STRUCTS

#define SCREEN_X 1600
#define SCREEN_Y 900
#define SMOLL_FLOAT 0.001953125f

#include "raylib.h"
#include "stdio.h"
#include "math.h"

typedef struct Triangle {
    Vector3 sides[3];
} Triangle;

int sign(double number);
int floatIsZero(float num);
void smallFloatToZero(float* num);
double module(double number);
double distance2Points(Vector3 p1, Vector3 p2);
double vector2Module(Vector2 vector);

double vector3Module(Vector3 vector);
Vector3 vector3Add(Vector3 v1, Vector3 v2);
Vector3 vector3Sub(Vector3 v1, Vector3 v2);

double vectorToAngle(Vector2 vector);

double toRad(double degree);
double toAngles(double radians);
double circleArea(double radius);

#endif