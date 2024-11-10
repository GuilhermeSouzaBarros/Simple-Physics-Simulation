#include "raylib.h"
#include "stdlib.h"

#include "../h/simulation.h"

int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_X, SCREEN_Y, "Testando");
    DisableCursor();
    SetExitKey(KEY_DELETE);
    Simulation teste;

    SetTargetFPS(100);

    while(!WindowShouldClose()) {
        teste.tick += GetFrameTime();
        int updated = 0;
        while(teste.tick >= TICK) {
            teste.update();
            teste.tick -= TICK;
            updated++;
        }
        teste.player.swapped = 0;
        teste.draw();
    }

    CloseWindow();
    return 0;
}