#ifndef MESH
#define MESH

#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>

#include "matrix.h"
#include "structs.h"

typedef Vector3 vertex;

typedef struct face {
    vertex *p_vertices[3];
} face;

namespace mesh {
    class MeshData {
        public:
            MeshData();
            ~MeshData();
            vertex *original_vertices;
            vertex *p_current_vertices;
            int qtd_vertices;
            face *p_model;
            int qtd_faces;

            Vector3 center_of_mass;
            Vector3 speed;
            myMatrix::Matrix rotation = {3, 3};
            Vector3 angularSpeed;

            void stepMeshData(float DeltaTime);
            void drawMeshData();
        
        private:

            void stepLinearMomentum(float deltaTime);
            void stepAngularMomentum(float deltatime);
            void updateMeshCoordinates();
    };
}

#endif