#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>

using namespace myMatrix;

myMatrix::Matrix::Matrix(int init_rows, int init_columns) {
    rows = init_rows;
    columns = init_columns;
    
    values = (float**)malloc(sizeof(float*) * init_rows);
    for (int index = 0; index < init_rows; index++) {
        values[index] = (float*)malloc(sizeof(float) * init_columns);
        for (int jndex = 0; jndex < init_columns; jndex++) {
            values[index][jndex] = 0.0f;
        }
    }
}

myMatrix::Matrix::~Matrix() {
    for (int i = 0; i < rows; i++) {
        free(values[i]);
    }
    free(values);
    printf("Matrix desalocada\n");
}

void myMatrix::Matrix::multiplyVector(Vector3 *p_vector) {
    Vector3 result = {0.0f, 0.0f, 0.0f};
    result.x = values[0][0] * p_vector->x + values[0][1] * p_vector->y + values[0][2] * p_vector->z + values[0][3];
    result.y = values[1][0] * p_vector->x + values[1][1] * p_vector->y + values[1][2] * p_vector->z + values[1][3];
    result.z = values[2][0] * p_vector->x + values[2][1] * p_vector->y + values[2][2] * p_vector->z + values[2][3];
    p_vector->x = result.x;
    p_vector->y = result.y;
    p_vector->z = result.z;
}
