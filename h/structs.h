#ifndef STRUCTS
#define STRUCTS

#define SCREEN_X 1600
#define SCREEN_Y 900

#include "raylib.h"
#include "stdio.h"
#include "math.h"

typedef struct CollInf {
    int col;
    float gap_close;
    float momentum;
} CollInf;

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

typedef struct Physics_obj {
    void* hitbox;
    int hitbox_shape; // 0: Prisma 4 sided base / 1: Cylinder
    Vector3 speed;
    float drag;
    float mass;
    float fric[2]; // 0: Stationary / 1: Moving
} Physics_obj;

typedef struct Player {
    Camera3D camera;
    Vector2 cam_ang;
    Physics_obj body;
} Player;

typedef struct Map_obj {
    int x_bound; // 0: None / 1: Start / 2: End / 3: Both 
    int y_bound;
    int z_bound;
    Vector3 start;
    Vector3 end;
} Map_obj;

typedef struct Simulation {
    Player* player;
    Physics_obj* props;
    int num_props;
    Map_obj* map;
    int num_maps;
} Simulation;

int sign(double number);
double module(double number);
double distance2Points(Vector3 p1, Vector3 p2);
double vector2Module(Vector2 vector);
double vector3Module(Vector3 vector);
double vectorToAngle(Vector2 vector);
double toRad(double degree);
double toAngles(double radians);
double circleArea(double radius);

#endif