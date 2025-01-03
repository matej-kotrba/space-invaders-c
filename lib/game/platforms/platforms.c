#include "platforms.h"

#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../../hitboxes.h"
#include "../../utils.h"
#include "../projectile/projectile.h"

static int platform_template[PLATFORM_TEMPLATE_ROWS *
                             (PLATFORM_TEMPLATE_COLUMNS / 2)] = {
    0, 0, 0, 0, 0, 1, 1, 1, 1, 1,  //
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1,  //
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1,  //
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1,  //
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1,  //
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  //
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  //
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  //
    1, 1, 1, 1, 1, 1, 1, 1, 0, 0,  //
    1, 1, 1, 1, 1, 1, 1, 0, 0, 0,  //
    1, 1, 1, 1, 1, 1, 0, 0, 0, 0,  //
    1, 1, 1, 1, 1, 0, 0, 0, 0, 0,  //
    1, 1, 1, 1, 0, 0, 0, 0, 0, 0,  //
    1, 1, 1, 1, 0, 0, 0, 0, 0, 0,  //
    1, 1, 1, 1, 0, 0, 0, 0, 0, 0,  //
    1, 1, 1, 1, 0, 0, 0, 0, 0, 0,  //
    1, 1, 1, 1, 0, 0, 0, 0, 0, 0,  //
    1, 1, 1, 1, 0, 0, 0, 0, 0, 0,  //
};

Platform create_new_platform(int x, int y) {
    Platform p = {
        .x = x,
        .y = y,
        .parts = (int*)malloc(PLATFORM_TEMPLATE_ROWS *
                              PLATFORM_TEMPLATE_COLUMNS * sizeof(int))};

    for (int i = 0; i < PLATFORM_TEMPLATE_ROWS; i++) {
        for (int j = 0; j < PLATFORM_TEMPLATE_COLUMNS; j++) {
            int index = j < PLATFORM_TEMPLATE_COLUMNS / 2
                            ? j
                            : PLATFORM_TEMPLATE_COLUMNS - 1 - j;
            p.parts[i * PLATFORM_TEMPLATE_COLUMNS + j] =
                platform_template[i * (PLATFORM_TEMPLATE_COLUMNS / 2) + index];
        }
    }

    return p;
}

void render_platform(Platform* platform, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect rect = {.x = platform->x,
                     .y = platform->y,
                     .w = PLATFORM_PART_SIZE,
                     .h = PLATFORM_PART_SIZE};

    for (int i = 0; i < PLATFORM_TEMPLATE_ROWS; i++) {
        rect.y = platform->y + i * PLATFORM_PART_SIZE;
        for (int j = 0; j < PLATFORM_TEMPLATE_COLUMNS; j++) {
            if (platform->parts[i * PLATFORM_TEMPLATE_COLUMNS + j]) {
                rect.x = platform->x + j * PLATFORM_PART_SIZE;
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

void platform_hit(Platform* platform, size_t index) {
    int hit_part_xm =
        platform->x + (index % PLATFORM_TEMPLATE_COLUMNS) * PLATFORM_PART_SIZE;
    int hit_part_ym =
        platform->y + (index / PLATFORM_TEMPLATE_COLUMNS) * PLATFORM_PART_SIZE;

    for (int i = 0; i < PLATFORM_TEMPLATE_ROWS; i++) {
        for (int j = 0; j < PLATFORM_TEMPLATE_COLUMNS; j++) {
            if (platform->parts[i * PLATFORM_TEMPLATE_COLUMNS + j] == 0)
                continue;

            float x = hit_part_xm - (platform->x + j * PLATFORM_PART_SIZE);
            float y = hit_part_ym - (platform->y + i * PLATFORM_PART_SIZE);
            x = x * x;
            y = y * y;

            float c = sqrt(x + y);

            if (c <=
                PLATFORM_PROJECTILE_DIRECT_HIT_THRESHOLD * PLATFORM_PART_SIZE) {
                platform->parts[i * PLATFORM_TEMPLATE_COLUMNS + j] = 0;
            } else if (c <= PLATFORM_PROJECTILE_INDIRECT_HIT_THRESHOLD *
                                PLATFORM_PART_SIZE &&
                       round(get_random_float(0, 1))) {
                platform->parts[i * PLATFORM_TEMPLATE_COLUMNS + j] = 0;
            }
        }
    }
}

bool is_bullet_on_platform(Platform* platform, Bullet* bullet) {
    if (!is_rect_on_rect(platform->x, platform->y,
                         PLATFORM_TEMPLATE_COLUMNS * PLATFORM_PART_SIZE,
                         PLATFORM_TEMPLATE_ROWS * PLATFORM_PART_SIZE, bullet->x,
                         bullet->y, bullet->w, bullet->h))
        return false;

    for (int i = 0; i < PLATFORM_TEMPLATE_ROWS; i++) {
        for (int j = 0; j < PLATFORM_TEMPLATE_COLUMNS; j++) {
            if (platform->parts[i * PLATFORM_TEMPLATE_COLUMNS + j] == 0)
                continue;
            if (is_rect_on_rect(platform->x + j * PLATFORM_PART_SIZE,
                                platform->y + i * PLATFORM_PART_SIZE,
                                PLATFORM_PART_SIZE, PLATFORM_PART_SIZE,
                                bullet->x, bullet->y, bullet->w, bullet->h)) {
                return true;
            }
        }
    }

    return false;
}

int get_platform_bullet_hit_part_index(Platform* platform, Bullet* bullet) {
    if (!is_rect_on_rect(platform->x, platform->y,
                         PLATFORM_TEMPLATE_COLUMNS * PLATFORM_PART_SIZE,
                         PLATFORM_TEMPLATE_ROWS * PLATFORM_PART_SIZE, bullet->x,
                         bullet->y, bullet->w, bullet->h))
        return -1;

    for (int i = 0; i < PLATFORM_TEMPLATE_ROWS; i++) {
        for (int j = 0; j < PLATFORM_TEMPLATE_COLUMNS; j++) {
            if (platform->parts[i * PLATFORM_TEMPLATE_COLUMNS + j] == 0)
                continue;
            if (is_rect_on_rect(platform->x + j * PLATFORM_PART_SIZE,
                                platform->y + i * PLATFORM_PART_SIZE,
                                PLATFORM_PART_SIZE, PLATFORM_PART_SIZE,
                                bullet->x, bullet->y, bullet->w, bullet->h)) {
                return i * PLATFORM_TEMPLATE_COLUMNS + j;
            }
        }
    }

    return -1;
}