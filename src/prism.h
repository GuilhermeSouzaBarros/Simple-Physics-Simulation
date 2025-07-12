#ifndef PRISM
#define PRISM

#include "physics_obj.h"

class Prism : virtual public PhysicsObject {
    public:
        float x_size;
        float y_size;
        float z_size;

        Prism(
            float fric_0, float fric_1, float drag,
            float mass,
            float x_size, float y_size, float z_size
        );
        
        void draw();

        int isGroundedMap(MapObject* map, double delta);
        double area(int coord);
        double rawRadius();
        double lowestY();
    
    private:
        int hasCollidedMap(MapObject* map, double delta);
        Vector3 distanceMapCollision(MapObject* map);
};

#endif