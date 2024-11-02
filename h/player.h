#ifndef PLAYER
#define PLAYER

#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#include "shapes.h"

class Player {
    Camera3D camera;
    Vector2 cam_ang;
    Physics_obj body;

    public:
        Player();
        //void freePlayer();
        void updateCoord();
        Camera3D* getCamera();
        Vector2* getCamAng();
        Physics_obj* getBody();

        void updateCamera();

};

#endif