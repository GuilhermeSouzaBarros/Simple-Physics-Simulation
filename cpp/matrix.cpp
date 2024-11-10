#include "../h/matrix.h"

#include <stdlib.h>

using namespace matrix;

Matrix::Matrix(int init_rows, int init_columns) {
    rows = init_rows;
    columns = init_columns;
    int size = rows * columns;
    values = (float*)malloc(sizeof(float) * size);
    for (int index = 0; index < size; index++) {
        values[index] = 0.0f;
    }
}

Matrix::~Matrix() {
    printf("Matrix desalocada\n");
    free(values);
}

int Matrix::toIndex(int row, int column) {
    return row * columns + column;
}

float Matrix::getValue(int row, int column) {
    return values[toIndex(row, column)];
}

void Matrix::setValue(int row, int column, float value) {
    values[toIndex(row, column)] = value;
}

void Matrix::addValue(int row, int column, float value) {
    values[toIndex(row, column)] += value;
}

void Matrix::printMatrix() {
    for (int index_row = 0; index_row < rows; index_row++) {
        printf("Row %d: ", index_row);
        for (int index_column = 0; index_column < columns; index_column++) {
            printf("%5.2f ", getValue(index_row, index_column));
        }
        printf("\n");
    }
    printf("\n");
}