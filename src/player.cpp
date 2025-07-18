#include "player.h"

Player::Player() {
    camera_mode = CAMERA_FIRST_PERSON;
    cam_dist = 1.5f;
    camera.position = (Vector3){0.0f, 3.9f, 0.0f};
    camera.target = (Vector3){1.0f, 3.9f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 71.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    cam_ang = (Vector2){180, 180};

    swapped = 0;
}

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
    camera.position.x = hitbox.position.x;
    camera.position.y = hitbox.position.y + 0.8f;
    camera.position.z = hitbox.position.z;

    Vector2 delta = GetMouseDelta(), camRad;

    delta.x *= 100.0/SCREEN_X;
    delta.y *= 100.0/SCREEN_X;

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
    if (cam_dist < 6.0f && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))  cam_dist += 0.5f;
    if (cam_dist > 1.0f && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) cam_dist -= 0.5f;
    
    camera.target.x = hitbox.position.x;
    camera.target.y = hitbox.position.y + 0.8f;
    camera.target.z = hitbox.position.z;

    Vector2 delta = GetMouseDelta(), camRad;

    delta.x *= 100.0/SCREEN_X;
    delta.y *= -100.0/SCREEN_X;

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
    if (swapped || !IsKeyPressed(KEY_Q)) return;
    swapped = 1;
    cam_ang = (Vector2){cam_ang.x - 180, 360 - cam_ang.y};
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
