#include "../h/physics.h"

void updateGravity(Physics_obj* obj) {
    obj->speed.y -= 9.8 / 75;
}

void updateFriction(Physics_obj* obj, Simulation* simulation) {
    if (!isGrounded(obj, simulation)) return;
    if (vector2Module((Vector2){obj->speed.x, obj->speed.z}) > 0.02) {
        obj->speed.x -= obj->fric[1] * obj->speed.x;
        obj->speed.z -= obj->fric[1] * obj->speed.z;
        return;
    }
    obj->speed.x -= obj->fric[0] * obj->speed.x;
    obj->speed.z -= obj->fric[0] * obj->speed.z;
}

void updateDrag(Physics_obj* obj) {
    double force_x = 0.5 * 1.225 * pow(obj->speed.x, 2) *
                  obj->drag * (getArea(obj->hitbox, obj->hitbox_shape, 0));
    double force_y = 0.5 * 1.225 * pow(obj->speed.y, 2) *
                  obj->drag * (getArea(obj->hitbox, obj->hitbox_shape, 1));
    double force_z = 0.5 * 1.225 * pow(obj->speed.x, 2) *
                  obj->drag * (getArea(obj->hitbox, obj->hitbox_shape, 2));

    if (obj->speed.x > 0) force_x *= -1;
    if (obj->speed.y > 0) force_y *= -1;
    if (obj->speed.z > 0) force_z *= -1;

    obj->speed.x += force_x/obj->mass;
    obj->speed.y += force_y/obj->mass;
    obj->speed.z += force_z/obj->mass;
}

void updatePhysics(Physics_obj* object, Simulation* simulation) {
    updateGravity(object);
    updateFriction(object, simulation);
    updateDrag(object);
}

int isGrounded(Physics_obj* object, Simulation* simulation) {
    if (isGroundedMap(object, simulation)) return 1;
    return isGroundedProp(object, simulation);
}
