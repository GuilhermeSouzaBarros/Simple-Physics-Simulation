#include "raylib.h"
#include "stdlib.h"

#include "simulation.h"
#include "structs.h"

int main() {

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(960, 540, "Testando");
    DisableCursor();
    SetExitKey(KEY_DELETE);
    Simulation* teste = setupSimulation();

    SetTargetFPS(75);

    while(!WindowShouldClose()) {
        updateSimulation(teste);
        drawSimulation(teste);
    }

    freeSimulation(teste);
    CloseWindow();
    return 0;
}