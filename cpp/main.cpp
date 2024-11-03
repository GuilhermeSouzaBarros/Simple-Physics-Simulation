#include "raylib.h"
#include "stdlib.h"

#include "../h/simulation.h"

int main() {

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(1440, 810, "Testando");
    DisableCursor();
    SetExitKey(KEY_DELETE);
    Simulation teste;

    SetTargetFPS(75);

    while(!WindowShouldClose()) {
        teste.update();
        teste.draw();
    }

    teste.freeSim();
    CloseWindow();
    return 0;
}