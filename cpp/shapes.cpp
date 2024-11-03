#include "../h/shapes.h"

void setupPrism4(Physics_obj* obj, double x_size, double y_size, double z_size, Vector3 pos,
                float fric_0, float fric_1, float drag, float mass) {
    Prism4* prisma = (Prism4*)malloc(sizeof(Prism4));
    prisma->x_size = x_size;
    prisma->y_size = y_size;
    prisma->z_size = z_size;
    prisma->position = pos;
    prisma->up = (Vector3){0.0f, 1.0f, 0.0f};
    obj->hitbox = (void*)prisma;
    obj->hitbox_shape = 0;
    obj->speed = (Vector3){0.0f, 0.0f, 0.0f};
    obj->fric[0] = fric_0;
    obj->fric[1] = fric_1;
    obj->drag = drag;
    obj->mass = mass;
}

void drawPrism4(Prism4 prisma) {
    static int i = 0;
    Vector3 oriB = {prisma.position.x - prisma.x_size/2,
                   prisma.position.y - prisma.y_size/2,
                   prisma.position.z - prisma.z_size/2};
    
    Vector3 oriT = {prisma.position.x - prisma.x_size/2,
                   prisma.position.y + prisma.y_size/2,
                   prisma.position.z - prisma.z_size/2};

    Vector3 vertexB2 = {oriB.x + prisma.x_size, oriB.y, oriB.z}; 
    Vector3 vertexB3 = {vertexB2.x, oriB.y, vertexB2.z + prisma.z_size};
    Vector3 vertexB4 = {oriB.x, oriB.y, vertexB2.z + prisma.z_size};

    Vector3 vertexT2 = {oriT.x + prisma.x_size, oriT.y, oriT.z}; 
    Vector3 vertexT3 = {vertexT2.x, oriT.y, vertexT2.z + prisma.z_size};
    Vector3 vertexT4 = {oriT.x, oriT.y, vertexB2.z + prisma.z_size};

    Color color = GOLD;
    if (i) {
        color = RED;
    }

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
    DrawCubeWires(prisma.position, prisma.x_size, prisma.y_size, prisma.z_size, BLACK);
    i++;
    i %= 2;
}

Vector3 getCoordinates(void* object, int shape) {
    switch (shape) {
        case 0: {
            Prism4 prisma = *(Prism4*)object;
            return prisma.position;
        }
        default: {
            return (Vector3){0, 0, 0};
        }
    }
}

void addCoordinates(void* object, int shape, Vector3 coordinates) {
    switch (shape) {
        case 0: {
            Prism4* prisma = (Prism4*)object;
            prisma->position.x += coordinates.x;
            prisma->position.y += coordinates.y;
            prisma->position.z += coordinates.z;
            return;
        }
        default: {
            return;
        }
    }
}

void setCoordinates(void* object, int shape, Vector3 coordinates) {
    switch (shape) {
        case 0: {
            Prism4* prisma = (Prism4*)object;
            prisma->position.x = coordinates.x;
            prisma->position.y = coordinates.y;
            prisma->position.z = coordinates.z;
            return;
        }
        default: {
            return;
        }
    }
}

double getDistFarthestPoint(void* object, int shape) {
    switch (shape) {
        case 0: {
            Prism4 prisma = *(Prism4*)object;
            return (sqrt(pow(prisma.x_size, 2) + pow(prisma.y_size, 2) + pow(prisma.z_size, 2)))/2;
        }
        default: {
            return 0;
        }
    }
}

double getLowestPoint(void* object, int shape) {
    switch (shape) {
        case 0: {
            Prism4 prisma = *(Prism4*)object;
            return (prisma.position.y - prisma.y_size/2);
        }
        default: {
            return 0;
        }
    }
}

double getArea(void* object, int shape, int coord) {
    switch (shape) {
        case 0: {
            Prism4 prisma = *(Prism4*)object;
            switch (coord) {
                case 0: {
                    return (prisma.y_size * prisma.z_size);
                }
                case 1: {
                    return (prisma.x_size * prisma.z_size);
                }
                case 2: {
                    return (prisma.x_size * prisma.y_size);
                }
            }
            return 0;
        }
        default: {
            return 0;
        }
    }
}

double getHeight(void* object, int shape) {
    switch (shape) {
        case 0: {
            Prism4 Prisma = *(Prism4*)object;
            return (Prisma.y_size);
        }
        default: {
            return 0;
        }
    }
}

int hasCollidedPrism4ToPrism4(Prism4* prism1, Prism4* prism2, Vector3 prism1_n, Vector3 prism2_n) {
    Vector3 speed_1 = (Vector3){prism1_n.x - prism1->position.x,
                              prism1_n.y - prism1->position.y,
                              prism1_n.z - prism1->position.z};
    Vector3 speed_2 = (Vector3){prism2_n.x - prism2->position.x,
                              prism2_n.y - prism2->position.y,
                              prism2_n.z - prism2->position.z};

    if (getDistFarthestPoint(prism1, 0) + getDistFarthestPoint(prism2, 0) +
        vector3Module(speed_1) + vector3Module(speed_2) + SMOLL_FLOAT <
        distance2Points(prism1->position, prism2->position)) {
        return 0;
    }

    if ((prism1->x_size + prism2->x_size)/2 + SMOLL_FLOAT < module(prism1_n.x - prism2_n.x) ||
        (prism1->y_size + prism2->y_size)/2 + SMOLL_FLOAT < module(prism1_n.y - prism2_n.y) ||
        (prism1->z_size + prism2->z_size)/2 + SMOLL_FLOAT < module(prism1_n.z - prism2_n.z)) {
        return 0;
    }
    return 1;
}
