#ifndef MAP
#define MAP

#include "structs.h"

class MapObject {
    public:
        int x_bound; // 0: None / 1: Start / 2: End / 3: Both 
        int y_bound;
        int z_bound;
        Vector3 start;
        Vector3 end;
        MapObject(
            int x, int y, int z,
            Vector3 start, Vector3 end
        );
};

#endif