#include "setup.h"

#include <SDL2/SDL_ttf.h>

Fonts get_fonts() {
    Fonts fonts = {
        .pixeled = TTF_OpenFont(
            "../static/fonts/Pixelify_Sans/PixelifySans-Medium.ttf", 64),
        .pixeled_small = TTF_OpenFont(
            "../static/fonts/Pixelify_Sans/PixelifySans-Medium.ttf", 44)};

    return fonts;
}

Cursors get_cursors() {
    Cursors cursors = {.pointer =
                           SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND)};

    return cursors;
}