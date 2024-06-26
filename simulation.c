#include "simulation.h"

Simulation* setupSimulation() {
    Simulation* simulation = (Simulation*)malloc(sizeof(Simulation));
    simulation->player = setupPlayer();
    
    simulation->num_props = 1;
    simulation->props = (Physics_obj*)malloc(sizeof(Physics_obj) * simulation->num_props);
    setupPrism4(&simulation->props[0], 2, 2, 2, (Vector3){-5, 1, 0});

    simulation->num_maps = 5;
    simulation->map = (Map_obj*)malloc(sizeof(Map_obj) * simulation->num_maps);
    for (int i = 0; i < simulation->num_maps; i++) {
        setupMapObject(&simulation->map[i], i);
    }
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
    updatePlayer(simulation);
    updateProps(simulation);
    updateCoord(simulation);
    updateCamera(simulation->player);
}

void drawSimulation(Simulation* simulation) {
    BeginDrawing();
    ClearBackground(LIGHTGRAY);

    BeginMode3D(simulation->player->camera);
        Vector3 tri1s1 = {20, 0, 20}, tri1s2 = {20, 0, -20}, tri1s3 = {-20, 0, 20};
        Vector3 tri2s1 = {-20, 0, -20}, tri2s2 = {-20, 0, 20}, tri2s3 = {20, 0, -20};
        DrawTriangle3D(tri1s1, tri1s2, tri1s3, WHITE);
        DrawTriangle3D(tri2s1, tri2s2, tri2s3, WHITE);
        DrawGrid(40, 1.0f);

        DrawCube((Vector3){21, 20, 0}, 2, 40, 40, BLUE);
        DrawCubeWires((Vector3){21, 20, 0}, 2, 40, 40, BLACK);

        DrawCube((Vector3){0, 20, 21}, 40, 40, 2, GREEN);
        DrawCubeWires((Vector3){0, 20, 21}, 40, 40, 2, BLACK);

        Prism4 body = *(Prism4*)simulation->player->body.hitbox;
        
        DrawCubeWires(body.position, body.x_size, body.y_size, body.z_size, BLACK);

        DrawProps(simulation);
    EndMode3D();
    
    EndDrawing();
}
