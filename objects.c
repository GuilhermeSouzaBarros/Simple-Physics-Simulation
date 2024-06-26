#include "objects.h"

void updateProps(Simulation* simulation) {
    for(int i = 0; i < simulation->num_props; i++) {
        updatePhysics(&simulation->props[i], simulation);
    }
    updatePhysics(&simulation->player->body, simulation);
    
    CollInf col = {0, 0.0f, 0.0f};
    for(int i = 0; i < simulation->num_props; i++) {
        updateObjCollisionsMap(&simulation->props[i], simulation, &col);
    }
    updateObjCollisionsMap(&simulation->player->body, simulation, &col);

    for(int i = 0; i < simulation->num_props; i++) {
        updateCollisionsProps(&simulation->props[i], simulation, &col);
    }
    updateCollisionsProps(&simulation->player->body, simulation, &col);
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
