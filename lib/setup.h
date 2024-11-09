#pragma once

#include <SDL2/SDL_ttf.h>

typedef struct {
    TTF_Font* pixeled;
    TTF_Font* pixeled_small;
} Fonts;

Fonts get_fonts();