#include "../h/shapes.h"

void setupPrism4(Physics_obj* obj, double x_size, double y_size, double z_size, Vector3 pos) {
    Prism4* prisma = (Prism4*)malloc(sizeof(Prism4));
    prisma->x_size = x_size;
    prisma->y_size = y_size;
    prisma->z_size = z_size;
    prisma->position = pos;
    prisma->up = (Vector3){0.0f, 1.0f, 0.0f};
    obj->hitbox = (void*)prisma;
    obj->hitbox_shape = 0;
    obj->speed = (Vector3){0.0f, 0.0f, 0.0f};
    obj->fric[0] = 0.4f;
    obj->fric[1] = 0.6f;
    obj->drag = 0.5;
    obj->mass = 40;
}

void drawPrism4(Prism4 prisma) {
    Vector3 oriB = {prisma.position.x - prisma.x_size/2,
                   prisma.position.y - prisma.y_size/2,
                   prisma.position.z - prisma.z_size/2};
    
    Vector3 oriT = {prisma.position.x - prisma.x_size/2,
                   prisma.position.y + prisma.y_size/2,
                   prisma.position.z - prisma.z_size/2};

    Vector3 vertexB2 = {oriB.x + prisma.x_size, oriB.y, oriB.z}; 
    Vector3 vertexB3 = {vertexB2.x, oriB.y, vertexB2.z + prisma.z_size};
    Vector3 vertexB4 = {oriB.x, oriB.y, vertexB2.z + prisma.z_size};

    Vector3 vertexT2 = {oriT.x + prisma.x_size, oriT.y, oriT.z}; 
    Vector3 vertexT3 = {vertexT2.x, oriT.y, vertexT2.z + prisma.z_size};
    Vector3 vertexT4 = {oriT.x, oriT.y, vertexB2.z + prisma.z_size};

    DrawTriangle3D(oriB, oriT, vertexB2, GOLD);
    DrawTriangle3D(vertexB2, oriT, vertexT2, GOLD);

    DrawTriangle3D(vertexB2, vertexT2, vertexB3, GOLD);
    DrawTriangle3D(vertexB3, vertexT2, vertexT3, GOLD);

    DrawTriangle3D(vertexB3, vertexT3, vertexB4, GOLD);
    DrawTriangle3D(vertexB4, vertexT3, vertexT4, GOLD);

    DrawTriangle3D(vertexB4, vertexT4, oriB, GOLD);
    DrawTriangle3D(oriB, vertexT4, oriT, GOLD);

    DrawTriangle3D(oriT, vertexT4, vertexT3, GOLD);
    DrawTriangle3D(vertexT3, vertexT2, oriT, GOLD);

    DrawTriangle3D(oriB, vertexB2, vertexB3, GOLD);
    DrawTriangle3D(vertexB3, vertexB4, oriB, GOLD);
    DrawCubeWires(prisma.position, prisma.x_size, prisma.y_size, prisma.z_size, BLACK);
}

Vector3 getCoordinates(void* object, int shape) {
    switch (shape) {
        case 0:
            Prism4 prisma = *(Prism4*)object;
            return prisma.position;
        case 1:
            Cylinder cylinder = *(Cylinder*)object;
            return cylinder.position;
        default:
            return (Vector3){0, 0, 0};
    }
}

void addCoordinates(void* object, int shape, Vector3 coordinates) {
    switch (shape) {
        case 0:
            Prism4* prisma = (Prism4*)object;
            prisma->position.x += coordinates.x;
            prisma->position.y += coordinates.y;
            prisma->position.z += coordinates.z;
            return;
        case 1:
            Cylinder* cylinder = (Cylinder*)object;
            cylinder->position.x += coordinates.x;
            cylinder->position.y += coordinates.y;
            cylinder->position.z += coordinates.z;
            return;
    }
}

void setCoordinates(void* object, int shape, Vector3 coordinates) {
    switch (shape) {
        case 0:
            Prism4* prisma = (Prism4*)object;
            prisma->position.x = coordinates.x;
            prisma->position.y = coordinates.y;
            prisma->position.z = coordinates.z;
            return;
        case 1:
            Cylinder* hitbox = (Cylinder*)object;
            hitbox->position.x = coordinates.x;
            hitbox->position.y = coordinates.y;
            hitbox->position.z = coordinates.z;
            return;
    }
}

double getDistFarthestPoint(void* object, int shape) {
    switch (shape) {
        case 0:
            Prism4 prisma = *(Prism4*)object;
            return (sqrt(pow(prisma.x_size, 2) + pow(prisma.y_size, 2) + pow(prisma.z_size, 2)))/2;
        case 1:
            Cylinder cylinder = *(Cylinder*)object;
            return (sqrt(pow(cylinder.radius, 2) + pow(cylinder.height/2, 2)));
        default:
            return 0;
    }
}

double getLowestPoint(void* object, int shape) {
    switch (shape) {
        case 0:
            Prism4 prisma = *(Prism4*)object;
            return (prisma.position.y - prisma.y_size/2);
        case 1:
            Cylinder hitbox = *(Cylinder*)object;
            return (hitbox.position.y - hitbox.height/2);
        default:
            return 0;
    }
}

double getArea(void* object, int shape, int coord) {
    switch (shape) {
        case 0:
            Prism4 prisma = *(Prism4*)object;
            switch (coord) {
                case 0:
                    return (prisma.y_size * prisma.z_size);
                case 1:
                    return (prisma.x_size * prisma.z_size);
                case 2:
                    return (prisma.x_size * prisma.y_size);
            }
            return 0;
        case 1:
            Cylinder cylinder = *(Cylinder*)object;
            switch (coord) {
                case 1:
                    return (circleArea(cylinder.radius));
                default:
                    return (cylinder.height * cylinder.radius * 2);
            }
            return 0;
        default:
            return 0;
    }
}

double getHeight(void* object, int shape) {
    switch (shape) {
        case 0:
            Prism4 Prisma = *(Prism4*)object;
            return (Prisma.y_size);
        case 1:
            Cylinder cylinder = *(Cylinder*)object;
            return (cylinder.height);
        default:
            return 0;
    }
}

int hasCollidedPrism4ToPrism4(Prism4* prism1, Prism4* prism2, Vector3 prism1_n, Vector3 prism2_n) {
    if (getDistFarthestPoint(prism1, 0) + getDistFarthestPoint(prism2, 0) <
        distance2Points(prism1->position, prism2->position) + 0.0001f) {
        return 0;
    }

    if ((prism1->x_size + prism2->x_size)/2 < module(prism1_n.x - prism2_n.x) + 0.0001f ||
        (prism1->y_size + prism2->y_size)/2 < module(prism1_n.y - prism2_n.y) + 0.0001f ||
        (prism1->z_size + prism2->z_size)/2 < module(prism1_n.z - prism2_n.z) + 0.0001f) {
        return 0;
    }
    return 1;
}

int isGroundedProp(Physics_obj* object, Simulation* simulation) {
    Prism4* prism1 = (Prism4*)object->hitbox;
    Vector3 prism1_n = {prism1->position.x + object->speed.x,
                        prism1->position.y + object->speed.y - 1,
                        prism1->position.z + object->speed.z};
    for (int i = 0; i < simulation->num_props; i++) {
        if (simulation->props[i].hitbox == object->hitbox) {
            continue;
        }
        Prism4* prism2 = (Prism4*)simulation->props[i].hitbox;
        Vector3 prism2_n = {prism2->position.x + simulation->props[i].speed.x,
                            prism2->position.y + simulation->props[i].speed.y,
                            prism2->position.z + simulation->props[i].speed.z};
        if (hasCollidedPrism4ToPrism4(prism1, prism2, prism1_n, prism2_n) &&
            module(prism2->position.y - prism1_n.y) - (prism1->y_size + prism2->y_size)/2 <= 0) {
            return 1;
        }
    }
    if (object->hitbox != simulation->player->body.hitbox) {
        Prism4* prism2 = (Prism4*)simulation->player->body.hitbox;
        Vector3 prism2_n = {prism2->position.x + simulation->player->body.speed.x,
                            prism2->position.y + simulation->player->body.speed.y,
                            prism2->position.z + simulation->player->body.speed.z};
        if (hasCollidedPrism4ToPrism4(prism1, prism2, prism1_n, prism2_n) &&
            module(prism2->position.y - prism1_n.y) - (prism1->y_size + prism2->y_size)/2 <= 0) {
            return 1;
        }
    }
    return 0;
}

CollInf updateCollisions(Physics_obj* obj, Simulation* simulation) {
    CollInf col = {0, 0.0f, 0.0f};
    updateObjCollisionsMap(obj, simulation, &col);
    updateCollisionsProps(obj, simulation, &col);
    return col;
}


void updateCollisionsPropsChain(Physics_obj* obj, Physics_obj* ign, Simulation* simulation, CollInf* col) {
    for (int i = 0; i < simulation->num_props; i++) {
        if (simulation->props[i].hitbox == obj->hitbox) continue;
        if (simulation->props[i].hitbox == ign->hitbox) continue;
        updateCollisionProps(obj, &simulation->props[i], col, simulation);
    }
    if (obj->hitbox != simulation->player->body.hitbox) {
        updateCollisionProps(obj, &simulation->player->body, col, simulation);
    }
}

CollInf updateCollisionsChain(Physics_obj* obj, Physics_obj* ign, Simulation* simulation) {
    CollInf col = {0, 0.0f, 0.0f};
    updateObjCollisionsMap(obj, simulation, &col);
    updateCollisionsPropsChain(obj, ign, simulation, &col);
    return col;
}

void momentumCollision(float* f_speed, float* t_speed, float dist,
                       float f_mass, float t_mass, Physics_obj* f_obj,
                       Physics_obj* t_obj, Simulation* simulation) {  

    float ratio_f = *f_speed / (module(*f_speed) + module(*t_speed)),
    ratio_t = *t_speed / (module(*f_speed) + module(*t_speed));
    float fs_gap = ratio_f * dist, ts_gap = ratio_t * dist;
    *f_speed -= fs_gap;
    *t_speed -= ts_gap;
    float momentum = (*f_speed * f_mass + *t_speed * t_mass) / (f_mass + t_mass);

    *f_speed = fs_gap + momentum;
    *t_speed = ts_gap + momentum;
    
    CollInf f_chain = updateCollisionsChain(f_obj, t_obj, simulation);
    if (f_chain.col) {
        printf("%d / %f / %f\n", f_chain.col, f_chain.gap_close, f_chain.momentum);
        *t_speed = ts_gap + (sign(*t_speed) * module(fs_gap - f_chain.gap_close)) + f_chain.momentum;
    }
    CollInf t_chain = updateCollisionsChain(t_obj, f_obj, simulation);
    if (t_chain.col) {
        printf("%d / %f / %f\n", t_chain.col, t_chain.gap_close, t_chain.momentum);
        *f_speed = fs_gap + (sign(*f_speed) * module(ts_gap - t_chain.gap_close)) + t_chain.momentum;
    }
}

void collisionPrism4ToPrism4(Physics_obj* from_obj, Physics_obj* to_obj,
                             CollInf* col, Simulation* simulation) {
    Prism4* prism1 = (Prism4*)from_obj->hitbox;
    Prism4* prism2 = (Prism4*)to_obj->hitbox;

    Vector3 prism1_n = {prism1->position.x + from_obj->speed.x,
                        prism1->position.y + from_obj->speed.y,
                        prism1->position.z + from_obj->speed.z};

    Vector3 prism2_n = {prism2->position.x + to_obj->speed.x,
                        prism2->position.y + to_obj->speed.y,
                        prism2->position.z + to_obj->speed.z};

    if (!hasCollidedPrism4ToPrism4(prism1, prism2, prism1_n, prism2_n)) return;
    
    Vector3 dist = {module(prism1->position.x - prism2->position.x) - (prism1->x_size + prism2->x_size)/2,
                    module(prism1->position.y - prism2->position.y) - (prism1->y_size + prism2->y_size)/2,
                    module(prism1->position.z - prism2->position.z) - (prism1->z_size + prism2->z_size)/2};

    col->col++;

    if (dist.x >= -0.0001f) {
        momentumCollision(&from_obj->speed.x, &to_obj->speed.x, dist.x,
                          from_obj->mass, to_obj->mass, from_obj, to_obj, simulation);
    }
    if (dist.y >= -0.0001f) {
        momentumCollision(&from_obj->speed.y, &to_obj->speed.y, dist.y,
                          from_obj->mass, to_obj->mass, from_obj, to_obj, simulation);
    }
    if (dist.z >= -0.0001f) {
        momentumCollision(&from_obj->speed.z, &to_obj->speed.z, dist.z,
                        from_obj->mass, to_obj->mass, from_obj, to_obj, simulation);
    }
}

void updateCollisionProps(Physics_obj* from_obj, Physics_obj* to_obj,
                         CollInf* col, Simulation* simulation) {
    switch(from_obj->hitbox_shape) {
        case 0:
            switch(to_obj->hitbox_shape) {
                case 0:
                    collisionPrism4ToPrism4(from_obj, to_obj, col, simulation);
            }
    }
}

void updateCollisionsProps(Physics_obj* obj, Simulation* simulation, CollInf* col) {
    for (int i = 0; i < simulation->num_props; i++) {
        if (simulation->props[i].hitbox == obj->hitbox) continue;
        updateCollisionProps(obj, &simulation->props[i], col, simulation);
    }
    if (obj->hitbox != simulation->player->body.hitbox) {
        updateCollisionProps(obj, &simulation->player->body, col, simulation);
    }
}

void updateCoord(Simulation* simulation) {
    for (int i = 0; i < simulation->num_props; i++) {
        addCoordinates(simulation->props[i].hitbox, simulation->props[i].hitbox_shape,
        (Vector3){simulation->props[i].speed.x, simulation->props[i].speed.y, simulation->props[i].speed.z});
    }
        addCoordinates(simulation->player->body.hitbox, simulation->player->body.hitbox_shape,
        (Vector3){simulation->player->body.speed.x, simulation->player->body.speed.y, simulation->player->body.speed.z});
}
