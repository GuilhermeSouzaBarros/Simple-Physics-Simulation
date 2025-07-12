#ifndef PHYSICS_OBJ
#define PHYSICS_OBJ

#include "raylib.h"

#include "map.h"

class PhysicsObject {
    public:
        Vector3 position;
        Vector3 up;
        Vector3 speed;
        float mass;
        float drag;
        float* fric;

        PhysicsObject(float fric_0, float fric_1, float drag, float mass);
        ~PhysicsObject();

        void updateMapCollision(MapObject* map, double delta);
        virtual int isGroundedMap(MapObject* map, double delta) = 0;
        virtual void draw() = 0;
        
        Vector3 nextPosition(double delta);
        virtual double area(int coord) = 0;
        virtual double rawRadius() = 0;
        virtual double lowestY() = 0;
        virtual int hasCollidedMap(MapObject* map, double delta) = 0;
        virtual Vector3 distanceMapCollision(MapObject* map) = 0;
};

#endif