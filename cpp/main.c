#include "raylib.h"
#include "stdlib.h"

#include "../h/simulation.h"
#include "../h/structs.h"

int main() {

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(1600, 900, "Testando");
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