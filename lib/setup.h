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

Fonts get_fonts();

Cursors get_cursors();