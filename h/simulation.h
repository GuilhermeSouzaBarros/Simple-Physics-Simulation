#ifndef SIMULATION
#define SIMULATION

#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"

#include "structs.h"
#include "player.h"
#include "map.h"
#include "shapes.h"

class Simulation {
    public:
        Player player;
        Physics_obj* props;
        int num_props;
        Map_obj* map;
        int num_maps;
        Simulation();
        void freeSim();
        void update();
        void draw();

        int isGrounded(Physics_obj* object);

        void updatePhysics(Physics_obj* object);

        void updateObjCollisionMap(Physics_obj* obj, Map_obj* map);

        void updateCollisions(Physics_obj* obj);

        void momentumCollision(float* f_speed, float* t_speed, float dist,
                               float f_mass, float t_mass,
                               Physics_obj* f_obj, Physics_obj* t_obj);
        void collisionPrism4ToPrism4(Physics_obj* from_obj, Physics_obj* to_obj);

        void updatePlayer();

    private:
        int isGroundedMap(Physics_obj* obj);
        int isGroundedProp(Physics_obj* object);

        void updateGravity(Physics_obj* obj);
        void updateFriction(Physics_obj* obj);
        void updateDrag(Physics_obj* obj);

        void updateCollisionProps(Physics_obj* from_obj, Physics_obj* to_obj);
        void updateCollisionsProps(Physics_obj* obj);

        void updateObjCollisionsMap(Physics_obj* obj);

        void updateProps();
        void updateCoords();

        void drawInfo();
        void drawProps();

        void updateMovement();
        void updateJump();
};

#endif