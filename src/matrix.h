#ifndef MATRIX
#define MATRIX

#include <raylib.h>

namespace myMatrix {
    class Matrix {
        public:
            int rows;
            int columns;
            float** values;

            Matrix(int init_rows, int init_columns);
            ~Matrix();

            void multiplyVector(Vector3 *p_vector);

    };
}

#endif