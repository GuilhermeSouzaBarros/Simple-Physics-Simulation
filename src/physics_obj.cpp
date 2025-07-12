#include "physics_obj.h"


PhysicsObject::PhysicsObject(
    float fric_0, float fric_1, float drag,
    float mass
) {
    position = (Vector3){0, 1.00, 0};
    up = (Vector3){0, 1.0, 0};
    speed = (Vector3){0, 0, 0};

    fric = (float*)malloc(sizeof(float) * 2);
    fric[0] = fric_0;
    fric[1] = fric_1;
    this->drag = drag;
    this->mass = mass;
}

PhysicsObject::~PhysicsObject() {
    free(fric);
}

Vector3 PhysicsObject::nextPosition(double delta) {
    Vector3 new_position = position;
    new_position.x += speed.x * delta;
    new_position.y += speed.y * delta;
    new_position.z += speed.z * delta;
    return new_position;
}

void PhysicsObject::updateMapCollision(MapObject* map, double delta) {
    if (!hasCollidedMap(map, delta)) return;
    Vector3 dist = distanceMapCollision(map);
    
    if (abs(dist.x) - abs(speed.x) < SMOLL_FLOAT && map->x_bound) {
        speed.x = sign(speed.x) * dist.x;
        if (abs(speed.x) < SMOLL_FLOAT) speed.x = 0.0f;
    }

    if (abs(dist.y) - abs(speed.y) < SMOLL_FLOAT && map->y_bound) {
        speed.y = sign(speed.y) * dist.y;
        if (abs(speed.y) < SMOLL_FLOAT) speed.y = 0.0f;
    }

    if (abs(dist.z) - abs(speed.z) < SMOLL_FLOAT  && map->z_bound) {
        speed.z = sign(speed.z) * dist.z;
        if (abs(speed.z) < SMOLL_FLOAT) speed.z = 0.0f;
    }
}
