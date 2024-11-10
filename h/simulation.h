#ifndef SIMULATION
#define SIMULATION

#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"

#include "structs.h"
#include "player.h"
#include "map.h"

#include "mesh.h"

class Simulation {
    public:
        float tick;

        Player player;
        Map_obj* map;
        int num_maps;
        mesh::MeshData mesh;
        Simulation();
        ~Simulation();
        void update();
        void draw();

        void updatePlayer();

    private:
        void updateProps();
        void updateCoords();

        void drawInfo();
        void drawProps();

        void updateMovement();
        void updateJump();
};

#endif