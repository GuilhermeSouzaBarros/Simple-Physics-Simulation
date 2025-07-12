#ifndef PLAYER
#define PLAYER

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "structs.h"
#include "prism.h"

class Player {
    public:
        Camera3D camera;
        Prism hitbox = {0.46f, 0.22f, 0.7f, 80.0f, 0.8f, 1.8f, 0.8f};
        int camera_mode;
        Vector2 cam_ang;
        float cam_dist;
        short int swapped;

        Player();
        void updateCamera();

    private:
        void cameraBound();
        void switchCamera();
        void updateCameraFP();
        void updateCameraTP();

};

#endif