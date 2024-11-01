#include "../h/player.h"

Player* setupPlayer() {
    Player* player = (Player*)malloc(sizeof(Player));
    player->camera.position = (Vector3){0.0f, 1.7f, 0.0f};
    player->camera.target = (Vector3){1.0f, 1.7f, 0.0f};
    player->camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    player->camera.fovy = 71.0f;
    player->camera.projection = CAMERA_PERSPECTIVE;
    player->cam_ang = (Vector2){180, 180};

    Prism4* hitbox = (Prism4*)malloc(sizeof(Prism4));
    hitbox->x_size = 1.0f;
    hitbox->y_size = 1.8f;
    hitbox->z_size = 1.0f;
    hitbox->position = (Vector3){0.0f, 0.9f, 0.0f};
    hitbox->up = (Vector3){0.0f, 1.0f, 0.0f};
    player->body.hitbox = hitbox;
    player->body.hitbox_shape = 0;

    player->body.mass = 80.0f;
    player->body.speed = (Vector3){0.0f, 0.0f, 0.0f};
    player->body.fric[0] = 0.46f;
    player->body.fric[1] = 0.22f;
    player->body.drag = 0.7f;

    return player;
}

void freePlayer(Player* player) {
    free(player->body.hitbox);
    free(player);
}

void updateMovement(Player* player, Simulation* Simulation) {
    if (!isGrounded(&player->body, Simulation)) return;

    int walk = 0, strafe = 0;
    if (IsKeyDown(KEY_W)) walk++;
    if (IsKeyDown(KEY_A)) strafe--;
    if (IsKeyDown(KEY_S)) walk--;
    if (IsKeyDown(KEY_D)) strafe++;

    if (!(walk || strafe)) return;

    Vector2* cam_ang = &player->cam_ang;

    int offset = 0;
    if (walk && strafe) {
        offset = 180 - strafe * (45 * (2 + walk));
    } else if (strafe) {
        offset = 180 - 90 * strafe; 
    } else {
        offset =  90 - 90 * walk;
    }

    float direction = cam_ang->x + offset;
    if (direction > 360) {
        direction -= 360;
    }

    double speed = 1.25/75;
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        speed *= 2;
    }

    player->body.speed.x -= cos(toRad(direction)) * speed;
    player->body.speed.z -= sin(toRad(direction)) * speed;
}

void updateJump(Player* player, Simulation* simulation) {
    if (isGrounded(&player->body, simulation)) {
        if (IsKeyPressed(KEY_SPACE)) {
            player->body.speed.y += 1.0f;
        }
    }
}

void updateCamera(Player* player) {
    Camera3D* camera = &player->camera;
    Vector2* cam_ang = &player->cam_ang;
    Vector3 position = getCoordinates(player->body.hitbox, player->body.hitbox_shape);

    camera->position.x = position.x;
    camera->position.y = position.y + 0.8f;
    camera->position.z = position.z;

    Vector2 delta = GetMouseDelta(), camRad;

    delta.x *= 360.0/SCREEN_X;
    delta.y *= 360.0/SCREEN_X;

    cam_ang->x += delta.x;
    if (cam_ang->x > 360) {
        cam_ang->x -= 360;
    } else if (cam_ang->x < 0) {
        cam_ang->x += 360;
    }

    if (cam_ang->y + delta.y > 269.875) {
        cam_ang->y = 269.875;
    } else if (cam_ang->y + delta.y < 90.125) {
        cam_ang->y = 90.125;
    } else {
        cam_ang->y += delta.y;
    }

    camRad.x = toRad(cam_ang->x);
    camRad.y = toRad(cam_ang->y);

    camera->target.x = camera->position.x + cos(camRad.x) * cos(camRad.y);
    camera->target.y = camera->position.y + sin(camRad.y);
    camera->target.z = camera->position.z + sin(camRad.x) * cos(camRad.y);
    vectorToAngle((Vector2){(camera->target.x - camera->position.x), (camera->target.z - camera->position.z)});
}

void updatePlayer(Simulation* simulation) {
    Player* player = simulation->player;
    updateMovement(player, simulation);
    updateJump(player, simulation);
};
