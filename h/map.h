#ifndef MAP
#define MAP

#include "structs.h"
#include "shapes.h"

typedef struct Physics_obj Physics_obj;

typedef struct Map_obj {
    int x_bound; // 0: None / 1: Start / 2: End / 3: Both 
    int y_bound;
    int z_bound;
    Vector3 start;
    Vector3 end;
} Map_obj;

void setupMapObject(Map_obj* object, int i);
int isTouchingMap(Physics_obj* obj, Map_obj* map);
int hasCollidedMap(Physics_obj* obj, Map_obj* map);
float distanceMapCollisionAxis(float pos, float size, float map_s, float map_e, int limit);
Vector3 distanceMapCollision(Physics_obj* obj, Map_obj* map);

#endif