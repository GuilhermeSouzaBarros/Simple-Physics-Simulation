#ifndef STRUCTS
#define STRUCTS

#define SMOLL_FLOAT 0.001953125f
#define TICK 0.001f

#include "raylib.h"
#include "stdio.h"
#include "math.h"

typedef struct Triangle {
    Vector3 sides[3];
} Triangle;

extern int FULL_X, FULL_Y;
extern int SCREEN_X, SCREEN_Y;
extern int POS_X, POS_Y;

void setScreen();
void updateScreen();

int sign(double number);
int floatIsZero(float num);
void smallFloatToZero(float* num);
double module(double number);
double distance2Points(Vector3 p1, Vector3 p2);
double vector2Module(Vector2 vector);

double vectorToAngle(Vector2 vector);

double toRad(double degree);
double toAngles(double radians);
double circleArea(double radius);

double vector3Module(Vector3 vector);
Vector3 vector3Add(Vector3 v1, Vector3 v2);
Vector3 vector3Sub(Vector3 v1, Vector3 v2);
Vector3 vector3Product(Vector3 v1, Vector3 v2);
void setVector3ModuleToValue(Vector3 *p_vector, float value);
double dotProduct(Vector3 vector_1, Vector3 vector_2);
double project(Vector3 vector_1, Vector3 vector_2);
Vector3 removeProjection(Vector3 vector_1, Vector3 vector_2);
Vector3 crossProduct(Vector3 vector_1, Vector3 vector_2); //Produto Vetorial
void printVector3(Vector3 vector);

#endif