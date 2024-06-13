#include "simulation.h"

Simulation* setupSimulation() {
    Simulation* simulation = (Simulation*)malloc(sizeof(Simulation));
    simulation->player = setupPlayer();
    
    simulation->num_props = 1;
    simulation->props = (Physics_obj*)malloc(sizeof(Physics_obj) * simulation->num_props);
    setupPrism4(&simulation->props[0], 2, 2, 2, (Vector3){-5, 1, 0});

    simulation->num_maps = 1;
    simulation->map = (Map_obj*)malloc(sizeof(Map_obj) * simulation->num_maps);
    setupMapObject(&simulation->map[0]);

    return simulation;
}

void freeSimulation(Simulation* simulation) {
    freePlayer(simulation->player);
    for (int i = 0; i < simulation->num_props; i++) {
        free(simulation->props[i].hitbox);
    }
    free(simulation->props);
    free(simulation->map);
    free(simulation);
}

void updateSimulation(Simulation* simulation) {
    updateProps(simulation);
    updatePlayer(simulation);
    updateCoord(simulation);
}

void drawSimulation(Simulation* simulation) {
    BeginDrawing();
    ClearBackground(LIGHTGRAY);

    BeginMode3D(simulation->player->camera);
        Vector3 tri1s1 = {5, 0, 5}, tri1s2 = {5, 0, -5}, tri1s3 = {-5, 0, 5};
        Vector3 tri2s1 = {-5, 0, -5}, tri2s2 = {-5, 0, 5}, tri2s3 = {5, 0, -5};
        DrawTriangle3D(tri1s1, tri1s2, tri1s3, WHITE);
        DrawTriangle3D(tri2s1, tri2s2, tri2s3, WHITE);
        DrawGrid(100, 1.0f);

        DrawCube((Vector3){5, 0, 0}, 2, 2, 2, BLUE);
        DrawCubeWires((Vector3){5, 0, 0}, 2, 2, 2, BLACK);

        DrawCube((Vector3){0, 0, 5}, 2, 2, 2, GREEN);
        DrawCubeWires((Vector3){0, 0, 5}, 2, 2, 2, BLACK);

        DrawCube((Vector3){0, 5, 0}, 2, 2, 2, RED);
        DrawCubeWires((Vector3){0, 5, 0}, 2, 2, 2, BLACK);

        DrawCube((Vector3){0, 2.5, -5}, 5, 5, 2, BROWN);
        DrawCubeWires((Vector3){0, 2.5, -5}, 5, 5, 2, RED);

        Player player = *(simulation->player);
        Vector3 position = getCoordinates(player.body.hitbox, player.body.hitbox_shape);

        DrawTriangle3D((Vector3){position.x - 0.5, position.y - 0.89, position.z - 0.5},
                       (Vector3){position.x + 0.5, position.y - 0.89, position.z + 0.5},
                       (Vector3){position.x + 0.5, position.y - 0.89, position.z - 0.5}, GRAY);

        DrawTriangle3D((Vector3){position.x - 0.5, position.y - 0.89, position.z - 0.5},
                       (Vector3){position.x - 0.5, position.y - 0.89, position.z + 0.5},
                       (Vector3){position.x + 0.5, position.y - 0.89, position.z + 0.5}, GRAY);

        DrawProps(simulation);
    EndMode3D();

    DrawText(TextFormat("Pos: %7.2f / %7.2f / %7.2f", position.x,
                position.y, position.z), 10, 10, 30, BLACK);
    DrawText(TextFormat("Tar: %7.2f / %7.2f / %7.2f", player.camera.target.x,
                player.camera.target.y, player.camera.target.z), 10, 50, 30, BLACK);
    DrawText(TextFormat("Spd: %7.2f / %7.2f / %7.2f", player.body.speed.x,
                player.body.speed.y, player.body.speed.z), 10, 90, 30, BLACK);
    if(isGrounded(player.body)) DrawText("Grounded!", 10, 170, 30, BLACK);

    DrawText(TextFormat("Frame Time: %5.4f", GetFrameTime()), 1300, 5, 30, BLUE);

    EndDrawing();
}
