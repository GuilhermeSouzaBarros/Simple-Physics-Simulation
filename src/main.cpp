#include "raylib.h"
#include "stdlib.h"

#include "structs.h"
#include "simulation.h"

int main() {
    setScreen();
    Simulation teste;

    SetTargetFPS(100);

    int debug = 0;
    while(!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) debug = !debug;
        if (!debug) {
            teste.tick += GetFrameTime();
        } else if (IsKeyDown(KEY_E)) {
            teste.tick += TICK;
        }
        int updated = 0;
        while(teste.tick >= TICK) {
            teste.update(TICK);
            teste.tick -= TICK;
            updated++;
        }
        updateScreen();
        teste.draw();
    }

    CloseWindow();
    return 0;
}