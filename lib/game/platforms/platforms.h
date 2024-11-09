#pragma once

#include <stdbool.h>

typedef struct {
    int x;
    int y;
    int* parts;
} Platform;

void render_platform(Platform* platform, SDL_Renderer* renderer);
void platform_hit(Platform* platform, Bullet* bullet);
bool is_bullet_on_platform(Platform* platform, Bullet* bullet);