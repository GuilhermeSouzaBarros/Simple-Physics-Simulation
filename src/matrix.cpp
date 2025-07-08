#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>

using namespace myMatrix;

myMatrix::Matrix::Matrix(int init_rows, int init_columns) {
    rows = init_rows;
    columns = init_columns;
    int size = rows * columns;
    values = (float*)malloc(sizeof(float) * size);
    for (int index = 0; index < size; index++) {
        values[index] = 0.0f;
    }
}

myMatrix::Matrix::~Matrix() {
    printf("Matrix desalocada\n");
    free(values);
}

void myMatrix::Matrix::multiplyVector(Vector3 *p_vector) {
    Vector3 result = {0.0f, 0.0f, 0.0f};
    result.x = getValue(0, 0) * p_vector->x + getValue(0, 1) * p_vector->y + getValue(0, 2) * p_vector->z;
    result.y = getValue(1, 0) * p_vector->x + getValue(1, 1) * p_vector->y + getValue(1, 2) * p_vector->z;
    result.z = getValue(2, 0) * p_vector->x + getValue(2, 1) * p_vector->y + getValue(2, 2) * p_vector->z;
    p_vector->x = result.x;
    p_vector->y = result.y;
    p_vector->z = result.z;
}

int myMatrix::Matrix::toIndex(int row, int column) {
    return row * columns + column;
}

float myMatrix::Matrix::getValue(int row, int column) {
    return values[toIndex(row, column)];
}

void myMatrix::Matrix::setValue(int row, int column, float value) {
    values[toIndex(row, column)] = value;
}

void myMatrix::Matrix::addValue(int row, int column, float value) {
    values[toIndex(row, column)] += value;
}

void myMatrix::Matrix::printMatrix() {
    for (int index_row = 0; index_row < rows; index_row++) {
        printf("Row %d: ", index_row);
        for (int index_column = 0; index_column < columns; index_column++) {
            printf("%5.2f ", getValue(index_row, index_column));
        }
        printf("\n");
    }
    printf("\n");
}
