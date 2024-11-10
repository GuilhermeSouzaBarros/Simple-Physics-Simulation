#ifndef MAP
#define MAP

#include "structs.h"

typedef struct Map_obj {
    int x_bound; // 0: None / 1: Start / 2: End / 3: Both 
    int y_bound;
    int z_bound;
    Vector3 start;
    Vector3 end;
} Map_obj;

void setupMapObject(Map_obj* object, int i);
float distanceMapCollisionAxis(float pos, float size, float map_s, float map_e, int limit);

#endif