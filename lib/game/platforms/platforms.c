#include "platforms.h"

#include <SDL2/SDL.h>
#include <stdlib.h>

#define PLATFORM_TEMPLATE_ROWS 10
#define PLATFORM_TEMPLATE_COLUMN 20
#define PLATFORM_PART_SIZE 4

static int
    platform_template[PLATFORM_TEMPLATE_ROWS * PLATFORM_TEMPLATE_COLUMN] = {
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
};

Platform create_new_platform(int x, int y) {
    Platform p = {
        .x = x,
        .y = y,
        .parts = (int*)malloc(PLATFORM_TEMPLATE_ROWS *
                              PLATFORM_TEMPLATE_COLUMN * sizeof(int))};

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
        for (int j = 0; j < PLATFORM_TEMPLATE_COLUMN * 2; j++) {
            int index = j < PLATFORM_TEMPLATE_COLUMN
                            ? j
                            : 2 * PLATFORM_TEMPLATE_COLUMN - 1 - j;
            if (platform_template[i * PLATFORM_TEMPLATE_COLUMN + index]) {
                rect.x = platform->x + j * PLATFORM_PART_SIZE;
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}