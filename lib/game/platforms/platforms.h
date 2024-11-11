#pragma once

#include <stdbool.h>

typedef struct {
    int x;
    int y;
    int* parts;
} Platform;

void render_platform(Platform* platform, SDL_Renderer* renderer);
void platform_hit(Platform* platform, Bullet* bullet, size_t index);
bool is_bullet_on_platform(Platform* platform, Bullet* bullet);
int get_platform_bullet_hit_part_index(Platform* platform, Bullet* bullet);