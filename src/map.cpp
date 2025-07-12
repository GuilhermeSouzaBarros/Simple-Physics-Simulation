#include "map.h"

MapObject::MapObject(
    int x_bound, int y_bound, int z_bound,
    Vector3 start, Vector3 end
) {
    this->x_bound = x_bound;
    this->y_bound = y_bound;
    this->z_bound = z_bound;
    this->start = start;
    this->end = end;
}
