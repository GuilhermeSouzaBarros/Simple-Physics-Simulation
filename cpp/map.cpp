#include "../h/map.h"

void setupMapObject(Map_obj* object, int i) {
    switch (i) {
        case 0:
            object->x_bound = 0;
            object->y_bound = 2;
            object->z_bound = 0;
            object->start = (Vector3){0, 0, 0};
            object->end = (Vector3){0, 0, 0};
            break;
        case 1:
            object->x_bound = 1;
            object->y_bound = 0;
            object->z_bound = 0;
            object->start = (Vector3){20, 0, 0};
            object->end = (Vector3){0, 0, 0};
            break;
        case 2:
            object->x_bound = 2;
            object->y_bound = 0;
            object->z_bound = 0;
            object->start = (Vector3){0, 0, 0};
            object->end = (Vector3){-20, 0, 0};
            break;
        case 3:
            object->x_bound = 0;
            object->y_bound = 0;
            object->z_bound = 1;
            object->start = (Vector3){0, 0, 20};
            object->end = (Vector3){0, 0, 0};
            break;
        case 4:
            object->x_bound = 0;
            object->y_bound = 0;
            object->z_bound = 2;
            object->start = (Vector3){0, 0, 0};
            object->end = (Vector3){0, 0, -20};
            break;
    }
}

float distanceMapCollisionAxis(float pos, float size, float map_s, float map_e, int limit) {
    if (limit == 0) return 0;
    if (limit == 1) return map_s - (pos + size/2.0f);
    if (limit == 2) return (pos - size/2.0f) - map_e;
    printf("Error: limit 3 fell\n");
    return 0;
}
