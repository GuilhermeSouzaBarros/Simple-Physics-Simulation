#include "structs.h"

int FULL_X, FULL_Y;
int SCREEN_X, SCREEN_Y;
int POS_X, POS_Y;

void setScreen() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(0, 0, "Easter Egg?!?!");
    FULL_X= GetScreenWidth(); FULL_Y = GetScreenHeight();
    SCREEN_X = FULL_X * 0.8;
    SCREEN_Y = FULL_Y * 0.8;
    POS_X = FULL_X * 0.1;
    POS_Y = FULL_Y * 0.1;
    SetWindowSize(SCREEN_X, SCREEN_Y);
    SetWindowPosition(POS_X, POS_Y);
    DisableCursor();
    
    SetExitKey(KEY_DELETE);
    SetTargetFPS(100);
}

void updateScreen() {
    if (!IsCursorOnScreen()) {
        SetMousePosition(0, 0);
    }
    if (!IsKeyPressed(KEY_F11)) return;
    ToggleFullscreen();
    if (!IsWindowFullscreen()) {
        SetWindowSize(SCREEN_X, SCREEN_Y);
        SetWindowPosition(POS_X, POS_Y);
    }
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

double dotProduct(Vector3 vector_1, Vector3 vector_2) {
    return
        vector_1.x * vector_2.x +
        vector_1.y * vector_2.y +
        vector_1.z * vector_2.z;
}

double project(Vector3 vector_1, Vector3 vector_2) {
    return dotProduct(vector_1, vector_2) / vector3Module(vector_1);
}

Vector3 removeProjection(Vector3 vector_1, Vector3 vector_2) {
    double projection = project(vector_1, vector_2);
    vector_2.x -= vector_1.x * projection;
    vector_2.y -= vector_1.y * projection;
    vector_2.z -= vector_1.z * projection;
    return vector_2;
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
