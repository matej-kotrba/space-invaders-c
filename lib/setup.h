#pragma once

#include <SDL2/SDL_ttf.h>

typedef struct {
    TTF_Font* pixeled;
} Fonts;

Fonts get_fonts();