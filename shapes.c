#include "shapes.h"

int sign(double number) {
    if (number > 0) {
        return 1;
    }
    if (number < 0) {
        return -1;
    } 
    return 0;
}

double module(double number) {
    if (number < 0) {
        return -number;
    }
    return number;
}

double distance2Points(Vector3 p1, Vector3 p2) {
    return (sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) + pow(p2.z - p1.z, 2)));
}

double vector2Module(Vector2 vector){
    return (sqrt(pow(vector.x, 2) + pow(vector.y, 2)));
}

double vector3Module(Vector3 vector) {
    return (sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2)));
}

double vectorToAngle(Vector2 vector) {
    float angle = toAngles(atan2(vector.y, vector.x));
    if (angle < 0) {
        angle += 360;
    }
    return angle;
}

double toRad(double degree) {
    return (degree/180.0 * PI);
}

double toAngles(double radians) {
    return (radians/PI * 180);
}

double circleArea(double radius) {
    return (PI * pow(radius, 2));
}

Vector3 getCoordinates(void* object, int shape) {
    switch (shape) {
        case 0:
            Prism4 prisma = *(Prism4*)object;
            return prisma.position;
        case 1:
            Cylinder cylinder = *(Cylinder*)object;
            return cylinder.position;
        default:
            return (Vector3){0, 0, 0};
    }
}

void addCoordinates(void* object, int shape, Vector3 coordinates) {
    switch (shape) {
        case 0:
            Prism4* prisma = (Prism4*)object;
            prisma->position.x += coordinates.x;
            prisma->position.y += coordinates.y;
            prisma->position.z += coordinates.z;
            return;
        case 1:
            Cylinder* cylinder = (Cylinder*)object;
            cylinder->position.x += coordinates.x;
            cylinder->position.y += coordinates.y;
            cylinder->position.z += coordinates.z;
            return;
    }
}

void setCoordinates(void* object, int shape, Vector3 coordinates) {
    switch (shape) {
        case 0:
            Prism4* prisma = (Prism4*)object;
            prisma->position.x = coordinates.x;
            prisma->position.y = coordinates.y;
            prisma->position.z = coordinates.z;
            return;
        case 1:
            Cylinder* hitbox = (Cylinder*)object;
            hitbox->position.x = coordinates.x;
            hitbox->position.y = coordinates.y;
            hitbox->position.z = coordinates.z;
            return;
    }
}

double getDistFarthestPoint(void* object, int shape) {
    switch (shape) {
        case 0:
            Prism4 prisma = *(Prism4*)object;
            return (sqrt(pow(prisma.x_size, 2) + pow(prisma.y_size, 2) + pow(prisma.z_size, 2)))/2;
        case 1:
            Cylinder cylinder = *(Cylinder*)object;
            return (sqrt(pow(cylinder.radius, 2) + pow(cylinder.height/2, 2)));
        default:
            return 0;
    }
}

double getLowestPoint(void* object, int shape) {
    switch (shape) {
        case 0:
            Prism4 prisma = *(Prism4*)object;
            return (prisma.position.y - prisma.y_size/2);
        case 1:
            Cylinder hitbox = *(Cylinder*)object;
            return (hitbox.position.y - hitbox.height/2);
        default:
            return 0;
    }
}

double getArea(void* object, int shape, int coord) {
    switch (shape) {
        case 0:
            Prism4 prisma = *(Prism4*)object;
            switch (coord) {
                case 0:
                    return (prisma.y_size * prisma.z_size);
                case 1:
                    return (prisma.x_size * prisma.z_size);
                case 2:
                    return (prisma.x_size * prisma.y_size);
            }
            return 0;
        case 1:
            Cylinder cylinder = *(Cylinder*)object;
            switch (coord) {
                case 1:
                    return (circleArea(cylinder.radius));
                default:
                    return (cylinder.height * cylinder.radius * 2);
            }
            return 0;
        default:
            return 0;
    }
}

double getHeight(void* object, int shape) {
    switch (shape) {
        case 0:
            Prism4 Prisma = *(Prism4*)object;
            return (Prisma.y_size);
        case 1:
            Cylinder cylinder = *(Cylinder*)object;
            return (cylinder.height);
        default:
            return 0;
    }
}
