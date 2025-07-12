#include "prism.h"
#include "math.h"
#include "stdexcept"

Prism::Prism(
    float fric_0, float fric_1, float drag, float mass,
    float x_size, float y_size, float z_size
)
: PhysicsObject(fric_0, fric_1, drag, mass)
{
    this->x_size = x_size;
    this->y_size = y_size;
    this->z_size = z_size;
}

void Prism::draw() {
    Vector3 oriB = {position.x - x_size/2,
                   position.y - y_size/2,
                   position.z - z_size/2};
    
    Vector3 oriT = {position.x - x_size/2,
                   position.y + y_size/2,
                   position.z - z_size/2};

    Vector3 vertexB2 = {oriB.x + x_size, oriB.y, oriB.z}; 
    Vector3 vertexB3 = {vertexB2.x, oriB.y, vertexB2.z + z_size};
    Vector3 vertexB4 = {oriB.x, oriB.y, vertexB2.z + z_size};

    Vector3 vertexT2 = {oriT.x + x_size, oriT.y, oriT.z}; 
    Vector3 vertexT3 = {vertexT2.x, oriT.y, vertexT2.z + z_size};
    Vector3 vertexT4 = {oriT.x, oriT.y, vertexB2.z + z_size};

    Color color = GOLD;

    DrawTriangle3D(oriB, oriT, vertexB2, color);
    DrawTriangle3D(vertexB2, oriT, vertexT2, color);

    DrawTriangle3D(vertexB2, vertexT2, vertexB3, color);
    DrawTriangle3D(vertexB3, vertexT2, vertexT3, color);

    DrawTriangle3D(vertexB3, vertexT3, vertexB4, color);
    DrawTriangle3D(vertexB4, vertexT3, vertexT4, color);

    DrawTriangle3D(vertexB4, vertexT4, oriB, color);
    DrawTriangle3D(oriB, vertexT4, oriT, color);

    DrawTriangle3D(oriT, vertexT4, vertexT3, color);
    DrawTriangle3D(vertexT3, vertexT2, oriT, color);

    DrawTriangle3D(oriB, vertexB2, vertexB3, color);
    DrawTriangle3D(vertexB3, vertexB4, oriB, color);
    DrawCubeWires(position, x_size, y_size, z_size, BLACK);
}

double Prism::rawRadius() {
    return sqrt(pow(x_size, 2) + pow(y_size, 2) + pow(z_size, 2)) /2;
}

double Prism::lowestY() {
    return (position.y - y_size/2);
}

double Prism::area(int coord) {
    switch (coord) {
        case 0: {
            return (y_size * z_size);
        }
        case 1: {
            return (x_size * z_size);
        }
        case 2: {
            return (x_size * y_size);
        }
        default:
            throw std::runtime_error("Coordinate out of bounds");
    }
}

float distanceMapCollisionAxis(
    float pos, float size,
    float map_s, float map_e, int limit
) {
    if (limit == 0) return 0;
    if (limit == 1) return map_s - (pos + size/2.0f);
    if (limit == 2) return (pos - size/2.0f) - map_e;
    throw std::runtime_error("Limit above 2 is not defined\n");
}

int Prism::isGroundedMap(MapObject* map, double delta) {
    Vector3 n_position = position;
    position.y -= 0.01;
    int collided = hasCollidedMap(map, delta);
    position.y += 0.01;
    if (!collided) return 0;
    float distance_map = distanceMapCollisionAxis(
        n_position.y, y_size, map->start.y, map->end.y, map->y_bound);
    return distance_map <= SMOLL_FLOAT && map->y_bound;
}

int Prism::hasCollidedMap(MapObject* map, double delta) {
    Vector3 coll = {0, 0, 0};
    Vector3 n_position = nextPosition(delta);
    if (map->x_bound == 0 ||
       (map->x_bound == 1 && n_position.x + x_size/2 + SMOLL_FLOAT >= map->start.x) ||
       (map->x_bound == 2 && n_position.x - x_size/2 - SMOLL_FLOAT <= map->end.x) ||
       (map->x_bound == 3 && n_position.x + x_size/2 + SMOLL_FLOAT >= map->start.x &&
       n_position.x - x_size/2 - SMOLL_FLOAT <= map->end.x)) {
        coll.x = 1;
    }
    if (map->y_bound == 0 ||
       (map->y_bound == 1 && n_position.y + y_size/2 + SMOLL_FLOAT >= map->start.y) ||
       (map->y_bound == 2 && n_position.y - y_size/2 + SMOLL_FLOAT <= map->end.y) ||
       (map->y_bound == 3 && n_position.y + y_size/2 + SMOLL_FLOAT >= map->start.y &&
       n_position.y - y_size/2 - SMOLL_FLOAT <= map->end.y)) {
        coll.y = 1;
    }
    if (map->z_bound == 0 ||
       (map->z_bound == 1 && n_position.z + z_size/2 + SMOLL_FLOAT >= map->start.z) ||
       (map->z_bound == 2 && n_position.z - z_size/2 - SMOLL_FLOAT <= map->end.z) ||
       (map->z_bound == 3 && n_position.z + z_size/2 + SMOLL_FLOAT >= map->start.z &&
       n_position.z - z_size/2 - SMOLL_FLOAT <= map->end.z)) {
        coll.z = 1;
    }
    return coll.x && coll.y && coll.z;
}

Vector3 Prism::distanceMapCollision(MapObject* map) {
    Vector3 dist = {
        distanceMapCollisionAxis(position.x, x_size, map->start.x, map->end.x, map->x_bound),
        distanceMapCollisionAxis(position.y, y_size, map->start.y, map->end.y, map->y_bound),
        distanceMapCollisionAxis(position.z, z_size, map->start.z, map->end.z, map->z_bound)
    };
    return dist;
}
