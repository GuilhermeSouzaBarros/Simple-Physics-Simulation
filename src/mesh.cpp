#include "mesh.h"
using namespace mesh;

MeshData::MeshData() {
    reset();
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

    first_update = 1;
    locked = 0;
}

MeshData::~MeshData() {
    free(p_current_vertices);
    free(p_model);
    printf("MeshData desalocada\n");
}

void MeshData::reset() {
    for (int index_row = 0; index_row < 4; index_row++) {
        for (int index_column = 0; index_column < 4; index_column++) {
            transformation.values[index_row][index_column] = index_row == index_column;
        }
    }
    transformation.values[1][3] = 1.8f;
    transformation.values[2][3] = 5.0f;
    speed = {0.0f, 0.0f, 0.0f};
    angularSpeed = {0.0f, 0.0f, 0.0f};    
}

void MeshData::updateMeshCoordinates() {
    for (int index_vertex = 0; index_vertex < qtd_vertices; index_vertex++) {
        p_current_vertices[index_vertex].x = original_vertices[index_vertex].x;
        p_current_vertices[index_vertex].y = original_vertices[index_vertex].y;
        p_current_vertices[index_vertex].z = original_vertices[index_vertex].z;

        transformation.multiplyVector(&p_current_vertices[index_vertex]);
    }
}

void MeshData::stepLinearMomentum(float deltaTime) {
    transformation.values[0][3] += speed.x * deltaTime;
    transformation.values[1][3] += speed.y * deltaTime;
    transformation.values[2][3] += speed.z * deltaTime;
}

void MeshData::stepAngularMomentum(float deltaTime) {
    Vector3 cross_x = crossProduct(angularSpeed, (Vector3){transformation.values[0][0],
                                                           transformation.values[1][0],
                                                           transformation.values[2][0]});
    Vector3 cross_y = crossProduct(angularSpeed, (Vector3){transformation.values[0][1],
                                                           transformation.values[1][1],
                                                           transformation.values[2][1]});
    Vector3 cross_z = crossProduct(angularSpeed, (Vector3){transformation.values[0][2],
                                                           transformation.values[1][2],
                                                           transformation.values[2][2]});
    transformation.values[0][0] += cross_x.x * deltaTime;
    transformation.values[0][1] += cross_y.x * deltaTime;
    transformation.values[0][2] += cross_z.x * deltaTime;
    transformation.values[1][0] += cross_x.y * deltaTime;
    transformation.values[1][1] += cross_y.y * deltaTime;
    transformation.values[1][2] += cross_z.y * deltaTime;
    transformation.values[2][0] += cross_x.z * deltaTime;
    transformation.values[2][1] += cross_y.z * deltaTime;
    transformation.values[2][2] += cross_z.z * deltaTime;
}

int pi2 = 2 * PI;

void MeshData::stepMeshData(float deltaTime) {
    stepLinearMomentum(deltaTime);
    stepAngularMomentum(deltaTime);
    updateMeshCoordinates();
    if (first_update) {
        if (IsKeyDown(KEY_LEFT_CONTROL)) {
            if (IsKeyDown(KEY_J)) angularSpeed.x -= 0.05; 
            if (IsKeyDown(KEY_U)) angularSpeed.x += 0.05; 
            if (IsKeyDown(KEY_K)) angularSpeed.y -= 0.05; 
            if (IsKeyDown(KEY_I)) angularSpeed.y += 0.05; 
            if (IsKeyDown(KEY_L)) angularSpeed.z -= 0.05; 
            if (IsKeyDown(KEY_O)) angularSpeed.z += 0.05;
        } else {
            if (IsKeyDown(KEY_J)) speed.x -= 0.05; 
            if (IsKeyDown(KEY_U)) speed.x += 0.05; 
            if (IsKeyDown(KEY_K)) speed.y -= 0.05; 
            if (IsKeyDown(KEY_I)) speed.y += 0.05; 
            if (IsKeyDown(KEY_L)) speed.z -= 0.05; 
            if (IsKeyDown(KEY_O)) speed.z += 0.05;
        }
        if (IsKeyPressed(KEY_F)) {
            locked = !locked;
        }
    }
    first_update = 0;
}

void MeshData::normalizeColumn(int c) {
    float module = 0;
    for (int i = 0; i < 3; i++) {
        module += pow(transformation.values[i][c], 2);
    }
    module = sqrt(module);
    if (module == 0.0) return;
    for (int i = 0; i < 3; i++) {
        transformation.values[i][c] /= module;
    }
}

void MeshData::ortogonalize() {
    Vector3 projection1 = {transformation.values[0][0], transformation.values[1][0], transformation.values[2][0]};
    Vector3 projection2 = {transformation.values[0][1], transformation.values[1][1], transformation.values[2][1]};
    Vector3 projection3 = {transformation.values[0][2], transformation.values[1][2], transformation.values[2][2]};
    projection2 = removeProjection(projection1, projection2);
    projection3 = removeProjection(projection1, projection3);
    projection3 = removeProjection(projection2, projection3);
    
    transformation.values[0][1] = projection2.x;
    transformation.values[1][1] = projection2.y;
    transformation.values[2][1] = projection2.z;

    transformation.values[0][2] = projection3.x;
    transformation.values[1][2] = projection3.y;
    transformation.values[2][2] = projection3.z;
}

void MeshData::degreesOfFreedom() {
    ortogonalize();
    for (int i = 0; i < 3; i++){
        normalizeColumn(i);
    }
}

void MeshData::drawMeshData() {
    first_update = 1;
    if (IsKeyPressed(KEY_R)) reset();
    if (locked) degreesOfFreedom();

    Vector3 origin = {transformation.values[0][3], transformation.values[1][3], transformation.values[2][3]};

    DrawLine3D(origin, (Vector3){origin.x + angularSpeed.x,
                                 origin.y + angularSpeed.y,
                                 origin.z + angularSpeed.z}, ORANGE);
    
    DrawLine3D(origin, (Vector3){origin.x + speed.x,
                                 origin.y + speed.y,
                                 origin.z + speed.z}, DARKPURPLE);

    for (int index_f = 0; index_f < qtd_vertices; index_f++) {
        DrawSphereEx(p_current_vertices[index_f], 0.015f, 10, 10, BLACK);
    }

    for (int index = 0; index < qtd_faces; index++) {
        DrawLine3D(*p_model[index].p_vertices[0], *p_model[index].p_vertices[1], BLACK);
        DrawLine3D(*p_model[index].p_vertices[0], *p_model[index].p_vertices[2], BLACK);
        DrawLine3D(*p_model[index].p_vertices[1], *p_model[index].p_vertices[0], BLACK);
        DrawLine3D(*p_model[index].p_vertices[1], *p_model[index].p_vertices[2], BLACK);
        DrawLine3D(*p_model[index].p_vertices[2], *p_model[index].p_vertices[0], BLACK);
        DrawLine3D(*p_model[index].p_vertices[2], *p_model[index].p_vertices[1], BLACK);
    }
    for (int index = 0; index < qtd_faces; index++) {
        DrawTriangle3D(*(p_model[index].p_vertices[0]),
                       *(p_model[index].p_vertices[1]),
                       *(p_model[index].p_vertices[2]), ColorAlpha(GREEN, 0.5f));
    }
}