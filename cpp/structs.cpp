#include "../h/structs.h"

int floatIsZero(float num) {
    return (-SMOLL_FLOAT < num && num < SMOLL_FLOAT);
}

void smallFloatToZero(float* num) {
    if (-SMOLL_FLOAT < *num && *num < SMOLL_FLOAT) {
        *num = 0.0f;
    }
}

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

Vector3 vector3Add(Vector3 v1, Vector3 v2) {
    return (Vector3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

Vector3 vector3Sub(Vector3 v1, Vector3 v2) {
    return (Vector3){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
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
