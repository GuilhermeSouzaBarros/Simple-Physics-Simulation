#ifndef MATRIX
#define MATRIX

#include <raylib.h>

namespace myMatrix {
    class Matrix {
        public:
            Matrix(int init_rows, int init_columns);
            ~Matrix();

            float getValue(int row, int column);
            void setValue(int row, int column, float value);
            void addValue(int row, int column, float value);
            void printMatrix();

            void multiplyVector(Vector3 *p_vector);

        private:
            int rows;
            int columns;
            float* values;
            int toIndex(int row, int column);
    };
}

#endif