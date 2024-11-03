#include "../h/player.h"

Player::Player() {
    camera_mode = CAMERA_FIRST_PERSON;
    cam_dist = 1.5f;
    camera.position = (Vector3){0.0f, 3.9f, 0.0f};
    camera.target = (Vector3){1.0f, 3.9f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 71.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    cam_ang = (Vector2){180, 180};

    Prism4* hitbox = (Prism4*)malloc(sizeof(Prism4));
    hitbox->x_size = 1.0f;
    hitbox->y_size = 1.8f;
    hitbox->z_size = 1.0f;
    hitbox->position = (Vector3){0.0f, 4.0f, 0.0f};
    hitbox->up = (Vector3){0.0f, 1.0f, 0.0f};
    body.hitbox = hitbox;
    body.hitbox_shape = 0;

    body.mass = 80.0f;
    body.speed = (Vector3){0.0f, 0.0f, 0.0f};
    body.fric[0] = 0.46f;
    body.fric[1] = 0.22f;
    body.drag = 0.7f;
}

/*
void freePlayer(Player* player) {
    free(player->body.hitbox);
    free(player);
}
*/

void Player::cameraBound() {
    if (cam_ang.x > 360) {
        cam_ang.x -= 360;
    } else if (cam_ang.x < 0) {
        cam_ang.x += 360;
    }
    if (cam_ang.y > 269.875) {
        cam_ang.y = 269.875;
    } else if (cam_ang.y < 90.125) {
        cam_ang.y = 90.125;
    }
}

void Player::updateCameraFP() {
    Vector3 position = getCoordinates(body.hitbox, body.hitbox_shape);

    camera.position.x = position.x;
    camera.position.y = position.y + 0.8f;
    camera.position.z = position.z;

    Vector2 delta = GetMouseDelta(), camRad;

    delta.x *= 360.0/SCREEN_X;
    delta.y *= 360.0/SCREEN_X;

    cam_ang.x += delta.x;
    cam_ang.y += delta.y;
    cameraBound();

    camRad.x = toRad(cam_ang.x);
    camRad.y = toRad(cam_ang.y);

    camera.target.x = camera.position.x + cos(camRad.x) * cos(camRad.y);
    camera.target.y = camera.position.y + sin(camRad.y);
    camera.target.z = camera.position.z + sin(camRad.x) * cos(camRad.y);
}

void Player::updateCameraTP() {
    if (cam_dist < 3.0f && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))  cam_dist += 0.5f;
    if (cam_dist > 1.0f && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) cam_dist -= 0.5f;
    
    Vector3 position = getCoordinates(body.hitbox, body.hitbox_shape);

    camera.target.x = position.x;
    camera.target.y = position.y + 0.8f;
    camera.target.z = position.z;

    Vector2 delta = GetMouseDelta(), camRad;

    delta.x *= 360.0/SCREEN_X;
    delta.y *= -360.0/SCREEN_X;

    cam_ang.x += delta.x;
    cam_ang.y += delta.y;
    cameraBound();

    camRad.x = toRad(cam_ang.x);
    camRad.y = toRad(cam_ang.y);

    camera.position.x = camera.target.x + cam_dist * (cos(camRad.x) * cos(camRad.y));
    camera.position.y = camera.target.y + cam_dist * (sin(camRad.y));
    camera.position.z = camera.target.z + cam_dist * (sin(camRad.x) * cos(camRad.y));
}

void Player::switchCamera() {
    if (!IsKeyPressed(KEY_Q)) return;
    printf("B %f / %f\n", cam_ang.x, cam_ang.y);
    cam_ang = (Vector2){cam_ang.x - 180, 360 - cam_ang.y};
    printf("A %f / %f\n", cam_ang.x, cam_ang.y);
    cameraBound();

    if (camera_mode == CAMERA_FIRST_PERSON) {
        cam_dist = 1.5f;
        camera_mode = CAMERA_THIRD_PERSON;
    } else {
        cam_dist = 1.5f;
        camera_mode = CAMERA_FIRST_PERSON;
    }

}

void Player::updateCamera() {
    switchCamera();
    if (camera_mode == CAMERA_FIRST_PERSON) {
        updateCameraFP();   
    } else {
        updateCameraTP();
    }
}

void Player::updateCoord() {
    switch (body.hitbox_shape) {
        case 0: {
            Prism4* hitbox = (Prism4*)body.hitbox;
            hitbox->position.x += body.speed.x;
            hitbox->position.y += body.speed.y;
            hitbox->position.z += body.speed.z;
        }
        default:
            return;
    }
};

Camera3D* Player::getCamera() {
    return &camera;
};

Vector2* Player::getCamAng() {
    return &cam_ang;
}

Physics_obj* Player::getBody() {
    return &body;
};
