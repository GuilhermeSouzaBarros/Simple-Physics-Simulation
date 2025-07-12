#include "structs.h"

int SCREEN_X;
int SCREEN_Y;

void setScreen() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(0, 0, "Easter Egg?!?!");
    SCREEN_X = GetScreenWidth();
    SCREEN_Y = GetScreenHeight();
    SetWindowSize(SCREEN_X * 0.8, SCREEN_Y * 0.8);
    SetWindowPosition(SCREEN_X * 0.1, SCREEN_Y * 0.1);
    DisableCursor();
    SetExitKey(KEY_DELETE);
}

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


double vector3Module(Vector3 vector) {
    return (sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2)));
}

Vector3 vector3Add(Vector3 v1, Vector3 v2) {
    return (Vector3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

Vector3 vector3Sub(Vector3 v1, Vector3 v2) {
    return (Vector3){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}
Vector3 vector3Product(Vector3 v1, Vector3 v2) {
    return (Vector3){v1.x * v2.x, v1.y * v2.y, v1.z * v2.z};
}

void setVector3ModuleToValue(Vector3 *p_vector, float value) {
    float module = vector3Module(*p_vector);
    if (floatIsZero(module)) return;
    p_vector->x *= value/module;
    p_vector->y *= value/module;
    p_vector->z *= value/module;
}

Vector3 crossProduct(Vector3 vector_1, Vector3 vector_2) {
    Vector3 product = {0.0f, 0.0f, 0.0f};
    product.x = vector_1.y * vector_2.z - vector_2.y * vector_1.z;
    product.y = -(vector_1.x * vector_2.z - vector_2.x * vector_1.z);
    product.z = vector_1.x * vector_2.y - vector_2.x * vector_1.y;
    return product;
}

void printVector3(Vector3 vector) {
    printf("Vector: %f / %f / %f\n", vector.x, vector.y, vector.z);
}
