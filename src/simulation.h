#ifndef SIMULATION
#define SIMULATION

#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"

#include "structs.h"
#include "player.h"
#include "map.h"

#include "mesh.h"
#include "prism.h"

class Simulation {
    public:
        float tick;

        Player player;
        MapObject map[5] = {
            {0, 2, 0, { 0, 0,  0}, {  0, 0,   0}},
            {1, 0, 0, {20, 0,  0}, {  0, 0,   0}},
            {2, 0, 0, { 0, 0,  0}, {-20, 0,   0}},
            {0, 0, 1, { 0, 0, 20}, {  0, 0,   0}},
            {0, 0, 2, { 0, 0,  0}, {  0, 0, -20}}
        };
        
        mesh::MeshData mesh;
        Simulation();
        void update(double delta);
        void draw();
    
    private:
        int isGroundedMap(PhysicsObject* object, double delta);
        int isGrounded(PhysicsObject* object, double delta);
        void updateMapCollision(PhysicsObject* object, double delta);

        void updateMovement(double delta);
        void updatePlayer(double delta);
        
        void updateGravity(PhysicsObject* object, double delta);
        void updateFriction(PhysicsObject* object, double delta);
        void updateDrag(PhysicsObject* object, double delta);
        void updatePhysics(PhysicsObject* object, double delta);
        
        void updateCoords(PhysicsObject* object, double delta);
        void drawInfo();

        int hasCollidedPrisms(
            Prism *prism_1, Prism *prism_2,
            Vector3 *prism_1n, Vector3 *prism_2n
        );
};

#endif