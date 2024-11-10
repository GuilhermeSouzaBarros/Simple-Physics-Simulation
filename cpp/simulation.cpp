#include "../h/simulation.h"

Simulation::Simulation() {
    tick = 0.0f;
    num_maps = 5;
    map = (Map_obj*)malloc(sizeof(Map_obj) * num_maps);
    for (int i = 0; i < num_maps; i++) {
        setupMapObject(&map[i], i);
    }
}

Simulation::~Simulation() {
    free(map);
    mesh.freeMeshData();
}

void Simulation::updateMovement() {
    if (IsKeyDown(KEY_SPACE)) {
        player.speed.x = 0;
        player.speed.z = 0;
        return;
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

    double speed = 2.5f * TICK;
    if (player.camera_mode == CAMERA_THIRD_PERSON) speed *= -1;
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        speed *= 2.0f;
    } else if (IsKeyDown(KEY_LEFT_CONTROL)) {
        speed /= 10.0f;
    }

    player.speed.x -= cos(toRad(direction)) * speed;
    player.speed.z -= sin(toRad(direction)) * speed;

    player.speed.x -= sign(player.speed.x) * 16 * (pow(player.speed.x, 2));
    player.speed.z -= sign(player.speed.z) * 16 * (pow(player.speed.z, 2));
}


void Simulation::updatePlayer() {
    updateMovement();
    player.updateCoord();
};


void Simulation::update() {
    mesh.stepMeshData(TICK);
    updatePlayer();
    player.updateCamera();
}

void Simulation::drawInfo() {
    DrawText(TextFormat("POS: %.2f / %.2f / %.2f", player.position.x, player.position.y, player.position.z), 10, 10, 30, BLACK);
    DrawText(TextFormat("SPD: %.2f / %.2f / %.2f", player.speed.x, player.speed.y, player.speed.z), 10, 40, 30, BLACK);
}

void Simulation::draw() {
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

        mesh.drawMeshData();
    EndMode3D();
    drawInfo();
    
    EndDrawing();
}
