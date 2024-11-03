#include "../h/simulation.h"

Simulation::Simulation() {
    num_props = 2;
    props = (Physics_obj*)malloc(sizeof(Physics_obj) * num_props);
    setupPrism4(&props[0], 2, 2, 2, (Vector3){-5, 2, 0}, 0.4, 0.6, 0.5, 80);
    setupPrism4(&props[1], 5, 1, 2, (Vector3){5, 3, 0}, 0.4, 0.6, 0.5, 100);

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
    Prism4* obj_prism = (Prism4*)obj->hitbox;
    Prism4 prism = {obj_prism->x_size, obj_prism->y_size, obj_prism->z_size, obj_prism->position, obj_prism->up};
    prism.position.y -= 4.0f * SMOLL_FLOAT;

    Physics_obj n_obj = {(void*)(&prism), 0, obj->speed, obj->drag, obj->mass, {obj->fric[0], obj->fric[1]}};
    
    for (int i = 0; i < num_maps; i++) {
        if (hasCollidedMap(&n_obj, &map[i]) &&
            distanceMapCollisionAxis(prism.position.y, prism.y_size,
                map->start.y, map->end.y, map->y_bound) <= SMOLL_FLOAT) {
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

int Simulation::isGrounded(Physics_obj* object) {
    return isGroundedMap(object) || isGroundedProp(object);
}


void Simulation::updateObjCollisionMap(Physics_obj* obj, Map_obj* map) {
    if (!hasCollidedMap(obj, map)) return;
    Vector3 dist = distanceMapCollision(obj, map);
    
    if (dist.x + obj->speed.x < SMOLL_FLOAT && map->x_bound) {
        obj->speed.x = sign(obj->speed.x) * dist.x;
        if ( -SMOLL_FLOAT < obj->speed.x && obj->speed.x < SMOLL_FLOAT) obj->speed.x = 0.0f;
    }

    if (dist.y + obj->speed.y < SMOLL_FLOAT && map->y_bound) {
        obj->speed.y = sign(obj->speed.y) * dist.y;
        if ( -SMOLL_FLOAT < obj->speed.y && obj->speed.y < SMOLL_FLOAT) obj->speed.y = 0.0f;
    }

    if (dist.z + obj->speed.z < SMOLL_FLOAT && map->z_bound) {
        obj->speed.z = sign(obj->speed.z) * dist.z;
        if ( -SMOLL_FLOAT < obj->speed.z && obj->speed.z < SMOLL_FLOAT) obj->speed.z = 0.0f;
    }
}

void Simulation::updateObjCollisionsMap(Physics_obj* obj) {
    for (int i = 0; i < num_maps; i++) {
        updateObjCollisionMap(obj, &map[i]);
    }
}


void Simulation::momentumCollision(float* f_speed, float* t_speed, float dist,
                       float f_mass, float t_mass, Physics_obj* f_obj,
                       Physics_obj* t_obj) {  
    if (floatIsZero(*t_speed + *f_speed)) {
        printf("Collision with 0 speed dumb\n");
        *t_speed = 0.0f;
        *f_speed = 0.0f;
        return;
    }
    printf("Spds: %f / %f\n", *f_speed, *t_speed);
    float ratio_f = *f_speed / (module(*f_speed) + module(*t_speed)),
          ratio_t = *t_speed / (module(*f_speed) + module(*t_speed));
    printf("Rtos: %f / %f\n", ratio_f, ratio_t);
    printf("Dist: %f\n", dist);
    float fs_gap = ratio_f * dist,
          ts_gap = ratio_t * dist;
    smallFloatToZero(&fs_gap);
    smallFloatToZero(&ts_gap);

    *f_speed -= fs_gap;
    *t_speed -= ts_gap;

    float momentum = (*f_speed * f_mass + *t_speed * t_mass) / (f_mass + t_mass);

    *f_speed = fs_gap + momentum;
    *t_speed = ts_gap + momentum;
    *f_speed = sign(*f_speed) * (module(*f_speed) - SMOLL_FLOAT/2.0f);
    if (floatIsZero(*f_speed)) {
        smallFloatToZero(f_speed);
    }
    printf("Fspd: %f / %f\n", *f_speed, *t_speed);
}

void Simulation::collisionPrism4ToPrism4(Physics_obj* from_obj, Physics_obj* to_obj) {
    Prism4* prism1 = (Prism4*)from_obj->hitbox;
    Prism4* prism2 = (Prism4*)to_obj->hitbox;

    Vector3 prism1_n = {prism1->position.x + from_obj->speed.x,
                        prism1->position.y + from_obj->speed.y,
                        prism1->position.z + from_obj->speed.z};

    Vector3 prism2_n = {prism2->position.x + to_obj->speed.x,
                        prism2->position.y + to_obj->speed.y,
                        prism2->position.z + to_obj->speed.z};

    if (!hasCollidedPrism4ToPrism4(prism1, prism2, prism1_n, prism2_n)) return;
    
    Vector3 dist = {float( module(prism1->position.x - prism2->position.x) - (prism1->x_size + prism2->x_size) / 2.0f),
                    float( module(prism1->position.y - prism2->position.y) - (prism1->y_size + prism2->y_size) / 2.0f),
                    float( module(prism1->position.z - prism2->position.z) - (prism1->z_size + prism2->z_size) / 2.0f)};

    if (dist.x > 0) {
        momentumCollision(&from_obj->speed.x, &to_obj->speed.x, dist.x,
                          from_obj->mass, to_obj->mass, from_obj, to_obj);
    }
    if (dist.y > 0) {
        momentumCollision(&from_obj->speed.y, &to_obj->speed.y, dist.y,
                          from_obj->mass, to_obj->mass, from_obj, to_obj);
    }
    if (dist.z > 0) {
        momentumCollision(&from_obj->speed.z, &to_obj->speed.z, dist.z,
                        from_obj->mass, to_obj->mass, from_obj, to_obj);
    }
}

void Simulation::updateCollisionProps(Physics_obj* from_obj, Physics_obj* to_obj) {
    switch(from_obj->hitbox_shape) {
        case 0:
            switch(to_obj->hitbox_shape) {
                case 0:
                    collisionPrism4ToPrism4(from_obj, to_obj);
            }
    }
}

void Simulation::updateCollisionsProps(Physics_obj* obj) {
    for (int i = 0; i < num_props; i++) {
        if (&props[i] == obj) continue;
        updateCollisionProps(obj, &props[i]);
    }
    if (obj != player.getBody()) {
        updateCollisionProps(obj, player.getBody());
    }
}


void Simulation::updateGravity(Physics_obj* obj) {
    obj->speed.y -= 9.8 / 75;
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
    if (player.camera_mode == CAMERA_THIRD_PERSON) speed *= -1;
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        speed *= 2.0f;
    } else if (IsKeyDown(KEY_LEFT_CONTROL)) {
        speed /= 10.0f;
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


void Simulation::updateCollisions(Physics_obj* obj) {
    updateObjCollisionsMap(obj);
    updateCollisionsProps(obj);
}

void Simulation::updateProps() {
    for(int i = 0; i < num_props; i++) {
        updatePhysics(&props[i]);
    }
    updatePhysics(player.getBody());
    
    for(int i = 0; i < num_props; i++) {
        updateObjCollisionsMap(&props[i]);
    }
    updateObjCollisionsMap(player.getBody());

    for(int i = 0; i < num_props; i++) {
        updateCollisionsProps(&props[i]);
    }
    updateCollisionsProps(player.getBody());
}

void Simulation::updateCoords() {
    for (int i = 0; i < num_props; i++) {
        addCoordinates(props[i].hitbox, props[i].hitbox_shape,
        (Vector3){props[i].speed.x, props[i].speed.y, props[i].speed.z});
    }
    player.updateCoord();
}

void Simulation::update() {
    printf("\nNew Frame\n");
    updatePlayer();
    updateProps();
    updateCoords();
    player.updateCamera();
}


void Simulation::drawProps() {
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

void Simulation::drawInfo() {
    Physics_obj* body = player.getBody();
    DrawText(TextFormat("POS: %.2f / %.2f / %.2f", ((Prism4*)body->hitbox)->position.x, ((Prism4*)body->hitbox)->position.y, ((Prism4*)body->hitbox)->position.z), 10, 10, 30, BLACK);
    DrawText(TextFormat("SPD: %.2f / %.2f / %.2f", body->speed.x, body->speed.y, body->speed.z), 10, 40, 30, BLACK);
    if (isGrounded(player.getBody())) DrawText("Grounded", 10, 70, 30, BLACK);
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

        drawProps();
    EndMode3D();
    drawInfo();
    
    EndDrawing();
}
