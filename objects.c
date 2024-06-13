#include "objects.h"

void updateProps(Simulation* simulation) {
    for(int i = 0; i < simulation->num_props; i++) {
        updatePhysics(&simulation->props[i], simulation);
    }
}

void DrawProps(Simulation* simulation) {
    for (int i = 0; i < simulation->num_props; i++) {
        switch (simulation->props[i].hitbox_shape) {
            case 0:
                Prism4 prisma = *(Prism4*)simulation->props[i].hitbox;
                drawPrism4(prisma);
                return;
            case 1:
                return;
        }
    }
}

void setupMapObject(Map_obj* object) {
    object->x_limit = 0;
    object->y_limit = 2;
    object->z_limit = 0;
    object->start = (Vector3){0, 0, 0};
    object->end = (Vector3){0, 0, 0};
}
