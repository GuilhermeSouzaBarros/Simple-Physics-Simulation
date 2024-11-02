#include "../h/simulation.h"

Simulation::Simulation() {
    num_props = 1;
    props = (Physics_obj*)malloc(sizeof(Physics_obj) * num_props);
    setupPrism4(&props[0], 2, 2, 2, (Vector3){-5, 1, 0});

    num_maps = 5;
    map = (Map_obj*)malloc(sizeof(Map_obj) * num_maps);
    for (int i = 0; i < num_maps; i++) {
        setupMapObject(&map[i], i);
    }
}

void Simulation::freeSim() {
    for (int i = 0; i < num_props; i++) {
        free(props[i].hitbox);
    }
    free(props);
    free(map);
}

int Simulation::isGroundedMap(Physics_obj* obj) {
    Prism4* prism = (Prism4*)obj->hitbox;
    Vector3 prism_n = {prism->position.x + obj->speed.x,
                        prism->position.y + obj->speed.y - 0.05f,
                        prism->position.z + obj->speed.z};
    for (int i = 0; i < num_maps; i++) {
        if (isTouchingMap(obj, &map[i]) &&
            distanceMapCollisionAxis(prism_n.y, prism->y_size,
                map->start.y, map->end.y, map->y_bound) <= 0) {
            return 1;
        }
    }
    return 0;
}

int Simulation::isGroundedProp(Physics_obj* obj) {
    Prism4* prism1 = (Prism4*)obj->hitbox;
    Vector3 prism1_n = {prism1->position.x + obj->speed.x,
                        prism1->position.y + obj->speed.y - 1,
                        prism1->position.z + obj->speed.z};
    for (int i = 0; i < num_props; i++) {
        if (props[i].hitbox == obj->hitbox) {
            continue;
        }
        Prism4* prism2 = (Prism4*)props[i].hitbox;
        Vector3 prism2_n = {prism2->position.x + props[i].speed.x,
                            prism2->position.y + props[i].speed.y,
                            prism2->position.z + props[i].speed.z};
        if (hasCollidedPrism4ToPrism4(prism1, prism2, prism1_n, prism2_n) &&
            module(prism2->position.y - prism1_n.y) - (prism1->y_size + prism2->y_size)/2 <= 0) {
            return 1;
        }
    }
    if (obj->hitbox != player.getBody()) {
        Prism4* prism2 = (Prism4*)player.getBody();
        Vector3 prism2_n = {prism2->position.x + player.getBody()->speed.x,
                            prism2->position.y + player.getBody()->speed.y,
                            prism2->position.z + player.getBody()->speed.z};
        if (hasCollidedPrism4ToPrism4(prism1, prism2, prism1_n, prism2_n) &&
            module(prism2->position.y - prism1_n.y) - (prism1->y_size + prism2->y_size)/2 <= 0) {
            return 1;
        }
    }
    return 0;
}

void Simulation::updateObjCollisionMap(Physics_obj* obj, Map_obj* map, CollInf* col) {
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

void Simulation::updateObjCollisionsMap(Physics_obj* obj, CollInf* col) {
    for (int i = 0; i < num_maps; i++) {
        updateObjCollisionMap(obj, &map[i], col);
    }
}

void Simulation::updateCollisionsPropsChain(Physics_obj* obj, Physics_obj* ign, CollInf* col) {
    for (int i = 0; i < num_props; i++) {
        if (props[i].hitbox == obj->hitbox) continue;
        if (props[i].hitbox == ign->hitbox) continue;
        updateCollisionProps(obj, &props[i], col);
    }
    if (obj->hitbox != player.getBody()) {
        updateCollisionProps(obj, player.getBody(), col);
    }
}

CollInf Simulation::updateCollisionsChain(Physics_obj* obj, Physics_obj* ign) {
    CollInf col = {0, 0.0f, 0.0f};
    updateObjCollisionsMap(obj, &col);
    updateCollisionsPropsChain(obj, ign, &col);
    return col;
}

void Simulation::momentumCollision(float* f_speed, float* t_speed, float dist,
                       float f_mass, float t_mass, Physics_obj* f_obj,
                       Physics_obj* t_obj) {  

    float ratio_f = *f_speed / (module(*f_speed) + module(*t_speed)),
    ratio_t = *t_speed / (module(*f_speed) + module(*t_speed));
    float fs_gap = ratio_f * dist, ts_gap = ratio_t * dist;
    *f_speed -= fs_gap;
    *t_speed -= ts_gap;
    float momentum = (*f_speed * f_mass + *t_speed * t_mass) / (f_mass + t_mass);

    *f_speed = fs_gap + momentum;
    *t_speed = ts_gap + momentum;
    
    CollInf f_chain = updateCollisionsChain(f_obj, t_obj);
    if (f_chain.col) {
        printf("%d / %f / %f\n", f_chain.col, f_chain.gap_close, f_chain.momentum);
        *t_speed = ts_gap + (sign(*t_speed) * module(fs_gap - f_chain.gap_close)) + f_chain.momentum;
    }
    CollInf t_chain = updateCollisionsChain(t_obj, f_obj);
    if (t_chain.col) {
        printf("%d / %f / %f\n", t_chain.col, t_chain.gap_close, t_chain.momentum);
        *f_speed = fs_gap + (sign(*f_speed) * module(ts_gap - t_chain.gap_close)) + t_chain.momentum;
    }
}

void Simulation::collisionPrism4ToPrism4(Physics_obj* from_obj, Physics_obj* to_obj, CollInf* col) {
    Prism4* prism1 = (Prism4*)from_obj->hitbox;
    Prism4* prism2 = (Prism4*)to_obj->hitbox;

    Vector3 prism1_n = {prism1->position.x + from_obj->speed.x,
                        prism1->position.y + from_obj->speed.y,
                        prism1->position.z + from_obj->speed.z};

    Vector3 prism2_n = {prism2->position.x + to_obj->speed.x,
                        prism2->position.y + to_obj->speed.y,
                        prism2->position.z + to_obj->speed.z};

    if (!hasCollidedPrism4ToPrism4(prism1, prism2, prism1_n, prism2_n)) return;
    
    Vector3 dist = {float(module(prism1->position.x - prism2->position.x) - (prism1->x_size + prism2->x_size)/2),
                    float(module(prism1->position.y - prism2->position.y) - (prism1->y_size + prism2->y_size)/2),
                    float(module(prism1->position.z - prism2->position.z) - (prism1->z_size + prism2->z_size)/2)};

    col->col++;

    if (dist.x >= -0.0001f) {
        momentumCollision(&from_obj->speed.x, &to_obj->speed.x, dist.x,
                          from_obj->mass, to_obj->mass, from_obj, to_obj);
    }
    if (dist.y >= -0.0001f) {
        momentumCollision(&from_obj->speed.y, &to_obj->speed.y, dist.y,
                          from_obj->mass, to_obj->mass, from_obj, to_obj);
    }
    if (dist.z >= -0.0001f) {
        momentumCollision(&from_obj->speed.z, &to_obj->speed.z, dist.z,
                        from_obj->mass, to_obj->mass, from_obj, to_obj);
    }
}

void Simulation::updateCollisionProps(Physics_obj* from_obj, Physics_obj* to_obj, CollInf* col) {
    switch(from_obj->hitbox_shape) {
        case 0:
            switch(to_obj->hitbox_shape) {
                case 0:
                    collisionPrism4ToPrism4(from_obj, to_obj, col);
            }
    }
}

void Simulation::updateCollisionsProps(Physics_obj* obj, CollInf* col) {
    for (int i = 0; i < num_props; i++) {
        if (props[i].hitbox == obj->hitbox) continue;
        updateCollisionProps(obj, &props[i], col);
    }
    if (obj->hitbox != player.getBody()) {
        updateCollisionProps(obj, player.getBody(), col);
    }
}

void Simulation::updateGravity(Physics_obj* obj) {
    obj->speed.y -= 9.8 / 75;
}

int Simulation::isGrounded(Physics_obj* object) {
    return isGroundedMap(object) || isGroundedProp(object);
}

void Simulation::updateFriction(Physics_obj* obj) {
    if (!isGrounded(obj)) return;
    if (vector2Module((Vector2){obj->speed.x, obj->speed.z}) > 0.02) {
        obj->speed.x -= obj->fric[1] * obj->speed.x;
        obj->speed.z -= obj->fric[1] * obj->speed.z;
        return;
    }
    obj->speed.x -= obj->fric[0] * obj->speed.x;
    obj->speed.z -= obj->fric[0] * obj->speed.z;
}

void Simulation::updateDrag(Physics_obj* obj) {
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

void Simulation::updatePhysics(Physics_obj* object) {
    updateGravity(object);
    updateFriction(object);
    updateDrag(object);
}

CollInf Simulation::updateCollisions(Physics_obj* obj) {
    CollInf col = {0, 0.0f, 0.0f};
    updateObjCollisionsMap(obj, &col);
    updateCollisionsProps(obj, &col);
    return col;
}

void Simulation::updateProps() {
    for(int i = 0; i < num_props; i++) {
        updatePhysics(&props[i]);
    }
    updatePhysics(player.getBody());
    
    CollInf col = {0, 0.0f, 0.0f};
    for(int i = 0; i < num_props; i++) {
        updateObjCollisionsMap(&props[i], &col);
    }
    updateObjCollisionsMap(player.getBody(), &col);

    for(int i = 0; i < num_props; i++) {
        updateCollisionsProps(&props[i], &col);
    }
    updateCollisionsProps(player.getBody(), &col);
}

void Simulation::updateCoords() {
    for (int i = 0; i < num_props; i++) {
        addCoordinates(props[i].hitbox, props[i].hitbox_shape,
        (Vector3){props[i].speed.x, props[i].speed.y, props[i].speed.z});
    }
    player.updateCoord();
}

void Simulation::updateMovement() {
    if (!isGrounded(player.getBody())) return;

    int walk = 0, strafe = 0;
    if (IsKeyDown(KEY_W)) walk++;
    if (IsKeyDown(KEY_A)) strafe--;
    if (IsKeyDown(KEY_S)) walk--;
    if (IsKeyDown(KEY_D)) strafe++;

    if (!(walk || strafe)) return;

    int offset = 0;
    if (walk && strafe) {
        offset = 180 - strafe * (45 * (2 + walk));
    } else if (strafe) {
        offset = 180 - 90 * strafe; 
    } else {
        offset =  90 - 90 * walk;
    }

    float direction = player.getCamAng()->x + offset;
    if (direction > 360) {
        direction -= 360;
    }

    double speed = 1.25/75;
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        speed *= 2;
    }

    player.getBody()->speed.x -= cos(toRad(direction)) * speed;
    player.getBody()->speed.z -= sin(toRad(direction)) * speed;
}

void Simulation::updateJump() {
    if (isGrounded(player.getBody())) {
        if (IsKeyPressed(KEY_SPACE)) {
            player.getBody()->speed.y += 1.0f;
        }
    }
}

void Simulation::updatePlayer() {
    updateMovement();
    updateJump();
};

void Simulation::update() {
    updatePlayer();
    updateProps();
    updateCoords();
    player.updateCamera();
}

void Simulation::DrawProps() {
    for (int i = 0; i < num_props; i++) {
        switch (props[i].hitbox_shape) {
            case 0: {
                Prism4 prisma = *(Prism4*)props[i].hitbox;
                drawPrism4(prisma);
                continue;
            }
            default: {
                continue;
            }
        }
    }
}

void Simulation::draw() {
    BeginDrawing();
    ClearBackground(LIGHTGRAY);

    BeginMode3D(*player.getCamera());
        Vector3 tri1s1 = {20, 0, 20}, tri1s2 = {20, 0, -20}, tri1s3 = {-20, 0, 20};
        Vector3 tri2s1 = {-20, 0, -20}, tri2s2 = {-20, 0, 20}, tri2s3 = {20, 0, -20};
        DrawTriangle3D(tri1s1, tri1s2, tri1s3, WHITE);
        DrawTriangle3D(tri2s1, tri2s2, tri2s3, WHITE);
        DrawGrid(40, 1.0f);

        DrawCube((Vector3){21, 20, 0}, 2, 40, 40, BLUE);
        DrawCubeWires((Vector3){21, 20, 0}, 2, 40, 40, BLACK);

        DrawCube((Vector3){0, 20, 21}, 40, 40, 2, GREEN);
        DrawCubeWires((Vector3){0, 20, 21}, 40, 40, 2, BLACK);

        Prism4 body = *(Prism4*)player.getBody()->hitbox;
        
        DrawCubeWires(body.position, body.x_size, body.y_size, body.z_size, BLACK);

        DrawProps();
    EndMode3D();
    
    EndDrawing();
}
