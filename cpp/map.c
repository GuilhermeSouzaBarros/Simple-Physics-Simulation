#include "../h/map.h"

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

void updateObjCollisionMap(Physics_obj* obj, Map_obj* map, CollInf* col) {
    if (!hasCollidedMap(obj, map)) return;
    Vector3 dist = distanceMapCollision(obj, map);
    col->col++;
    col->momentum = 0.0f;
    if (dist.x < 0) {
        col->gap_close = sign(obj->speed.x) * (module(obj->speed.x) - module(dist.x));
        obj->speed.x = col->gap_close;
    }
    if (dist.y < 0) {
        col->gap_close = sign(obj->speed.y) * (module(obj->speed.y) - module(dist.y));
        obj->speed.y = col->gap_close;
    }
    if (dist.z < 0) {
        col->gap_close = sign(obj->speed.z) * (module(obj->speed.z) - module(dist.z));
        obj->speed.z = col->gap_close;
    }
}

void updateObjCollisionsMap(Physics_obj* obj, Simulation* simulation, CollInf* col) {
    for (int i = 0; i < simulation->num_maps; i++) {
        updateObjCollisionMap(obj, &simulation->map[i], col);
    }
}

int isGroundedMap(Physics_obj* obj, Simulation* simulation) {
    Prism4* prism = (Prism4*)obj->hitbox;
    Vector3 prism_n = {prism->position.x + obj->speed.x,
                        prism->position.y + obj->speed.y - 0.05f,
                        prism->position.z + obj->speed.z};
    for (int i = 0; i < simulation->num_maps; i++) {
        if (isTouchingMap(obj, &simulation->map[i]) &&
            distanceMapCollisionAxis(prism_n.y, prism->y_size,
                simulation->map->start.y, simulation->map->end.y, simulation->map->y_bound) <= 0) {
            return 1;
        }
    }
    return 0;
}
