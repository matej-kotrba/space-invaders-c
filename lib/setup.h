#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    TTF_Font* pixeled;
    TTF_Font* pixeled_small;
} Fonts;

typedef struct {
    SDL_Cursor* def;
    SDL_Cursor* pointer;
} Cursors;

typedef struct {
    SDL_Texture* title;
    SDL_Texture* player_ship;
} Images;

Fonts get_fonts();

Cursors get_cursors();

Images get_images(SDL_Renderer* renderer);