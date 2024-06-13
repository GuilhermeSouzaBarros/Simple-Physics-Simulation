#include "physics.h"

int isGrounded(Physics_obj object) {
    return (getLowestPoint(object.hitbox, object.hitbox_shape) <= 0);
}

void updateGravity(Physics_obj* obj) {
    obj->speed.y -= 9.8 / 75;
}

void updateFriction(Physics_obj* obj) {
    float ground_distance = getLowestPoint(obj->hitbox, obj->hitbox_shape);
    if (ground_distance < -0.001 || ground_distance > 0.001) return;
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

void updateMapCollision(Physics_obj* obj, Simulation* simulation) {
    
}

void collisionPrism4ToPrism4(Physics_obj* from_obj, Physics_obj* to_obj) {
    Prism4* prism1 = (Prism4*)from_obj->hitbox;
    Prism4* prism2 = (Prism4*)to_obj->hitbox;

    if (getDistFarthestPoint(prism1, 0) + getDistFarthestPoint(prism2, 0) <= 
        distance2Points(prism1->position, prism2->position)) {
        return;
    }

    Vector3 prism1_n = {prism1->position.x + from_obj->speed.x,
                        prism1->position.y + from_obj->speed.y,
                        prism1->position.z + from_obj->speed.z};

    Vector3 prism2_n = {prism2->position.x + to_obj->speed.x,
                        prism2->position.y + to_obj->speed.y,
                        prism2->position.z + to_obj->speed.z};

    if ((prism1->x_size + prism2->x_size)/2 < module(prism2_n.x - prism1_n.x) ||
        (prism1->y_size + prism2->y_size)/2 < module(prism2_n.y - prism1_n.y) ||
        (prism1->z_size + prism2->z_size)/2 < module(prism2_n.z - prism1_n.z)) {
        return;
    }

    Vector3 dist = {module(prism2_n.x - prism1_n.x) - (prism1->x_size + prism2->x_size)/2,
                    module(prism2_n.y - prism1_n.y) - (prism1->y_size + prism2->y_size)/2,
                    module(prism2_n.z - prism1_n.z) - (prism1->z_size + prism2->z_size)/2};

    printf("Pris: %p\n", from_obj);
    printf("Dist; %f / %f / %f\n", dist.x, dist.y, dist.z);

    if (dist.x > dist.y && dist.x > dist.z) {
        float prism1_ns = (sign(from_obj->speed.x) * (module(from_obj->speed.x) - dist.x));
        float final_speed = (from_obj->mass*prism1_ns + to_obj->mass*to_obj->speed.x)/(from_obj->mass+to_obj->mass);
        from_obj->speed.x = final_speed;
        to_obj->speed.x = final_speed;
        return;
    }
    if (dist.y > dist.z) {
        float prism1_ns = (sign(from_obj->speed.y) * (module(from_obj->speed.y) - dist.y));
        float final_speed = (from_obj->mass*prism1_ns + to_obj->mass*to_obj->speed.z)/(from_obj->mass+to_obj->mass);
        from_obj->speed.z = final_speed;
        to_obj->speed.z = final_speed;
        return;
    }
    float prism1_ns = (sign(from_obj->speed.z) * (module(from_obj->speed.z) - dist.z));
    float final_speed = (from_obj->mass*prism1_ns + to_obj->mass*to_obj->speed.z)/(from_obj->mass+to_obj->mass);
    from_obj->speed.z = final_speed;
    to_obj->speed.z = final_speed;
}

void checkCollision(Physics_obj* from_obj, Physics_obj* to_obj) {
    switch(from_obj->hitbox_shape) {
        case 0:
            switch(to_obj->hitbox_shape) {
                case 0:
                    collisionPrism4ToPrism4(from_obj, to_obj);
                    return;
                case 1:
                    return;
            }
            return;
        case 1:
            switch(to_obj->hitbox_shape) {
                case 0:
                    return;
                case 1:
                    return;
            }
            return;
    }
}

void updatePropCollision(Physics_obj* obj, Simulation* simulation) {
    for (int i = 0; i < simulation->num_props; i++) {
        if (simulation->props[i].hitbox == obj->hitbox) {
            continue;
        }
        checkCollision(obj, &simulation->props[i]);
    }
    if (obj->hitbox != simulation->player->body.hitbox) {
        checkCollision(obj, &simulation->player->body);
    }
}

void updatePhysics(Physics_obj* object, Simulation* simulation) {
    updateFriction(object);
    updateDrag(object);
    updateMapCollision(object, simulation);
    updatePropCollision(object, simulation);
}

void updateCoord(Simulation* simulation) {
    for (int i = 0; i < simulation->num_props; i++) {
        addCoordinates(simulation->props[i].hitbox, simulation->props[i].hitbox_shape,
        (Vector3){simulation->props[i].speed.x, simulation->props[i].speed.y, simulation->props[i].speed.z});
    }
        addCoordinates(simulation->player->body.hitbox, simulation->player->body.hitbox_shape,
        (Vector3){simulation->player->body.speed.x, simulation->player->body.speed.y, simulation->player->body.speed.z});
}
