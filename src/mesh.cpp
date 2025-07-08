#include "mesh.h"
using namespace mesh;

MeshData::MeshData() {
    center_of_mass = {0.0f, 1.8f, 5.0f};
    speed = {0.0f, 0.0f, 0.0f};
    for (int index_row = 0; index_row < 3; index_row++) {
        for (int index_column = 0; index_column < 3; index_column++) {
            if (index_row == index_column) rotation.setValue(index_row, index_column, 1.0f);
            else rotation.setValue(index_row, index_column, 0.0f);
        }
    }
    angularSpeed = {0.0f, 2.0f, 0.0f};

    qtd_vertices = 8;
    original_vertices = (vertex*)malloc(sizeof(vertex) * qtd_vertices);
    original_vertices[0] = {-1.5f, -0.75f, -0.5f};
    original_vertices[1] = {-1.5f, -0.75f,  0.5f};
    original_vertices[2] = {-1.5f,  0.75f,  0.5f};
    original_vertices[3] = {-1.5f,  0.75f, -0.5f};
    original_vertices[4] = { 1.5f,  0.75f, -0.5f};
    original_vertices[5] = { 1.5f,  0.75f,  0.5f};
    original_vertices[6] = { 1.5f, -0.75f,  0.5f};
    original_vertices[7] = { 1.5f, -0.75f, -0.5f};

    p_current_vertices = (vertex*)malloc(sizeof(vertex) * qtd_vertices);

    qtd_faces = 12;
    p_model = (face*)malloc(sizeof(face) * qtd_faces);
    p_model[0] = {&p_current_vertices[0], &p_current_vertices[1], &p_current_vertices[2]};
    p_model[1] = {&p_current_vertices[0], &p_current_vertices[2], &p_current_vertices[3]};

    p_model[2] = {&p_current_vertices[0], &p_current_vertices[3], &p_current_vertices[4]};
    p_model[3] = {&p_current_vertices[0], &p_current_vertices[4], &p_current_vertices[7]};

    p_model[4] = {&p_current_vertices[0], &p_current_vertices[7], &p_current_vertices[6]};
    p_model[5] = {&p_current_vertices[0], &p_current_vertices[6], &p_current_vertices[1]};

    p_model[6] = {&p_current_vertices[5], &p_current_vertices[3], &p_current_vertices[2]};
    p_model[7] = {&p_current_vertices[5], &p_current_vertices[4], &p_current_vertices[3]};

    p_model[8] = {&p_current_vertices[5], &p_current_vertices[2], &p_current_vertices[1]};
    p_model[9] = {&p_current_vertices[5], &p_current_vertices[1], &p_current_vertices[6]};

    p_model[10] = {&p_current_vertices[5], &p_current_vertices[6], &p_current_vertices[7]};
    p_model[11] = {&p_current_vertices[5], &p_current_vertices[7], &p_current_vertices[4]};
    updateMeshCoordinates();
}

MeshData::~MeshData() {
    free(p_current_vertices);
    free(p_model);
    printf("MeshData desalocada\n");
}

void MeshData::updateMeshCoordinates() {
    for (int index_vertex = 0; index_vertex < qtd_vertices; index_vertex++) {
        p_current_vertices[index_vertex].x = original_vertices[index_vertex].x;
        p_current_vertices[index_vertex].y = original_vertices[index_vertex].y;
        p_current_vertices[index_vertex].z = original_vertices[index_vertex].z;

        rotation.multiplyVector(&p_current_vertices[index_vertex]);

        p_current_vertices[index_vertex].x += center_of_mass.x;
        p_current_vertices[index_vertex].y += center_of_mass.y;
        p_current_vertices[index_vertex].z += center_of_mass.z;
    }
}

void MeshData::stepLinearMomentum(float deltaTime) {
    center_of_mass.x += speed.x * deltaTime;
    center_of_mass.y += speed.y * deltaTime;
    center_of_mass.z += speed.z * deltaTime;
}

void MeshData::stepAngularMomentum(float deltaTime) {
    Vector3 cross_x = crossProduct(angularSpeed, (Vector3){rotation.getValue(0, 0),
                                                           rotation.getValue(1, 0),
                                                           rotation.getValue(2, 0)});
    Vector3 cross_y = crossProduct(angularSpeed, (Vector3){rotation.getValue(0, 1),
                                                           rotation.getValue(1, 1),
                                                           rotation.getValue(2, 1)});
    Vector3 cross_z = crossProduct(angularSpeed, (Vector3){rotation.getValue(0, 2),
                                                           rotation.getValue(1, 2),
                                                           rotation.getValue(2, 2)});
    rotation.addValue(0, 0, cross_x.x * deltaTime);
    rotation.addValue(0, 1, cross_y.x * deltaTime);
    rotation.addValue(0, 2, cross_z.x * deltaTime);
    rotation.addValue(1, 0, cross_x.y * deltaTime);
    rotation.addValue(1, 1, cross_y.y * deltaTime);
    rotation.addValue(1, 2, cross_z.y * deltaTime);
    rotation.addValue(2, 0, cross_x.z * deltaTime);
    rotation.addValue(2, 1, cross_y.z * deltaTime);
    rotation.addValue(2, 2, cross_z.z * deltaTime);
}

void MeshData::stepMeshData(float deltaTime) {
    stepLinearMomentum(deltaTime);
    stepAngularMomentum(deltaTime);
    updateMeshCoordinates();
}

void MeshData::drawMeshData() {
    for (int index_f = 0; index_f < qtd_vertices; index_f++) {
        for (int index_j = 0; index_j < qtd_vertices; index_j++) {
            if (index_f == index_j) continue;
            DrawLine3D(p_current_vertices[index_f], p_current_vertices[index_j], BLACK);
        }
        DrawSphereEx(p_current_vertices[index_f], 0.015f, 10, 10, BLACK);
    }
    for (int index = 0; index < qtd_faces; index++) {
        DrawTriangle3D(*(p_model[index].p_vertices[0]),
                       *(p_model[index].p_vertices[1]),
                       *(p_model[index].p_vertices[2]), ColorAlpha(PINK, 0.5f));
    }
}