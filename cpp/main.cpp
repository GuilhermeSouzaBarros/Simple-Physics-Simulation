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

    int debug = 0;
    while(!WindowShouldClose()) {
        if (IsKeyPressed(KEY_P)) {
            debug++;
            debug %= 2;
        }
        if (debug) {
            if (IsKeyPressed(KEY_F)) {
                teste.update();
            }
        } else {
            teste.update();
        }
        teste.draw();
    }

    teste.freeSim();
    CloseWindow();
    return 0;
}