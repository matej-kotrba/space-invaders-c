#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "../../utils.h"

typedef enum { PLAYER, ENEMY } BulletType;

typedef struct {
    float x;
    float y;
    int w;
    int h;
    float xs;
    float ys;
} Bullet;

Bullet create_new_bullet(float x, float y, int w, int h, float xs, float ys);
Vector2 get_default_bullet_size(BulletType bullet_type);
void render_bullet(Bullet* bullet, SDL_Renderer* renderer);
void update_bullet(Bullet* bullet, float delta_time);
bool should_remove_bullet(Bullet* bullet, int window_width, int window_height);