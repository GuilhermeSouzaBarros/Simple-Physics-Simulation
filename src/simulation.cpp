#include "simulation.h"

Simulation::Simulation() {
    tick = 0.0f;
}

int Simulation::isGroundedMap(PhysicsObject* object, double delta) {
    for (int i = 0; i < 5; i++) {
        if (object->isGroundedMap(&map[i], delta)) return 1;
    }
    return 0;
}

int Simulation::isGrounded(PhysicsObject* object, double delta) {
    return isGroundedMap(object, delta);
}

void Simulation::updateMapCollision(PhysicsObject* object, double delta) {
    for (int i = 0; i < 5; i++) {
        object->updateMapCollision(&map[i], delta);
    }
}

void Simulation::updateMovement(double delta) {
    if (!isGrounded(&player.hitbox, delta)) return;

    if (IsKeyPressed(KEY_SPACE)) {
        player.hitbox.speed.y += 1.0f;
    }

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

    float direction = player.cam_ang.x + offset;
    if (direction > 360) {
        direction -= 360;
    }

    double speed = 1.5f;
    if (player.camera_mode == CAMERA_THIRD_PERSON) speed *= -1;
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        speed *= 2.0f;
    } else if (IsKeyDown(KEY_LEFT_CONTROL)) {
        speed /= 10.0f;
    }

    player.hitbox.speed.x -= cos(toRad(direction)) * speed;
    player.hitbox.speed.z -= sin(toRad(direction)) * speed;
}

void Simulation::updatePlayer(double delta) {
    updateMovement(delta);
};

void Simulation::updateGravity(PhysicsObject* object, double delta) {
    object->speed.y -= 9.8 * delta;
}

void Simulation::updateFriction(PhysicsObject* object, double delta) {
    if (!isGrounded(object, delta)) return;
    
    double friction = object->fric[0];
    if (vector2Module((Vector2){object->speed.x, object->speed.z}) > 0.02) {
        friction = object->fric[1];
    }
    object->speed.x *= (1.0 - friction);
    object->speed.z *= (1.0 - friction);
}

void Simulation::updateDrag(PhysicsObject* object, double delta) {
    double force_x = pow(object->speed.x, 2) * object->drag * object->area(0);
    double force_y = pow(object->speed.y, 2) * object->drag * object->area(1);
    double force_z = pow(object->speed.x, 2) * object->drag * object->area(2);

    if (object->speed.x > 0) force_x *= -1;
    if (object->speed.y > 0) force_y *= -1;
    if (object->speed.z > 0) force_z *= -1;

    object->speed.x += (force_x/object->mass) * delta;
    object->speed.y += (force_y/object->mass) * delta;
    object->speed.z += (force_z/object->mass) * delta;
}

void Simulation::updatePhysics(PhysicsObject* object, double delta) {
    updateGravity(object, delta);
    updateFriction(object, delta);
    updateDrag(object, delta);
}

void Simulation::updateCoords(PhysicsObject* object, double delta) {
    object->position = object->nextPosition(delta);
}

void Simulation::update(double delta) {
    mesh.stepMeshData(delta);
    updatePlayer(delta);
    updatePhysics(&player.hitbox, delta);
    updateMapCollision(&player.hitbox, delta);
    updateCoords(&player.hitbox, delta);
}

void Simulation::drawInfo() {
    int text_size = SCREEN_Y / 30;
    DrawText(TextFormat("POS: % 6.2f / % 6.2f / % 6.2f", player.hitbox.position.x, player.hitbox.position.y, player.hitbox.position.z), 10, 10, text_size, BLACK);
    DrawText(TextFormat("SPD: % 6.2f / % 6.2f / % 6.2f", player.hitbox.speed.x, player.hitbox.speed.y, player.hitbox.speed.z), 10, 40, text_size, BLACK);
    if (isGrounded(&player.hitbox, 0)) DrawText("Grounded", 10, 70, text_size, BLACK);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            DrawText(TextFormat("% 6.2f", mesh.transformation.values[i][j]), SCREEN_X - 700 + 160 * j, 10 + 50 * i, 2*text_size, BLACK);
        }
    }
}

void Simulation::draw() {
    player.updateCamera();
    player.swapped = 0;
    BeginDrawing();
    ClearBackground(LIGHTGRAY);

    BeginMode3D(player.camera);
        Vector3 tri1s1 = {20, 0, 20}, tri1s2 = {20, 0, -20}, tri1s3 = {-20, 0, 20};
        Vector3 tri2s1 = {-20, 0, -20}, tri2s2 = {-20, 0, 20}, tri2s3 = {20, 0, -20};
        DrawTriangle3D(tri1s1, tri1s2, tri1s3, WHITE);
        DrawTriangle3D(tri2s1, tri2s2, tri2s3, WHITE);
        DrawGrid(40, 1.0f);

        DrawCube((Vector3){21, 2.5, 0}, 2, 5, 40, ColorAlpha(BLUE, 0.5));
        DrawCubeWires((Vector3){21, 2.5, 0}, 2, 5, 40, BLACK);

        DrawCube((Vector3){0, 2.5, 21}, 40, 5, 2, ColorAlpha(GREEN, 0.5));
        DrawCubeWires((Vector3){0, 2.5, 21}, 40, 5, 2, BLACK);

        //if(player.camera_mode == CAMERA_THIRD_PERSON)
            player.hitbox.draw();

        mesh.drawMeshData();
    EndMode3D();
    drawInfo();
    
    EndDrawing();
}

int Simulation::hasCollidedPrisms(
    Prism *prism_1, Prism *prism_2,
    Vector3 *prism_1n, Vector3 *prism_2n
) {
    Vector3 speed_1 = (Vector3){prism_1n->x - prism_1->position.x,
                                prism_1n->y - prism_1->position.y,
                                prism_1n->z - prism_1->position.z};
    Vector3 speed_2 = (Vector3){prism_2n->x - prism_2->position.x,
                                prism_2n->y - prism_2->position.y,
                                prism_2n->z - prism_2->position.z};

    if (prism_1->rawRadius() + prism_2->rawRadius() +
        vector3Module(speed_1) + vector3Module(speed_2) + SMOLL_FLOAT <
        distance2Points(prism_1->position, prism_2->position)) {
        return 0;
    }

    if ((prism_1->x_size + prism_2->x_size)/2 + SMOLL_FLOAT < module(prism_1n->x - prism_2n->x) ||
        (prism_1->y_size + prism_2->y_size)/2 + SMOLL_FLOAT < module(prism_1n->y - prism_2n->y) ||
        (prism_1->z_size + prism_2->z_size)/2 + SMOLL_FLOAT < module(prism_1n->z - prism_2n->z)) {
        return 0;
    }
    return 1;
}
