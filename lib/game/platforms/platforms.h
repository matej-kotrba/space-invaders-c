#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "../projectile/projectile.h"

#define PLATFORM_TEMPLATE_ROWS 18
#define PLATFORM_TEMPLATE_COLUMNS 20
#define PLATFORM_PART_SIZE 4
#define PLATFORM_PROJECTILE_DIRECT_HIT_THRESHOLD 2
#define PLATFORM_PROJECTILE_INDIRECT_HIT_THRESHOLD 4

#define PLATFORMS_COUNT 3

typedef struct {
    int x;
    int y;
    int* parts;
} Platform;

Platform create_new_platform(int x, int y);
void render_platform(Platform* platform, SDL_Renderer* renderer);
void platform_hit(Platform* platform, Bullet* bullet, size_t index);
bool is_bullet_on_platform(Platform* platform, Bullet* bullet);
int get_platform_bullet_hit_part_index(Platform* platform, Bullet* bullet);