#ifndef STRUCTS
#define STRUCTS

#define SCREEN_X 1600
#define SCREEN_Y 900

#include "raylib.h"
#include "stdio.h"
#include "math.h"

#include "shapes.h"

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
    int x_limit; // 0: None / 1: Start / 2: End / 3: Both 
    int y_limit;
    int z_limit;
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

void setupPrism4(Physics_obj* obj, double x_size, double y_size, double z_size, Vector3 pos);

void drawPrism4(Prism4 prisma);

#endif