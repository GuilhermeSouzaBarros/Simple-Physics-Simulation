#ifndef PLAYER
#define PLAYER

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "structs.h"

class Player {
    public:
        Camera3D camera;
        int camera_mode;
        Vector2 cam_ang;
        float cam_dist;
        Vector3 position;
        Vector3 speed;

        Player();
        
        void updateCoord();
        void updateCamera();

    private:
        void cameraBound();
        void switchCamera();
        void updateCameraFP();
        void updateCameraTP();

};

#endif