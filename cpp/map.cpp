#include "../h/map.h"

void setupMapObject(Map_obj* object, int i) {
    switch (i) {
        case 0:
            object->x_bound = 0;
            object->y_bound = 2;
            object->z_bound = 0;
            object->start = (Vector3){0, 0, 0};
            object->end = (Vector3){0, 0, 0};
            break;
        case 1:
            object->x_bound = 1;
            object->y_bound = 0;
            object->z_bound = 0;
            object->start = (Vector3){20, 0, 0};
            object->end = (Vector3){0, 0, 0};
            break;
        case 2:
            object->x_bound = 2;
            object->y_bound = 0;
            object->z_bound = 0;
            object->start = (Vector3){0, 0, 0};
            object->end = (Vector3){-20, 0, 0};
            break;
        case 3:
            object->x_bound = 0;
            object->y_bound = 0;
            object->z_bound = 1;
            object->start = (Vector3){0, 0, 20};
            object->end = (Vector3){0, 0, 0};
            break;
        case 4:
            object->x_bound = 0;
            object->y_bound = 0;
            object->z_bound = 2;
            object->start = (Vector3){0, 0, 0};
            object->end = (Vector3){0, 0, -20};
            break;
    }
}

int isTouchingMap(Physics_obj* obj, Map_obj* map) {
    Vector3 coll = {0, 0, 0};
    Prism4 prism = *(Prism4*)obj->hitbox;
    prism.position.x += obj->speed.x;
    prism.position.y += obj->speed.y;
    prism.position.z += obj->speed.z;

    if (map->x_bound == 0 ||
       (map->x_bound == 1 && prism.position.x + prism.x_size/2 >= map->start.x) ||
       (map->x_bound == 2 && prism.position.x - prism.x_size/2 <= map->end.x) ||
       (map->x_bound == 3 && prism.position.x + prism.x_size/2 >= map->start.x &&
       prism.position.x - prism.x_size/2 <= map->end.x)) {
        coll.x = 1;
    }
    if (map->y_bound == 0 ||
       (map->y_bound == 1 && prism.position.y + prism.y_size/2 >= map->start.y) ||
       (map->y_bound == 2 && prism.position.y - prism.y_size/2 <= map->end.y) ||
       (map->y_bound == 3 && prism.position.y + prism.y_size/2 >= map->start.y &&
       prism.position.y - prism.y_size/2 <= map->end.y)) {
        coll.y = 1;
    }
    if (map->z_bound == 0 ||
       (map->z_bound == 1 && prism.position.z + prism.z_size/2 >= map->start.z) ||
       (map->z_bound == 2 && prism.position.z - prism.z_size/2 <= map->end.z) ||
       (map->z_bound == 3 && prism.position.z + prism.z_size/2 >= map->start.z &&
       prism.position.z - prism.z_size/2 <= map->end.z)) {
        coll.z = 1;
    }
    if (!(coll.x && coll.y && coll.z)) {
        return 0;
    }
    return 1;

}

int hasCollidedMap(Physics_obj* obj, Map_obj* map) {
    Vector3 coll = {0, 0, 0};
    Prism4 prism = *(Prism4*)obj->hitbox;
    prism.position.x += obj->speed.x;
    prism.position.y += obj->speed.y;
    prism.position.z += obj->speed.z;

    if (map->x_bound == 0 ||
       (map->x_bound == 1 && prism.position.x + (prism.x_size/2 - 0.0001f) > map->start.x) ||
       (map->x_bound == 2 && prism.position.x - (prism.x_size/2 - 0.0001f) < map->end.x) ||
       (map->x_bound == 3 && prism.position.x + (prism.x_size/2 - 0.0001f) > map->start.x &&
       prism.position.x - (prism.x_size/2 - 0.0001f) < map->end.x)) {
        coll.x = 1;
    }
    if (map->y_bound == 0 ||
       (map->y_bound == 1 && prism.position.y + (prism.y_size/2 - 0.0001f) > map->start.y) ||
       (map->y_bound == 2 && prism.position.y - (prism.y_size/2 - 0.0001f) < map->end.y) ||
       (map->y_bound == 3 && prism.position.y + (prism.y_size/2 - 0.0001f) > map->start.y &&
       prism.position.y - (prism.y_size/2 - 0.0001f) < map->end.y)) {
        coll.y = 1;
    }
    if (map->z_bound == 0 ||
       (map->z_bound == 1 && prism.position.z + (prism.z_size/2 - 0.0001f) > map->start.z) ||
       (map->z_bound == 2 && prism.position.z - (prism.z_size/2 - 0.0001f) < map->end.z) ||
       (map->z_bound == 3 && prism.position.z + (prism.z_size/2 - 0.0001f) > map->start.z &&
       prism.position.z - (prism.z_size/2 - 0.0001f) < map->end.z)) {
        coll.z = 1;
    }
    if (!(coll.x && coll.y && coll.z)) {
        return 0;
    }
    return 1;
}

float distanceMapCollisionAxis(float pos, float size, float map_s, float map_e, int limit) {
    if (limit == 0) return 0;
    if (limit == 1) return map_s - (pos + size/2);
    if (limit == 2) return (pos - size/2) - map_e;
    printf("Error: limit 3 fell\n");
    return 0;
}

Vector3 distanceMapCollision(Physics_obj* obj, Map_obj* map) {
    Prism4 prism = *(Prism4*)obj->hitbox;
    prism.position.x += obj->speed.x;
    prism.position.y += obj->speed.y;
    prism.position.z += obj->speed.z;
    Vector3 dist = {
        distanceMapCollisionAxis(prism.position.x, prism.x_size, map->start.x, map->end.x, map->x_bound),
        distanceMapCollisionAxis(prism.position.y, prism.y_size, map->start.y, map->end.y, map->y_bound),
        distanceMapCollisionAxis(prism.position.z, prism.z_size, map->start.z, map->end.z, map->z_bound)};
    return dist;
}
