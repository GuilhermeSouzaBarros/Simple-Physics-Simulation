#ifndef PLAYER
#define PLAYER

#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#include "shapes.h"

class Player {
    public:
        Camera3D camera;
        int camera_mode;
        Vector2 cam_ang;
        float cam_dist;
        Physics_obj body;
        Player();
        //void freePlayer();
        void updateCoord();
        Camera3D* getCamera();
        Vector2* getCamAng();
        Physics_obj* getBody();

        void updateCamera();

    private:
        void cameraBound();
        void switchCamera();
        void updateCameraFP();
        void updateCameraTP();

};

#endif