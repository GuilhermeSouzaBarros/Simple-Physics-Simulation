#include "structs.h"

void setupPrism4(Physics_obj* obj, double x_size, double y_size, double z_size, Vector3 pos) {
    Prism4* prisma = (Prism4*)malloc(sizeof(Prism4));
    prisma->x_size = x_size;
    prisma->y_size = y_size;
    prisma->z_size = z_size;
    prisma->position = pos;
    prisma->up = (Vector3){0.0f, 1.0f, 0.0f};
    obj->hitbox = (void*)prisma;
    obj->speed = (Vector3){0.0f, 0.0f, 0.0f};
    obj->fric[0] = 0.4f;
    obj->fric[1] = 0.6f;
    obj->drag = 0.5;
    obj->mass = 40;
}

void drawPrism4(Prism4 prisma) {
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

    DrawTriangle3D(oriB, oriT, vertexB2, RED);
    DrawTriangle3D(vertexB2, oriT, vertexT2, RED);

    DrawTriangle3D(vertexB2, vertexT2, vertexB3, GOLD);
    DrawTriangle3D(vertexB3, vertexT2, vertexT3, GOLD);

    DrawTriangle3D(vertexB3, vertexT3, vertexB4, BLUE);
    DrawTriangle3D(vertexB4, vertexT3, vertexT4, BLUE);

    DrawTriangle3D(vertexB4, vertexT4, oriB, GREEN);
    DrawTriangle3D(oriB, vertexT4, oriT, GREEN);

    DrawTriangle3D(oriT, vertexT4, vertexT3, BLACK);
    DrawTriangle3D(vertexT3, vertexT2, oriT, BLACK);

    DrawTriangle3D(oriB, vertexB2, vertexB3, PURPLE);
    DrawTriangle3D(vertexB3, vertexB4, oriB, PURPLE);
}
